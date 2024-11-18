#pragma once
#include "../Renderer/Camera.h"

class Player {
public:
    explicit Player(Camera *cameraFP) : cameraFP(cameraFP) {
        cameraFP->setPosition(location);
        cameraFP->updateViewMatrix();
    }

    [[nodiscard]] glm::vec3 getLocation() const {
        return location;
    }

    [[nodiscard]] glm::vec3 getDirection() const {
        return direction;
    }

    void move(glm::vec3 directionNormalized);

    void rotate(float degX, float degY);

    void setSpeed(const float speed) {
        this->speed = speed;
    }

    [[nodiscard]] float getSpeed() const {
        return speed;
    }

private:
    Camera *cameraFP;

    glm::vec3 location = glm::vec3(0.0f, 0.0f, 0.0f);
    glm::vec3 direction = glm::vec3(0.0f, 0.0f, -1.0f);
    float speed = 0.1f;
};
