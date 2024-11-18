#pragma once
#include <string>
#include <utility>
#include <vector>
#include <glm/glm.hpp>

#include "../UI/ObjectChecker.h"
#include "spdlog/spdlog.h"

struct Material {
    glm::vec3 ambient;
    glm::vec3 diffuse;
    glm::vec3 specular;
    float shininess;
};

struct Vertex {
    glm::vec3 position;
    glm::vec3 normal;
    glm::vec2 texCoord;
};

struct Face {
    glm::uvec3 vertexIndices;
};

class Mesh {
public:
    std::vector<Vertex> vertices;
    std::vector<Face> faces;

    Mesh() {
        ObjectChecker::addMesh();
    }

    ~Mesh() {
        ObjectChecker::removeMesh();
    }

    [[nodiscard]] size_t getVertexCount() const {
        return vertices.size();
    }

    [[nodiscard]] size_t getTotalVerticesSize() const {
        return getVertexCount() * sizeof(Vertex);
    }

    [[nodiscard]] const Vertex *getVerticesData() const {
        return vertices.data();
    }

    [[nodiscard]] size_t getFaceCount() const {
        return faces.size();
    }

    [[nodiscard]] size_t getTotalFacesSize() const {
        return getFaceCount() * sizeof(Face);
    }

    [[nodiscard]] size_t getFacesAttributeCount() const {
        return getFaceCount() * 3;
    }

    [[nodiscard]] const Face *getFacesData() const {
        return faces.data();
    }
};
