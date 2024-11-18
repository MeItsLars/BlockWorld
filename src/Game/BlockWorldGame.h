#pragma once
#include <string>

#include "World/World.h"
#include "../Renderer/IWindow.h"
#include "spdlog/spdlog.h"

class GUI;

class BlockWorldGame {
public:
    explicit BlockWorldGame(IWindow *window);

    ~BlockWorldGame();

    [[nodiscard]] IWindow *getWindow() const {
        return window;
    }

    [[nodiscard]] World *getActiveWorld() const {
        return world;
    }

    [[nodiscard]] Camera *getCamera() const {
        return camera;
    }

    void run();

    bool isDebug() const {
        return debugMode;
    }

    void setDebug(bool debug);

    void createWorld();

    void loadWorld(std::string filename);

    void saveWorld(std::string filename);

    void destroyWorld();

    [[nodiscard]] float getFPS() const {
        return fps;
    }

    [[nodiscard]] size_t getVertexCount() const {
        return vertexCount;
    }

private:
    // The window used to render the game (e.g. OpenGLWindow or VulkanWindow)
    IWindow *window;
    // The UI instance
    GUI *gui;

    // The currently loaded world
    World *world{nullptr};
    // The camera instance used to render the world, from the player's perspective
    Camera *camera{nullptr};

    // Debug information
    bool debugMode = false;
    float fps = 0.0f;
    size_t vertexCount = 0;
};
