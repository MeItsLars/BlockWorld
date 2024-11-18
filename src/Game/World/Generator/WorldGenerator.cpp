#include "WorldGenerator.h"

WorldGenerator::WorldGenerator() {
    terrainNoise.SetSeed(0);
    terrainNoise.SetFrequency(0.01f);
    terrainNoise.SetNoiseType(FastNoiseLite::NoiseType_OpenSimplex2);
}

void WorldGenerator::generateChunk(const std::shared_ptr<Chunk> &chunk, const int32_t chunkX,
                                   const int32_t chunkZ) const {
    int32_t baseX = chunkX * Chunk::CHUNK_SIZE;
    int32_t baseZ = chunkZ * Chunk::CHUNK_SIZE;

    for (int x = 0; x < Chunk::CHUNK_SIZE; x++) {
        for (int z = 0; z < Chunk::CHUNK_SIZE; z++) {
            for (int y = 0; y < 2 * Chunk::CHUNK_SIZE - 4; y++) chunk->setBlock(x, y, z, {BlockType::STONE});
            for (int y = 2 * Chunk::CHUNK_SIZE - 4; y < 2 * Chunk::CHUNK_SIZE - 1; y++) chunk->setBlock(
                x, y, z, {BlockType::DIRT});
            chunk->setBlock(x, 2 * Chunk::CHUNK_SIZE - 1, z, {BlockType::GRASS});
        }
    }

    // for (int x = 0; x < Chunk::CHUNK_SIZE; x++) {
    //     for (int z = 0; z < Chunk::CHUNK_SIZE; z++) {
    //         float heightF = (terrainNoise.GetNoise((baseX + x) * 1.0f, (baseZ + z) * 1.0f) + 1.0f) / 2.0f;
    //         int height = static_cast<int>(20 + heightF * 20);
    //
    //         for (int y = 0; y < height - 4; y++) chunk->setBlock(x, y, z, { BlockType::STONE });
    //         for (int y = height - 4; y < height; y++) chunk->setBlock(x, y, z, { BlockType::DIRT });
    //         chunk->setBlock(x, height, z, { BlockType::GRASS });
    //     }
    // }
}
