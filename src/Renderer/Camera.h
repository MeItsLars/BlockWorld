#pragma once

#include <glm/mat4x4.hpp>
#include <glm/gtc/matrix_transform.hpp>

class Camera {
public:
    // Matrices
    glm::mat4 viewMatrix = glm::mat4(1.0f);
    glm::mat4 projectionMatrix = glm::mat4(1.0f);

    // Frustum properties
    float nearPlane = 0.1f;
    float farPlane = 500.0f;

    // Perspective projection properties
    float fov = 60.0f;
    float aspectRatio = 1.0f;

    explicit Camera(const float aspectRatio) {
        reset();
        updateViewMatrix();
        setAspectRatio(aspectRatio);
    }

    void reset() {
        position = glm::vec3(0.0f, 0.0f, 2.0f);
        direction = glm::vec3(0.0f, 0.0f, -1.0f);
        up = glm::vec3(0.0f, 1.0f, 0.0f);
    }

    void setAspectRatio(float aspectRatio) {
        this->aspectRatio = aspectRatio;
        updateProjectionMatrix();
    }

    [[nodiscard]] glm::vec3 getPosition() const {
        return position;
    }

    void setPosition(glm::vec3 position);

    [[nodiscard]] glm::vec3 getDirection() const {
        return direction;
    }

    void setDirection(glm::vec3 direction);

    [[nodiscard]] glm::vec3 getUp() const {
        return up;
    }

    void setUp(glm::vec3 up);

    void rotateX(float angle);

    void rotateY(float angle);

    void updateViewMatrix();

    void updateProjectionMatrix();

private:
    // Camera properties
    glm::vec3 position{};
    glm::vec3 direction{};
    glm::vec3 up{};
};
