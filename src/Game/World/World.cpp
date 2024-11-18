#include "World.h"

#include <glm/gtx/norm.hpp>

#include "../TaskScheduler.h"
#include "../../Resources/ResourceManager.h"
#include "../../Resources/ResourcePaths.h"
#include "spdlog/spdlog.h"

World::World(Camera *playerCamera) : chunkManager(this), player(playerCamera) {
    // TODO: Change this
    spdlog::info("Setting chunk shader texture...");
    IShader *chunkShader = ResourceManager::getOrLoadShader(Shaders::CHUNK).get();
    chunkShader->use();
    chunkShader->setTexture("tex", ResourceManager::getOrLoadTexture(ResourcePaths::BLOCK_ATLAS_TEXTURE).get());

    chunkManager.start();
}

World::~World() {
    // TODO: Does this need anything?
    chunkManager.stop();
}

Entity *World::spawnEntity(glm::vec3 position) {
    auto entity = std::make_unique<Entity>();
    entities.push_back(std::move(entity));
    // TODO: Set the position of the entity
    return entities.back().get();
}

size_t World::render(const Camera *camera) {
    auto startTime = std::chrono::high_resolution_clock::now();

    // Create a thread-safe copy of the chunks
    // TODO: This call can take very long because the world generation thread constantly keeps accessing this data and executing expensive copy operations
    const ChunkMap readOnlyChunkMap = chunkManager.getActiveChunksRO();

    auto accessTime = std::chrono::high_resolution_clock::now();

    size_t vertexCount = 0;
    for (const auto &entity: entities) {
        vertexCount += entity->render(camera);
    }

    auto entityRenderTime = std::chrono::high_resolution_clock::now();

    // TODO: Only update P and V matrices when they change
    IShader *chunkShader = ResourceManager::getOrLoadShader(Shaders::CHUNK).get();
    chunkShader->use();
    chunkShader->setUniformMatrix4fv("V", camera->viewMatrix);
    chunkShader->setUniformMatrix4fv("P", camera->projectionMatrix);

    auto shaderSetupTime = std::chrono::high_resolution_clock::now();

    for (const auto &[fst, snd]: readOnlyChunkMap) {
        vertexCount += snd->render();
    }

    auto chunkRenderTime = std::chrono::high_resolution_clock::now();
    auto endTime = std::chrono::high_resolution_clock::now();

    auto deltaAccessTime = std::chrono::duration_cast<std::chrono::milliseconds>(accessTime - startTime).count();
    auto deltaEntityRenderTime = std::chrono::duration_cast<std::chrono::milliseconds>(entityRenderTime - accessTime).count();
    auto deltaShaderSetupTime = std::chrono::duration_cast<std::chrono::milliseconds>(shaderSetupTime - entityRenderTime).count();
    auto deltaChunkRenderTime = std::chrono::duration_cast<std::chrono::milliseconds>(chunkRenderTime - shaderSetupTime).count();
    auto deltaTotalTime = std::chrono::duration_cast<std::chrono::milliseconds>(endTime - startTime).count();

    if (deltaTotalTime > 30) {
        spdlog::warn("World render took {}ms ({}ms access, {}ms entity render, {}ms shader setup, {}ms chunk render)",
                     deltaTotalTime, deltaAccessTime, deltaEntityRenderTime, deltaShaderSetupTime, deltaChunkRenderTime);
    }

    return vertexCount;
}
