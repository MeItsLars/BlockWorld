#pragma once
#include <memory>
#include <unordered_map>

#include "../Renderer/Mesh.h"
#include "../Renderer/Texture.h"
#include "../Renderer/IShader.h"

class ResourceManager {
public:
    ~ResourceManager() = default;

    static void releaseResources() {
        meshes.clear();
        textures.clear();
        shaders.clear();
    }

    static std::shared_ptr<Mesh> getOrLoadMesh(const std::string &path);

    static std::shared_ptr<Texture> getOrLoadTexture(const std::string &path);

    static std::shared_ptr<IShader> getOrLoadShader(const ShaderProgramInfo &source);

private:
    // Keys of the format "path/to/mesh.obj"
    static std::unordered_map<std::string, std::shared_ptr<Mesh> > meshes;
    // Keys of the format "path/to/texture.png"
    static std::unordered_map<std::string, std::shared_ptr<Texture> > textures;
    // Keys of the format "path/to/vertex_shader.glsl" (but contains all shader types)
    static std::unordered_map<ShaderProgramInfo, std::shared_ptr<IShader> > shaders;
};
