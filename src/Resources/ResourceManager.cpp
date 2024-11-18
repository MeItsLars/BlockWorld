#include "ResourceManager.h"

#include <fstream>

#include "OBJLoader.h"
#include "../Renderer/Mesh.h"
#include "../Renderer/RendererProvider.h"
#include "../Renderer/Texture.h"

std::unordered_map<std::string, std::shared_ptr<Mesh> > ResourceManager::meshes;
std::unordered_map<std::string, std::shared_ptr<Texture> > ResourceManager::textures;
std::unordered_map<ShaderProgramInfo, std::shared_ptr<IShader> > ResourceManager::shaders;

std::shared_ptr<Mesh> ResourceManager::getOrLoadMesh(const std::string &path) {
    auto it = meshes.find(path);
    if (it != meshes.end()) {
        return it->second;
    }

    OBJLoader loader;
    loader.load(path);
    std::vector<std::shared_ptr<Mesh> > &loadedMeshes = loader.getMeshes();
    if (loadedMeshes.empty()) {
        spdlog::error("No meshes loaded from '{}'.", path);
        return nullptr;
    }
    if (loadedMeshes.size() > 1) {
        spdlog::warn("Multiple meshes loaded from '{}'. Only the first one will be used.", path);
    }

    meshes[path] = loadedMeshes[0];
    return loadedMeshes[0];
}

std::shared_ptr<Texture> ResourceManager::getOrLoadTexture(const std::string &path) {
    auto it = textures.find(path);
    if (it != textures.end()) {
        return it->second;
    }

    std::shared_ptr<Texture> texture = std::make_shared<Texture>();
    texture->load(path);
    textures[path] = texture;
    return texture;
}

std::shared_ptr<IShader> ResourceManager::getOrLoadShader(const ShaderProgramInfo &source) {
    auto it = shaders.find(source);
    if (it != shaders.end()) {
        return it->second;
    }

    std::shared_ptr<IShader> shader = RendererProvider::createShader();
    shader->load(source);
    shaders[source] = shader;
    return shader;
}
