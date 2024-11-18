#include "Camera.h"

void Camera::setPosition(const glm::vec3 position) {
    this->position = position;
}

void Camera::setDirection(const glm::vec3 direction) {
    this->direction = direction;
}

void Camera::setUp(const glm::vec3 up) {
    this->up = up;
}

void Camera::rotateX(const float angle) {
    const glm::mat3 rotationMatrix = rotate(glm::mat4(1.0f), angle, up);
    const glm::vec3 newDirection = normalize(rotationMatrix * direction);
    if (newDirection.y > 0.99f || newDirection.y < -0.99f) {
        return;
    }
    direction = newDirection;
}

void Camera::rotateY(const float angle) {
    const glm::vec3 right = cross(direction, up);
    const glm::mat3 rotationMatrix = rotate(glm::mat4(1.0f), angle, right);
    const glm::vec3 newDirection = normalize(rotationMatrix * direction);
    if (newDirection.y > 0.99f || newDirection.y < -0.99f) {
        return;
    }
    direction = newDirection;
}

void Camera::updateViewMatrix() {
    viewMatrix = lookAt(position, position + direction, up);
}

void Camera::updateProjectionMatrix() {
    projectionMatrix = glm::perspective(glm::radians(fov), aspectRatio, nearPlane, farPlane);
}
