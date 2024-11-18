#include "Chunk.h"

#include "World.h"
#include "../../UI/ObjectChecker.h"

Chunk::Chunk(World *world, const int32_t x, const int32_t z) : world(world), x(x), z(z) {
    ObjectChecker::addChunk();
}

Chunk::~Chunk() {
    ObjectChecker::removeChunk();
}

void Chunk::updateMesh() {
    for (auto &[fst, snd]: subChunks) {
        snd->updateMesh();
    }
}

size_t Chunk::render() {
    size_t vertexCount = 0;
    switch (getStateTS()) {
        case C_RENDER_READY:
            // Only render the chunk if it is ready to be rendered
            for (const auto &[fst, snd]: subChunks) {
                vertexCount += snd->render();
            }
            break;
        case C_DELETING:
            // No other operations can happen when the chunk is in 'C_DELETING' state, so we can safely change the state
            // without using 'transitionStateTS'

            // Delete the OpenGL resources (has to be done in the rendering thread) of the subchunks
            for (const auto &[fst, snd]: subChunks) {
                snd->setStateTS(SC_DELETED);
                snd->deleteRenderable();
            }
        // Mark the chunk as deleted, as all subchunks have been deleted
            setStateTS(C_DELETED);
            break;
        default:
            break;
    }
    return vertexCount;
}
