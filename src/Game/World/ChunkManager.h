#pragma once
#include <cstdint>
#include <memory>
#include <shared_mutex>
#include <unordered_map>

#include "Chunk.h"

class World;

// Maps chunk keys to chunks. Chunk key is combination of 32-bit X and Z coordinates.
typedef std::unordered_map<int64_t, std::shared_ptr<Chunk> > ChunkMap;

class ChunkManager {
public:
    explicit ChunkManager(World *world) : world(world) {
    }

    void start();

    void stop();

    float getWsps() {
        return wsps.load();
    }

    void chunkManager();

    ChunkMap getActiveChunksRO();

    size_t getActiveChunkCount() {
        std::shared_lock lock(chunkMapMutex);
        return activeChunkMapTS.size();
    }

    //
    //
    // void setChunk(int32_t chunkX, int32_t chunkZ, const std::shared_ptr<Chunk> &chunk);
    //
    // void deleteChunk(int64_t key);
    //
    // void deleteChunk(const int32_t chunkX, const int32_t chunkZ) {
    //     deleteChunk(Chunk::getChunkKey(chunkX, chunkZ));
    // }
    //

    void generateChunk(int32_t chunkX, int32_t chunkZ);

    std::shared_ptr<Chunk> getChunk(int32_t chunkX, int32_t chunkZ);

    std::shared_ptr<SubChunk> getSubChunk(int32_t subChunkX, int32_t subChunkY, int32_t subChunkZ);

private:
    // World reference
    World *world;

    ChunkMap activeChunkMapTS; // Active chunk map. Supports thread-safe read-only access.
    ChunkMap pendingChunkMapNTS; // Pending chunk map. Not thread-safe, only accessed by the chunk manager thread.
    std::shared_mutex chunkMapMutex; // Mutex for copying the pending chunk map to the active chunk map

    // Chunk generation thread
    std::thread chunkGenerationThread;
    std::atomic_bool running = false;

    // World scans per second (FPS variant of the chunk manager loop)
    std::atomic<float> wsps = 0;
};
