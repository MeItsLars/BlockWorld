#pragma once
#include <atomic>

class ObjectChecker {
public:
    static int getChunkCount() {
        return chunkCount.load();
    }

    static void addChunk() {
        chunkCount.operator++();
    }

    static void removeChunk() {
        chunkCount.operator--();
    }

    static int getSubChunkCount() {
        return subChunkCount.load();
    }

    static void addSubChunk() {
        subChunkCount.operator++();
    }

    static void removeSubChunk() {
        subChunkCount.operator--();
    }

    static int getRenderableCount() {
        return renderableCount.load();
    }

    static void addRenderable() {
        renderableCount.operator++();
    }

    static void removeRenderable() {
        renderableCount.operator--();
    }

    static int getMeshCount() {
        return meshCount.load();
    }

    static void addMesh() {
        meshCount.operator++();
    }

    static void removeMesh() {
        meshCount.operator--();
    }

    static int getShaderCount() {
        return shaderCount.load();
    }

    static void addShader() {
        shaderCount.operator++();
    }

    static void removeShader() {
        shaderCount.operator--();
    }

private:
    static std::atomic_int chunkCount;
    static std::atomic_int subChunkCount;
    static std::atomic_int renderableCount;
    static std::atomic_int meshCount;
    static std::atomic_int shaderCount;
};
