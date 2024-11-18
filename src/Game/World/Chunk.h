#pragma once
#include <cstdint>
#include <optional>
#include <shared_mutex>

#include "Block.h"
#include "SubChunk.h"

class World;

enum ChunkState {
    C_GENERATING_CHUNK,
    C_GENERATING_INITIAL_MESH,
    C_RENDER_READY,
    C_DELETING,
    C_DELETED
};

class Chunk {
public:
    static const int CHUNK_SIZE = 16;
    static const int SUBCHUNK_SIZE = CHUNK_SIZE;
    static const int WORLD_HEIGHT = CHUNK_SIZE * 16;
    static const int NUM_SUBCHUNKS = WORLD_HEIGHT / CHUNK_SIZE;

    Chunk(World *world, int32_t x, int32_t z);

    ~Chunk();

    World *getWorld() const {
        return world;
    }

    int32_t getX() const {
        return x;
    }

    int32_t getZ() const {
        return z;
    }

    void updateMesh();

    std::shared_ptr<SubChunk> getSubChunk(const int y) {
        if (y < 0 || y >= NUM_SUBCHUNKS) {
            return nullptr;
        }
        auto it = subChunks.find(y);
        return it == subChunks.end() ? nullptr : it->second;
    }

    std::shared_ptr<SubChunk> getOrCreateSubChunk(const int y) {
        if (y < 0 || y >= NUM_SUBCHUNKS) {
            return nullptr;
        }
        std::shared_ptr<SubChunk> subChunk = getSubChunk(y);
        if (subChunk == nullptr) {
            subChunk = std::make_shared<SubChunk>(this, x, y, z);
            subChunks[y] = subChunk;
        }
        return subChunk;
    }

    std::unordered_map<uint8_t, std::shared_ptr<SubChunk> > &getSubChunks() {
        return subChunks;
    }

    size_t render();

    [[nodiscard]] std::optional<BlockData> getBlock(int x, int y, int z) const {
        auto it = subChunks.find(y / CHUNK_SIZE);
        if (it == subChunks.end()) {
            return std::nullopt;
        }
        return it->second->getBlock(x % CHUNK_SIZE, y % CHUNK_SIZE, z % CHUNK_SIZE);
    }

    void setBlock(int x, int y, int z, BlockData block) {
        std::shared_ptr<SubChunk> subChunk = getOrCreateSubChunk(y / CHUNK_SIZE);
        if (subChunk != nullptr) {
            subChunk->setBlock(x % CHUNK_SIZE, y % CHUNK_SIZE, z % CHUNK_SIZE, block);
        }
    }

    ChunkState getStateTS() {
        std::shared_lock lock(stateMutex);
        return stateTS;
    }

    void setStateTS(const ChunkState state) {
        std::unique_lock lock(stateMutex);
        this->stateTS = state;
    }

    bool transitionStateTS(const ChunkState expected, const ChunkState desired) {
        std::unique_lock lock(stateMutex);
        if (stateTS == expected) {
            stateTS = desired;
            return true;
        }
        return false;
    }

    static int64_t getChunkKey(int32_t x, int32_t z) {
        return (static_cast<int64_t>(x) << 32) | (static_cast<int64_t>(z) & 0xFFFFFFFF);
    }

    static int32_t getChunkX(int64_t key) {
        return static_cast<int32_t>(key >> 32);
    }

    static int32_t getChunkZ(int64_t key) {
        return static_cast<int32_t>(key & 0xFFFFFFFF);
    }

private:
    World *world;
    int32_t x, z;
    std::unordered_map<uint8_t, std::shared_ptr<SubChunk> > subChunks;
    // Ideas:
    // - Change storage type, maybe to: map<uint16_t, BlockData> blocks;
    //   Where the key is computed as ((x << 8) | (y << 4) | z) in that subchunk
    //   Or as ((x << 12) | (y << 8) | z) in the chunk

    ChunkState stateTS{C_GENERATING_CHUNK};
    std::shared_mutex stateMutex;
};
