#define STB_IMAGE_IMPLEMENTATION

#include "spdlog/spdlog.h"
#include "Renderer/RendererProvider.h"
#include "Renderer/IWindow.h"
#include "Game/BlockWorldGame.h"
#include "Resources/ResourceManager.h"

int main() {
    spdlog::info("Initializing BlockWorld");
    RendererProvider::setRendererBackend(RendererBackend::OpenGL);

    std::unique_ptr<IWindow> window = RendererProvider::createWindow();
    window->initialize(1280, 720, "BlockWorld");
    window->initializeCallbacks();

    spdlog::info("Starting BlockWorld");
    auto *game = new BlockWorldGame(window.get());
    game->createWorld();
    game->run();

    delete game;
    window.reset();
    ResourceManager::releaseResources();
    spdlog::info("Exiting application");
    // TODO: Validate exit code (currently not correct, possibly due to multithreading)
}

// int main() {
//     // Create a new OpenGL Window
//     OpenGLWindow window("3D Studio Wow", 1280, 720);
//
//     // Construct a new scene
//     OBJLoader loader;
//     loader.load("../models/solarsystem/solar_system.obj");
//     auto *scene = new SolarSystemScene(loader.getModels());
//     scene->initialize();
//
//     // Create a new OpenGL renderer and initialize it with an initial scene
//     Renderer renderer;
//     renderer.initialize();
//     renderer.loadScene(scene);
//
//     // Initialize OpenGL callbacks (before ImGui init)
//     GLFWCallbackManager::initCallbacks(&window, &renderer);
//     // ImGui init
//     window.initializeImGui(&renderer);
//     // Initialize the InputManager (after ImGui init)
//     InputManager::initialize(&renderer);
//
//     // Set the renderer as the OpenGLWindow's renderer
//     window.setRenderer(&renderer);
//     glfwMaximizeWindow(window.glfwWindow);
//     window.start();
// }
