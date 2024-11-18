#include "Player.h"

#include <glm/gtc/quaternion.hpp>

void Player::move(glm::vec3 directionNormalized) {
    location += directionNormalized * speed;
    cameraFP->setPosition(location);
    cameraFP->updateViewMatrix();
}

void Player::rotate(float degX, float degY) {
    // Calculate the up and right vector
    glm::vec3 up = {0.0f, 1.0f, 0.0f};
    glm::vec3 right = normalize(glm::cross(direction, up));

    // Apply the X-axis rotation (vertical) around the 'right' vector
    glm::quat rotationX = angleAxis(degX, up);
    glm::vec3 rotatedDirection = rotationX * direction;

    // Apply the Y-axis rotation (horizontal) around the global up vector
    glm::quat rotationY = angleAxis(degY, right);
    rotatedDirection = rotationY * rotatedDirection;

    // Normalize and check for limits to avoid gimbal lock
    rotatedDirection = normalize(rotatedDirection);
    if (rotatedDirection.y > 0.99f || rotatedDirection.y < -0.99f) {
        return;
    }

    // Update direction and camera
    direction = rotatedDirection;
    cameraFP->setDirection(direction);
    cameraFP->updateViewMatrix();
}


// void Player::rotate(float degX, float degY) {
//     glm::vec3 right = glm::cross(direction, DIRECTION_UP);
//     glm::mat3 rotationMatrixX = glm::rotate(glm::mat4(1.0f), degX, DIRECTION_UP);
//     glm::mat3 rotationMatrixY = glm::rotate(glm::mat4(1.0f), degY, right);
//     glm::vec3 newDirection = glm::normalize(rotationMatrixY * rotationMatrixX * direction);
//     if (newDirection.y > 0.99f || newDirection.y < -0.99f) {
//         return;
//     }
//     direction = newDirection;
//     cameraFP->setDirection(direction);
//     cameraFP->updateViewMatrix();
// }
