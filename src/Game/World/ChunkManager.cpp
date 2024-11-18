#include "ChunkManager.h"

#include <glm/gtx/norm.hpp>

#include "World.h"
#include "../TaskScheduler.h"

void ChunkManager::start() {
    spdlog::info("Starting chunk manager...");
    running.store(true);
    chunkGenerationThread = std::thread(&ChunkManager::chunkManager, this);
}

void ChunkManager::stop() {
    spdlog::info("Stopping chunk manager...");
    running.store(false);
    if (chunkGenerationThread.joinable()) {
        chunkGenerationThread.join();
    }
    spdlog::info("Chunk manager stopped.");
}

void ChunkManager::chunkManager() {
    spdlog::info("Chunk manager started");

    auto lastTime = std::chrono::high_resolution_clock::now();
    float worldScanCount = 0;

    while (running.load()) {
        // Calculate WSPS
        auto currentTime = std::chrono::high_resolution_clock::now();
        std::chrono::duration<float> deltaTime = currentTime - lastTime;
        worldScanCount++;
        if (deltaTime.count() >= 1.0f) {
            wsps.store(worldScanCount / deltaTime.count());
            worldScanCount = 0;
            lastTime = currentTime;
        }

        // Get relevant information
        const glm::vec3 center = world->getPlayer()->getLocation();
        glm::vec2 centerChunk = glm::vec2(center.x / Chunk::CHUNK_SIZE, center.z / Chunk::CHUNK_SIZE);
        // TODO: Perhaps ignore updates if the player hasn't moved far enough
        // TODO: Can be done with a local variable that stores the last centerChunk

        auto renderDistance = world->getRenderDistance();
        auto renderDistanceSquared = static_cast<float>(world->getRenderDistanceSquared());

        // Step 1: Check which chunks need to be marked as 'to be deleted' and which chunks are finished deleting
        int markedDeletions = 0;
        int markedDeletionsCompleted = 0;
        for (auto it = pendingChunkMapNTS.begin(); it != pendingChunkMapNTS.end();) {
            if (it->second->getStateTS() == C_DELETED) {
                // If a chunk is finished deleting, remove it from the pending chunk map
                it = pendingChunkMapNTS.erase(it);
                markedDeletionsCompleted++;
            } else {
                // If a chunk is outside the render distance, mark it for deletion
                const int32_t chunkX = it->second->getX();
                const int32_t chunkZ = it->second->getZ();
                if (distance2(glm::vec2(chunkX, chunkZ), centerChunk) > renderDistanceSquared) {
                    // Mark the chunk for deletion. This will cause the OpenGL thread to release OpenGL resources for the chunk
                    if (it->second->transitionStateTS(C_RENDER_READY, C_DELETING)) {
                        markedDeletions++;
                    }
                }
                ++it;
            }
        }

        // Step 2: Generate new chunks around the player within the render distance
        // Spiral outward from (0, 0)
        int markedGenerations = 0;
        for (int radius = 0; radius <= renderDistance; ++radius) {
            for (int x = -radius; x <= radius; ++x) {
                for (int z = -radius; z <= radius; ++z) {
                    if (std::max(std::abs(x), std::abs(z)) != radius) continue;
                    // Only process the current layer of the spiral
                    int32_t chunkX = static_cast<int32_t>(centerChunk.x) + x;
                    int32_t chunkZ = static_cast<int32_t>(centerChunk.y) + z;
                    int64_t chunkKey = Chunk::getChunkKey(chunkX, chunkZ);

                    // Skip chunks that are already in the chunk map (being generated, already generated, or being deleted)
                    if (pendingChunkMapNTS.find(chunkKey) != pendingChunkMapNTS.end()) {
                        continue;
                    }

                    // Skip chunks outside the render distance
                    if (distance2(glm::vec2(chunkX, chunkZ), centerChunk) > renderDistanceSquared) {
                        continue;
                    }

                    generateChunk(chunkX, chunkZ);
                    markedGenerations++;
                }
            }
        }

        // Step 3: Update the active chunk map
        auto start = std::chrono::high_resolution_clock::now(); {
            std::unique_lock lock(chunkMapMutex);
            activeChunkMapTS = pendingChunkMapNTS;
        }
        auto end = std::chrono::high_resolution_clock::now();
        auto duration = std::chrono::duration_cast<std::chrono::microseconds>(end - start);
        if (duration > std::chrono::microseconds(0)) {
            // spdlog::info("Updated active chunk map in {} microseconds", duration.count());
        }

        if (markedDeletions > 0 || markedGenerations > 0 || markedDeletionsCompleted > 0) {
            spdlog::info("Marked {} deletions, {} generations, and {} deletions completed", markedDeletions,
                         markedGenerations, markedDeletionsCompleted);
        }
    }
}

ChunkMap ChunkManager::getActiveChunksRO() {
    std::shared_lock lock(chunkMapMutex);
    return activeChunkMapTS;
}

void ChunkManager::generateChunk(int32_t chunkX, int32_t chunkZ) {
    // While still in the chunk manager thread, add the chunk to the pending chunk map
    const auto chunk = std::make_shared<Chunk>(world, chunkX, chunkZ);
    const int64_t chunkKey = Chunk::getChunkKey(chunkX, chunkZ);
    pendingChunkMapNTS[chunkKey] = chunk;

    // Start a new task to generate the chunk
    TaskSchedulers::WORLD_GEN.addTask(TaskPriority::NORMAL, [this, chunk, chunkX, chunkZ] {
        world->getWorldGenerator()->generateChunk(chunk, chunkX, chunkZ);
        chunk->transitionStateTS(C_GENERATING_CHUNK, C_GENERATING_INITIAL_MESH);

        TaskSchedulers::MESH_GEN.addTask(TaskPriority::NORMAL, [chunk] {
            // Generate the mesh. Afterward, the chunk is ready to be rendered
            chunk->updateMesh();
            chunk->transitionStateTS(C_GENERATING_INITIAL_MESH, C_RENDER_READY);
        });

        // For all 4 chunks around this chunk:
        // - If the chunk is in the 'C_GENERATING_INITIAL_MESH' or 'C_RENDER_READY' state, update the mesh
        //   In all other cases, no new mesh generation is necessary
        for (int i = 0; i < 4; i++) {
            const int32_t x = chunkX + (i == 0 ? 1 : (i == 1 ? -1 : 0));
            const int32_t z = chunkZ + (i == 2 ? 1 : (i == 3 ? -1 : 0));
            std::shared_ptr<Chunk> adjacentChunk = getChunk(x, z);
            if (adjacentChunk != nullptr) {
                ChunkState state = adjacentChunk->getStateTS();
                if (state == C_GENERATING_INITIAL_MESH || state == C_RENDER_READY) {
                    TaskSchedulers::MESH_GEN.addTask(TaskPriority::NORMAL, [adjacentChunk] {
                        adjacentChunk->updateMesh();
                    });
                }
            }
        }
    });
}


std::shared_ptr<Chunk> ChunkManager::getChunk(int32_t chunkX, int32_t chunkZ) {
    int64_t key = Chunk::getChunkKey(chunkX, chunkZ); {
        std::shared_lock lock(chunkMapMutex);
        const auto it = activeChunkMapTS.find(key);
        return it == activeChunkMapTS.end() ? nullptr : it->second;
    }
}

std::shared_ptr<SubChunk> ChunkManager::getSubChunk(int32_t subChunkX, int32_t subChunkY, int32_t subChunkZ) {
    std::shared_ptr<Chunk> chunk = getChunk(subChunkX, subChunkZ);
    return chunk == nullptr ? nullptr : chunk->getSubChunk(subChunkY);
}
