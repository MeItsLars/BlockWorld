#pragma once

#include <string>

#include "../Renderer/IShader.h"

namespace ResourcePaths {
    const std::string ENTITY_MESH = "res/models/cube.obj";
    const std::string ENTITY_TEXTURE = "res/textures/white.png";

    const std::string BLOCK_ATLAS_TEXTURE = "res/textures/block_atlas.png";

    const std::string DEFAULT_SHADER_VERTEX = "res/shaders/default.vert";
    const std::string DEFAULT_SHADER_FRAGMENT = "res/shaders/default.frag";
    const std::string CHUNK_SHADER_VERTEX = "res/shaders/chunk.vert";
    const std::string CHUNK_SHADER_FRAGMENT = "res/shaders/chunk.frag";
}

namespace Shaders {
    const ShaderProgramInfo DEFAULT = {
        ResourcePaths::DEFAULT_SHADER_VERTEX,
        ResourcePaths::DEFAULT_SHADER_FRAGMENT,
        {
            {"vPosition", AttributeType::FLOAT, 3, sizeof(Vertex)},
            {"vNormal", AttributeType::FLOAT, 3, sizeof(Vertex)}
        }
    };

    const ShaderProgramInfo CHUNK = {
        ResourcePaths::CHUNK_SHADER_VERTEX,
        ResourcePaths::CHUNK_SHADER_FRAGMENT,
        {
            {"vPosition", AttributeType::FLOAT, 3, sizeof(Vertex)},
            {"vNormal", AttributeType::FLOAT, 3, sizeof(Vertex)},
            {"vTexCoord", AttributeType::FLOAT, 2, sizeof(Vertex)}
        }
    };
}
