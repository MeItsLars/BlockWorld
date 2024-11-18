#include "SubChunk.h"

#include "Chunk.h"
#include "World.h"
#include "../../Renderer/RendererProvider.h"
#include "../../Resources/ResourceManager.h"
#include "../../Resources/ResourcePaths.h"
#include "../../UI/ObjectChecker.h"

SubChunk::SubChunk(Chunk *chunk, const int32_t x, const int32_t y, const int32_t z) : chunk(chunk), sx(x), sy(y),
    sz(z) {
    ObjectChecker::addSubChunk();
}

SubChunk::~SubChunk() {
    deleteRenderable();
    ObjectChecker::removeSubChunk();
}

void addFace(Mesh *mesh, Blocks::Face face, BlockType type, int x, int y, int z) {
    const size_t index = mesh->vertices.size();

    // Compute texture coordinates
    glm::vec2 texCoordOffsets = Blocks::getTextureCoordinates(type);
    float leftX = texCoordOffsets.x / Blocks::ATLAS_WIDTH;
    float rightX = (texCoordOffsets.x + 1) / Blocks::ATLAS_WIDTH;
    float topY = texCoordOffsets.y / Blocks::ATLAS_HEIGHT;
    float bottomY = (texCoordOffsets.y + 1) / Blocks::ATLAS_HEIGHT;
    glm::vec2 texCoords[4] = {{rightX, bottomY}, {leftX, bottomY}, {leftX, topY}, {rightX, topY}};

    // Add vertices
    for (int i = 0; i < 4; i++) {
        mesh->vertices.push_back({
            {x + face.vertexOffsets[i].x, y + face.vertexOffsets[i].y, z + face.vertexOffsets[i].z},
            face.normal, texCoords[i]
        });
    }

    // Add face indices
    mesh->faces.push_back({{index, index + 2, index + 1}});
    mesh->faces.push_back({{index, index + 3, index + 2}});
}

void SubChunk::updateMesh() {
    // Declare variables that will be set in the locked section
    int startVersion;
    std::shared_ptr<SubChunk> east;
    std::shared_ptr<SubChunk> west;
    std::shared_ptr<SubChunk> up;
    std::shared_ptr<SubChunk> down;
    std::shared_ptr<SubChunk> north;
    std::shared_ptr<SubChunk> south;

    // Use the lock to make sure only 1 thread can retrieve world state and increment the chunk version at the same time
    // The consequence of this is that the highest version will ALWAYS correspond to the most recent world state
    {
        std::unique_lock lock(versionMutex);
        version++;
        startVersion = version;

        // TODO: Retrieve adjacent subchunks from world
        // snd->updateMesh(
        //     world->getSubChunk(x, fst, z - 1),
        //     world->getSubChunk(x, fst, z + 1),
        //     world->getSubChunk(x + 1, fst, z),
        //     world->getSubChunk(x - 1, fst, z),
        //     fst == NUM_SUBCHUNKS - 1 ? nullptr : subChunks[fst + 1],
        //     fst == 0 ? nullptr : subChunks[fst - 1]
        // );
        // east = chunk->getWorld()->getChunkManager()->getSubChunk(sx + 1, sy, sz);
        // west = chunk->getWorld()->getChunkManager()->getSubChunk(sx - 1, sy, sz);
        // up = chunk->getSubChunk(sy + 1);
        // down = chunk->getSubChunk(sy - 1);
        // north = chunk->getWorld()->getChunkManager()->getSubChunk(sx, sy, sz - 1);
        // south = chunk->getWorld()->getChunkManager()->getSubChunk(sx, sy, sz + 1);
    }

    auto mesh = std::make_unique<Mesh>();

    for (int x = 0; x < SUBCHUNK_SIZE; x++) {
        for (int y = 0; y < SUBCHUNK_SIZE; y++) {
            for (int z = 0; z < SUBCHUNK_SIZE; z++) {
                BlockData block = getBlock(x, y, z);
                if (block.type == BlockType::AIR) {
                    continue;
                }

                // Check if the block is visible
                bool eastVis = (x == SUBCHUNK_SIZE - 1)
                                   ? (east == nullptr || !Blocks::isSolid(east->getBlock(0, y, z).type))
                                   : !Blocks::isSolid(getBlock(x + 1, y, z).type);
                bool westVis = (x == 0)
                                   ? (west == nullptr || !Blocks::isSolid(west->getBlock(SUBCHUNK_SIZE - 1, y, z).type))
                                   : !Blocks::isSolid(getBlock(x - 1, y, z).type);
                bool upVis = (y == SUBCHUNK_SIZE - 1)
                                 ? (up == nullptr || !Blocks::isSolid(up->getBlock(x, 0, z).type))
                                 : !Blocks::isSolid(getBlock(x, y + 1, z).type);
                bool downVis = (y == 0)
                                   ? (down == nullptr || !Blocks::isSolid(down->getBlock(x, SUBCHUNK_SIZE - 1, z).type))
                                   : !Blocks::isSolid(getBlock(x, y - 1, z).type);
                bool northVis = (z == 0)
                                    ? (north == nullptr || !Blocks::isSolid(
                                           north->getBlock(x, y, SUBCHUNK_SIZE - 1).type))
                                    : !Blocks::isSolid(getBlock(x, y, z - 1).type);
                bool southVis = (z == SUBCHUNK_SIZE - 1)
                                    ? (south == nullptr || !Blocks::isSolid(south->getBlock(x, y, 0).type))
                                    : !Blocks::isSolid(getBlock(x, y, z + 1).type);

                if (eastVis) addFace(mesh.get(), Blocks::FACE_EAST, block.type, x, y, z);
                if (westVis) addFace(mesh.get(), Blocks::FACE_WEST, block.type, x, y, z);
                if (upVis) addFace(mesh.get(), Blocks::FACE_UP, block.type, x, y, z);
                if (downVis) addFace(mesh.get(), Blocks::FACE_DOWN, block.type, x, y, z);
                if (northVis) addFace(mesh.get(), Blocks::FACE_NORTH, block.type, x, y, z);
                if (southVis) addFace(mesh.get(), Blocks::FACE_SOUTH, block.type, x, y, z);
            }
        }
    }

    // Lock the version mutex to handle mesh updating
    {
        std::unique_lock versionLock(versionMutex);

        if (startVersion < this->version) {
            // Only update the mesh if no newer mesh generation is currently being generated (or already finished)
            // TODO: This might cause issues when placing blocks very quickly, as that would keep cancelling the mesh generation
            // TODO: A way to fix this might be to maintain two versions, one that depicts the latest 'in-progress version'
            // TODO: And one that depicts the latest 'finished version'. It would be okay to overwrite the 'finished version',
            // TODO: As long as our version is newer.
        } else {
            std::unique_lock stateLock(stateMutex);

            switch (stateTS) {
                case SC_NO_MESH:
                case SC_NEW_MESH_READY:
                case SC_RENDER_READY:
                case SC_DELETE_MESH:
                    // In all these cases, we can safely update the mesh (and create a new renderable if needed)

                    if (mesh->getFaceCount() > 0) {
                        // If the chunk contains blocks, update the renderable mesh
                        newMesh = std::move(mesh);
                        stateTS = SC_NEW_MESH_READY;
                    } else {
                        // If the chunk doesn't contain any visible faces, mark mesh as ready to be deleted from GPU
                        // (it doesn't matter if there was no mesh in the first place, the renderer will handle that)
                        stateTS = SC_DELETE_MESH;
                    }
                    break;
                case SC_DELETED:
                    // Do not update the mesh if the subchunk is already deleted
                    break;
            }
        }
    }
}

void SubChunk::deleteRenderable() {
    if (renderable != nullptr) {
        // Delete the renderable from memory
        renderable.reset();
    }
}

size_t SubChunk::render() {
    std::shared_ptr<Mesh> newRenderableMesh = nullptr;
    bool deleteExistingRenderable = false;

    // Lock the state mutex
    {
        std::unique_lock lock(stateMutex);
        switch (stateTS) {
            case SC_NO_MESH:
            case SC_DELETED:
                // If there is no mesh or the subchunk is marked as deleted, there is nothing to render
                return 0;
            case SC_DELETE_MESH:
                // If a request to delete the current renderable has been made, schedule the deletion
                deleteExistingRenderable = true;
                stateTS = SC_NO_MESH;
                break;
            case SC_NEW_MESH_READY:
                // If a new mesh is ready, schedule mesh replacement (and optional renderable creation)
                newRenderableMesh = std::move(newMesh);
                stateTS = SC_RENDER_READY;
                break;
            default:
                // The subchunk is in a state where it can be rendered
                break;
        }
    }

    if (deleteExistingRenderable) {
        deleteRenderable();
        return 0;
    }

    if (newRenderableMesh != nullptr) {
        if (renderable == nullptr) {
            renderable = RendererProvider::createRenderable();
            renderable->setShaderProgram(ResourceManager::getOrLoadShader(Shaders::CHUNK));
            renderable->getModel().getTransform().translate(
                static_cast<float>(sx) * SUBCHUNK_SIZE,
                static_cast<float>(sy) * SUBCHUNK_SIZE,
                static_cast<float>(sz) * SUBCHUNK_SIZE
            );
        }
        renderable->setMesh(newRenderableMesh);
    }

    // At this point, the current renderable is in a safe state to be rendered
    renderable->getShaderProgram()->setUniformMatrix4fv("M", renderable->getModel().getTransform().getModelMatrix());
    return renderable->render();
}
