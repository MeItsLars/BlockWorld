#pragma once

#include <memory>
#include <string>
#include <tiny_obj_loader.h>
#include <vector>
#include "../Renderer/Mesh.h"

struct index_t {
    unsigned int faceIndex;
    tinyobj::index_t index;
};

class OBJLoader {
public:
    void load(const std::string &path);

    std::vector<std::shared_ptr<Mesh> > &getMeshes() {
        return meshes;
    }

private:
    // Vector of all the loaded meshes from the OBJ file
    std::vector<std::shared_ptr<Mesh> > meshes;

    // Index offsets. They start at 1 as the obj file format uses 1-based indexing
    int vertexOffset = 1;
    int normalOffset = 1;
    int texCoordOffset = 1;

    // Information of the entire object file
    std::vector<Material> materials;

    // Information of the object that is currently being loaded
    std::string currentMeshName;
    std::vector<glm::vec4> vertices;
    std::vector<glm::vec3> normals;
    std::vector<glm::vec2> texCoords;
    std::vector<index_t> indices;
    std::vector<glm::uvec3> faces;

    void completeCurrentMesh();

    void startNextMesh(const std::string &name);

    static void vertexCallback(void *user_data, tinyobj::real_t x, tinyobj::real_t y, tinyobj::real_t z,
                               tinyobj::real_t w);

    static void normalCallback(void *user_data, tinyobj::real_t x, tinyobj::real_t y, tinyobj::real_t z);

    static void texCoordCallback(void *user_data, tinyobj::real_t x, tinyobj::real_t y, tinyobj::real_t z);

    static void indexCallback(void *user_data, tinyobj::index_t *indices, int num_indices);

    static void usemtlCallback(void *user_data, const char *name, int material_index);

    static void mtllibCallback(void *user_data, const tinyobj::material_t *materials, int num_materials);

    static void objectCallback(void *user_data, const char *name);
};
