#define TINYOBJLOADER_IMPLEMENTATION

#include "OBJLoader.h"
#include <filesystem>

#include "spdlog/spdlog.h"

void OBJLoader::load(const std::string &path) {
    spdlog::info("[load] OBJ file: {}", path);

    // Initialize the first mesh
    startNextMesh("default");

    // Callbacks
    tinyobj::callback_t callback;
    callback.vertex_cb = vertexCallback;
    callback.normal_cb = normalCallback;
    callback.texcoord_cb = texCoordCallback;
    callback.index_cb = indexCallback;
    callback.usemtl_cb = usemtlCallback;
    callback.mtllib_cb = mtllibCallback;
    callback.object_cb = objectCallback;

    // Warnings and errors
    std::string warn;
    std::string err;

    // Start the loading process
    if (!std::filesystem::exists(path)) {
        spdlog::error("[load] > File '{}' does not exist.", path);
        return;
    }

    std::ifstream ifs(path);
    std::string pathDirectory = path.substr(0, path.find_last_of('/') + 1);
    if (pathDirectory.empty()) {
        pathDirectory = path.substr(0, path.find_last_of('\\') + 1);
    }
    if (pathDirectory.empty()) {
        pathDirectory = "./";
    }
    tinyobj::MaterialFileReader mtlReader(pathDirectory);
    LoadObjWithCallback(ifs, callback, this, &mtlReader, &warn, &err);

    // Check for warnings and errors
    if (!warn.empty()) {
        spdlog::warn(warn);
    }
    if (!err.empty()) {
        spdlog::error(err);
    }

    // Complete the last mesh
    completeCurrentMesh();
}

void OBJLoader::completeCurrentMesh() {
    if (vertices.empty() || faces.empty()) {
        spdlog::warn("[load] > No vertices or faces found for mesh '{}'.", currentMeshName);
        return;
    }
    bool hasNormals = !normals.empty();
    // bool hasTexCoords = !texCoords.empty();

    // Create a new mesh
    // Mesh mesh(currentMeshName);
    std::shared_ptr<Mesh> mesh = std::make_shared<Mesh>();

    // Step 1: Add the vertices to the mesh
    for (const auto &vertex: vertices) {
        // mesh.getVertices().emplace_back(Vertex{glm::vec3(vertex), glm::vec3(0.0f), glm::vec2(-1.0f)});
        mesh->vertices.emplace_back(Vertex{glm::vec3(vertex), glm::vec3(0.0f)});
    }

    // Step 2: Use the indices to add the normals to the mesh
    if (hasNormals) {
        for (const auto &index: indices) {
            // Add the normal to the vertex
            mesh->vertices[index.index.vertex_index].normal += normals[index.index.normal_index];
        }
    }

    // Step 3: Use the indices to add the texture coordinates to the mesh
    // if (hasTexCoords) {
    //     for (const auto &index: indices) {
    //         if (mesh.vertices[index.index.vertex_index].texCoord.x < 0.0f) {
    //             mesh.vertices[index.index.vertex_index].texCoord = texCoords[index.index.texcoord_index];
    //         } else {
    //             // The vertex already has texture coordinates. In this case, we need to add a new
    //             // vertex with the same position and normal, but with the new texture coordinates.
    //             // The corresponding face needs to be changed to use the new vertex.
    //             Vertex newVertex{
    //                     mesh.vertices[index.index.vertex_index].position,
    //                     mesh.vertices[index.index.vertex_index].normal,
    //                     texCoords[index.index.texcoord_index]
    //             };
    //             mesh.vertices.emplace_back(newVertex);
    //
    //             // Change the face to use the new vertex
    //             glm::uvec3 &faceIndices = faces[index.faceIndex];
    //             for (int i = 0; i < 3; ++i) {
    //                 if (faceIndices[i] == static_cast<unsigned int>(index.index.vertex_index)) {
    //                     faceIndices[i] = mesh.vertices.size() - 1;
    //                     break;
    //                 }
    //             }
    //         }
    //     }
    // }

    // Step 4: Add the faces to the mesh
    for (const auto &face: faces) {
        glm::uvec3 faceIndices = glm::uvec3(face.x, face.y, face.z);
        Face newFace{glm::uvec3(faceIndices.x, faceIndices.y, faceIndices.z)};
        mesh->faces.emplace_back(newFace);

        // If no normals were specified, calculate them
        if (!hasNormals) {
            // Calculate the normal of the face
            glm::vec3 v1 = mesh->vertices[face.x].position;
            glm::vec3 v2 = mesh->vertices[face.y].position;
            glm::vec3 v3 = mesh->vertices[face.z].position;
            glm::vec3 normal = glm::normalize(glm::cross(v2 - v1, v3 - v1));

            // Add the normal to the vertices
            mesh->vertices[face.x].normal += normal;
            mesh->vertices[face.y].normal += normal;
            mesh->vertices[face.z].normal += normal;
        }
    }

    // Step 5: Normalize the normals
    for (auto &vertex: mesh->vertices) {
        vertex.normal = normalize(vertex.normal);
    }

    // Add the mesh to the vector
    meshes.emplace_back(mesh);

    spdlog::info("[load] > Mesh '{}' ({} vertices, {} faces)", currentMeshName, vertices.size(), faces.size());

    // Adjust the offsets
    vertexOffset += static_cast<int>(vertices.size());
    normalOffset += static_cast<int>(normals.size());
    texCoordOffset += static_cast<int>(texCoords.size());
}

void OBJLoader::startNextMesh(const std::string &name) {
    currentMeshName = name;
    vertices.clear();
    normals.clear();
    texCoords.clear();
    indices.clear();
    faces.clear();
}

void OBJLoader::vertexCallback(void *user_data, tinyobj::real_t x, tinyobj::real_t y, tinyobj::real_t z,
                               tinyobj::real_t w) {
    auto *loader = static_cast<OBJLoader *>(user_data);
    loader->vertices.emplace_back(x, y, z, w);
}

void OBJLoader::normalCallback(void *user_data, tinyobj::real_t x, tinyobj::real_t y, tinyobj::real_t z) {
    auto *loader = static_cast<OBJLoader *>(user_data);
    loader->normals.emplace_back(x, y, z);
}

void OBJLoader::texCoordCallback(void *user_data, tinyobj::real_t x, tinyobj::real_t y, tinyobj::real_t z) {
    auto *loader = static_cast<OBJLoader *>(user_data);
    loader->texCoords.emplace_back(x, y);
}

void OBJLoader::indexCallback(void *user_data, tinyobj::index_t *indices, int num_indices) {
    auto *loader = static_cast<OBJLoader *>(user_data);

    // Triangulate the face (if necessary)
    for (int i = 2; i < num_indices; i++) {
        tinyobj::index_t idx0 = indices[0];
        tinyobj::index_t idx1 = indices[i - 1];
        tinyobj::index_t idx2 = indices[i];

        loader->faces.emplace_back(
            idx0.vertex_index - loader->vertexOffset,
            idx1.vertex_index - loader->vertexOffset,
            idx2.vertex_index - loader->vertexOffset
        );
        unsigned int faceIndex = loader->faces.size() - 1;

        loader->indices.emplace_back(index_t{
            faceIndex, {
                idx0.vertex_index - loader->vertexOffset,
                idx0.normal_index - loader->normalOffset,
                idx0.texcoord_index - loader->texCoordOffset
            }
        });
        loader->indices.emplace_back(index_t{
            faceIndex, {
                idx1.vertex_index - loader->vertexOffset,
                idx1.normal_index - loader->normalOffset,
                idx1.texcoord_index - loader->texCoordOffset
            }
        });
        loader->indices.emplace_back(index_t{
            faceIndex, {
                idx2.vertex_index - loader->vertexOffset,
                idx2.normal_index - loader->normalOffset,
                idx2.texcoord_index - loader->texCoordOffset
            }
        });
    }
}

void OBJLoader::usemtlCallback(void *user_data, const char *name, int material_index) {
    // TODO: Implement material usage?
}

void OBJLoader::mtllibCallback(void *user_data, const tinyobj::material_t *mats, int num_materials) {
    auto *loader = static_cast<OBJLoader *>(user_data);
    for (int i = 0; i < num_materials; i++) {
        loader->materials.emplace_back(Material{
            glm::vec3(mats[i].ambient[0], mats[i].ambient[1], mats[i].ambient[2]),
            glm::vec3(mats[i].diffuse[0], mats[i].diffuse[1], mats[i].diffuse[2]),
            glm::vec3(mats[i].specular[0], mats[i].specular[1], mats[i].specular[2]),
            mats[i].shininess
        });
    }
}

void OBJLoader::objectCallback(void *user_data, const char *name) {
    auto *loader = static_cast<OBJLoader *>(user_data);
    loader->completeCurrentMesh();
    loader->startNextMesh(name);
}
