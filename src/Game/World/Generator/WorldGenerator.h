#pragma once
#include <FastNoiseLite.h>
#include <memory>

#include "../Chunk.h"

class WorldGenerator {
public:
    WorldGenerator();

    void generateChunk(const std::shared_ptr<Chunk> &chunk, int32_t chunkX, int32_t chunkZ) const;

private:
    FastNoiseLite temperatureNoise;
    FastNoiseLite humidityNoise;
    FastNoiseLite terrainNoise;
};
