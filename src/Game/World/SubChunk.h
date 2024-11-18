#pragma once
#include <shared_mutex>

#include "Block.h"
#include "../../Renderer/IRenderable.h"

class Chunk;

enum SubChunkState {
    SC_NO_MESH, // The subchunk is empty and does not currently render anything
    SC_NEW_MESH_READY, // The mesh generator has generated a new mesh that is ready to be stored on the GPU
    SC_DELETE_MESH,
    // The mesh generator has generated an empty mesh and therefore requires the existing mesh/renderable to be deleted to free up (GPU) memory
    SC_RENDER_READY, // The subchunk has a mesh that is ready to be rendered
    SC_DELETED // The subchunk is part of a chunk that is being deleted and all OpenGL resources should be deleted
};

class SubChunk {
public:
    static constexpr int SUBCHUNK_SIZE = 16;

    SubChunk(Chunk *chunk, int32_t x, int32_t y, int32_t z);

    ~SubChunk();

    SubChunkState getStateTS() {
        std::shared_lock lock(stateMutex);
        return stateTS;
    }

    void setStateTS(const SubChunkState newState) {
        std::unique_lock lock(stateMutex);
        stateTS = newState;
    }

    bool transitionStateTS(const SubChunkState expected, const SubChunkState desired) {
        std::unique_lock lock(stateMutex);
        if (stateTS == expected) {
            stateTS = desired;
            return true;
        }
        return false;
    }

    void updateMesh();

    size_t render();

    void deleteRenderable();

    [[nodiscard]] BlockData getBlock(int x, int y, int z) const {
        // TODO: Add thread-safety
        return blocks[x][y][z];
    }

    void setBlock(int x, int y, int z, BlockData block) {
        // TODO: Add thread-safety
        blocks[x][y][z] = block;
    }

private:
    Chunk *chunk;
    const int32_t sx;
    const int32_t sy;
    const int32_t sz;

    // SubChunk version. Increment whenever a new mesh is generated, so we can keep track of the latest mesh
    // The mutex protects the start and end phases of the mesh generation, resulting in two useful properties:
    // 1. Only 1 mesh generation thread can retrieve the last world state and update the version variable.
    //    This ensures that the latest version always corresponds to the latest world state.
    // 2. When a mesh is finished, the mesh generation algorithm claims the mutex
    int version{0};
    std::mutex versionMutex;

    // TODO: Make this thread-safe? Right now not a problem because nothing ever modifies the blocks array after initialization
    BlockData blocks[SUBCHUNK_SIZE][SUBCHUNK_SIZE][SUBCHUNK_SIZE];
    std::unique_ptr<IRenderable> renderable;

    // The new mesh that is ready to be sent to the GPU
    std::unique_ptr<Mesh> newMesh = nullptr;

    SubChunkState stateTS = SC_NO_MESH;
    std::shared_mutex stateMutex;
};
