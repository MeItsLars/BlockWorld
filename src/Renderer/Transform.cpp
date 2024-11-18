#include "Transform.h"

#include <glm/ext/matrix_transform.hpp>

void Transform::translate(float dx, float dy, float dz) {
    glm::mat4 translationMatrix = glm::translate(glm::mat4(1.0f), glm::vec3(dx, dy, dz));
    modelMatrix = translationMatrix * modelMatrix;
    updateNormalMatrix();
}

void Transform::scale(float sx, float sy, float sz, glm::vec3 around) {
    translate(-around.x, -around.y, -around.z);
    glm::mat4 scaleMatrix = glm::scale(glm::mat4(1.0f), glm::vec3(sx, sy, sz));
    modelMatrix = scaleMatrix * modelMatrix;
    translate(around.x, around.y, around.z);
    updateNormalMatrix();
}

void Transform::rotateX(float angleDegrees, glm::vec3 around) {
    translate(-around.x, -around.y, -around.z);
    float angleRad = glm::radians(angleDegrees);
    glm::mat4 rotationMatrix = glm::rotate(glm::mat4(1.0f), angleRad, glm::vec3(1.0f, 0.0f, 0.0f));
    modelMatrix = rotationMatrix * modelMatrix;
    translate(around.x, around.y, around.z);
    updateNormalMatrix();
}

void Transform::rotateY(float angleDegrees, glm::vec3 around) {
    translate(-around.x, -around.y, -around.z);
    float angleRad = glm::radians(angleDegrees);
    glm::mat4 rotationMatrix = glm::rotate(glm::mat4(1.0f), angleRad, glm::vec3(0.0f, 1.0f, 0.0f));
    modelMatrix = rotationMatrix * modelMatrix;
    translate(around.x, around.y, around.z);
    updateNormalMatrix();
}

void Transform::rotateZ(float angleDegrees, glm::vec3 around) {
    translate(-around.x, -around.y, -around.z);
    float angleRad = glm::radians(angleDegrees);
    glm::mat4 rotationMatrix = glm::rotate(glm::mat4(1.0f), angleRad, glm::vec3(0.0f, 0.0f, 1.0f));
    modelMatrix = rotationMatrix * modelMatrix;
    translate(around.x, around.y, around.z);
    updateNormalMatrix();
}

void Transform::rotate(float angleDegrees, glm::vec3 axis, glm::vec3 around) {
    translate(-around.x, -around.y, -around.z);
    float angleRad = glm::radians(angleDegrees);
    glm::mat4 rotationMatrix = glm::rotate(glm::mat4(1.0f), angleRad, axis);
    modelMatrix = rotationMatrix * modelMatrix;
    translate(around.x, around.y, around.z);
    updateNormalMatrix();
}
