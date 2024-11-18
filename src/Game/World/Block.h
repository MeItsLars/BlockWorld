#pragma once

#include <cstdint>
#include <glm/vec2.hpp>
#include <glm/vec3.hpp>

#include "spdlog/spdlog.h"

enum class BlockType : uint8_t {
    AIR,
    DIRT,
    GRASS,
    STONE,
    SAND,
    WATER,
};

struct BlockData {
    BlockType type = BlockType::AIR;
};

namespace Blocks {
    constexpr int ATLAS_RESOLUTION = 16;
    constexpr int ATLAS_WIDTH = 4;
    constexpr int ATLAS_HEIGHT = 4;

    struct Face {
        glm::vec3 normal;
        glm::ivec3 vertexOffsets[4];
    };

    constexpr Face FACE_EAST{{1, 0, 0}, {{1, 0, 0}, {1, 0, 1}, {1, 1, 1}, {1, 1, 0}}};
    constexpr Face FACE_WEST{{-1, 0, 0}, {{0, 0, 1}, {0, 0, 0}, {0, 1, 0}, {0, 1, 1}}};
    constexpr Face FACE_UP{{0, 1, 0}, {{0, 1, 0}, {1, 1, 0}, {1, 1, 1}, {0, 1, 1}}};
    constexpr Face FACE_DOWN{{0, -1, 0}, {{0, 0, 1}, {1, 0, 1}, {1, 0, 0}, {0, 0, 0}}};
    constexpr Face FACE_NORTH{{0, 0, -1}, {{0, 0, 0}, {1, 0, 0}, {1, 1, 0}, {0, 1, 0}}};
    constexpr Face FACE_SOUTH{{0, 0, 1}, {{1, 0, 1}, {0, 0, 1}, {0, 1, 1}, {1, 1, 1}}};

    inline bool isSolid(const BlockType type) {
        return type != BlockType::AIR;
    }

    inline glm::vec2 getTextureCoordinates(const BlockType type) {
        switch (type) {
            case BlockType::DIRT:
                return {0, 0};
            case BlockType::GRASS:
                return {1, 0};
            case BlockType::STONE:
                return {2, 0};
            default:
                spdlog::error("No texture coordinates for block type {}", static_cast<uint8_t>(type));
                return {0, 0};
        }
    }
}
