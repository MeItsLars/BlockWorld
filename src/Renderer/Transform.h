#pragma once
#include <glm/glm.hpp>

class Transform {
public:
    Transform() = default;

    ~Transform() = default;

    [[nodiscard]] glm::mat4 getModelMatrix() const {
        return modelMatrix;
    }

    [[nodiscard]] glm::mat3 getNormalMatrix() const {
        return normalMatrix;
    }

    void updateNormalMatrix() {
        normalMatrix = glm::mat3(transpose(inverse(modelMatrix)));
    }

    void translate(float dx, float dy, float dz);

    void rotateX(float angle, glm::vec3 around = glm::vec3(0.0f));

    void rotateY(float angle, glm::vec3 around = glm::vec3(0.0f));

    void rotateZ(float angle, glm::vec3 around = glm::vec3(0.0f));

    void rotate(float angle, glm::vec3 axis, glm::vec3 around = glm::vec3(0.0f));

    void scale(float sx, float sy, float sz, glm::vec3 around = glm::vec3(0.0f));

private:
    glm::mat4 modelMatrix = glm::mat4(1.0f);
    glm::mat3 normalMatrix = glm::mat3(1.0f);
};
