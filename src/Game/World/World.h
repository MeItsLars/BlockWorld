#pragma once
#include <vector>
#include <glm/vec3.hpp>

#include "ChunkManager.h"
#include "../Entity.h"
#include "../Player.h"
#include "Generator/WorldGenerator.h"

class World {
public:
    World(Camera *playerCamera);

    ~World();

    WorldGenerator *getWorldGenerator() {
        return &worldGenerator;
    }

    ChunkManager *getChunkManager() {
        return &chunkManager;
    }

    int getRenderDistance() const {
        return renderDistance;
    }

    int getRenderDistanceSquared() const {
        return renderDistanceSquared;
    }

    void setRenderDistance(int renderDistance) {
        this->renderDistance = renderDistance;
        this->renderDistanceSquared = renderDistance * renderDistance;
    }

    Player *getPlayer() {
        return &player;
    }

    Entity *spawnEntity(glm::vec3 position);

    size_t render(const Camera *camera);

private:
    WorldGenerator worldGenerator;
    ChunkManager chunkManager;

    int renderDistance = 1;
    int renderDistanceSquared = renderDistance * renderDistance;

    Player player;
    std::vector<std::unique_ptr<Entity> > entities;
};
