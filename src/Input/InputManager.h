#pragma once

#include <unordered_set>
#include <imgui.h>

#include "../Game/BlockWorldGame.h"
#include "GLFW/glfw3.h"
#include "spdlog/spdlog.h"

#define DIRECTION_UP {0.0f, 1.0f, 0.0f}
#define DIRECTION_DOWN {0.0f, -1.0f, 0.0f}

class InputManager {
public:
    static void checkConstantInput() {
        if (!game || io->WantCaptureKeyboard)
            return;

        Player *player = game->getActiveWorld()->getPlayer();

        if (pressedKeys.count(GLFW_KEY_SPACE)) player->move(DIRECTION_UP);
        if (pressedKeys.count(GLFW_KEY_LEFT_SHIFT)) player->move(DIRECTION_DOWN);
        if (pressedKeys.count(GLFW_KEY_D)) player->move(cross(player->getDirection(), DIRECTION_UP));
        if (pressedKeys.count(GLFW_KEY_A)) player->move(cross(DIRECTION_UP, player->getDirection()));
        if (pressedKeys.count(GLFW_KEY_W)) player->move(player->getDirection());
        if (pressedKeys.count(GLFW_KEY_S)) player->move(-player->getDirection());
    }

    static void keyCallback(GLFWwindow *window, int key, int scancode, int action, int mods) {
        if (!game || io->WantCaptureKeyboard)
            return;

        // Store pressed keys, used for constant input
        if (action == GLFW_PRESS) {
            pressedKeys.insert(key);
        } else if (action == GLFW_RELEASE) {
            pressedKeys.erase(key);
        }

        if (action != GLFW_PRESS)
            return;

        // Handle single key presses
        switch (key) {
            case GLFW_KEY_ESCAPE:
                if (game->isDebug()) {
                    game->setDebug(false);
                } else {
                    glfwSetWindowShouldClose(window, GL_TRUE);
                }
                break;
            case GLFW_KEY_T:
                game->setDebug(!game->isDebug());
                break;
            default:
                break;
        }
    }

    static void scrollCallback(GLFWwindow *window, double xOffset, double yOffset) {
        if (!game || io->WantCaptureMouse)
            return;

        // TODO: ?
        // game->getCamera()->position += game->getCamera()->direction * static_cast<float>(yOffset) * 0.3f;
        // game->getCamera()->updateViewMatrix();

        float speed = game->getActiveWorld()->getPlayer()->getSpeed();
        if (yOffset > 0) {
            game->getActiveWorld()->getPlayer()->setSpeed(speed + 0.1f);
        } else if (yOffset < 0 && speed > 0.1f) {
            game->getActiveWorld()->getPlayer()->setSpeed(speed - 0.1f);
        } else if (yOffset < 0) {
            game->getActiveWorld()->getPlayer()->setSpeed(0.0f);
        }
    }

    static void mouseButtonCallback(GLFWwindow *window, int button, int action, int mods) {
        if (!game || io->WantCaptureMouse)
            return;

        if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_PRESS) {
            glfwGetCursorPos(window, &lastX, &lastY);
        }
    }

    static void cursorPosCallback(GLFWwindow *window, double xPos, double yPos) {
        if (!game || io->WantCaptureMouse)
            return;

        if (!game->isDebug() || glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT) == GLFW_PRESS) {
            float deltaX = xPos - lastX;
            float deltaY = yPos - lastY;

            // Apply sensitivity
            deltaX *= 0.3f;
            deltaY *= 0.3f;

            // Rotate player
            game->getActiveWorld()->getPlayer()->rotate(-deltaX / 100.0f, -deltaY / 100.0f);

            // Update last position
            lastX = xPos;
            lastY = yPos;
        }
    }

    static void resizeCallback(GLFWwindow *window, int width, int height) {
        if (!game)
            return;

        game->getCamera()->setAspectRatio(static_cast<float>(width) / static_cast<float>(height));
    }

    static void initialize(BlockWorldGame *game) {
        io = &ImGui::GetIO();
        InputManager::game = game;
    }

private:
    static ImGuiIO *io;
    static BlockWorldGame *game;

    static double lastX;
    static double lastY;

    static std::unordered_set<int> pressedKeys;
};
