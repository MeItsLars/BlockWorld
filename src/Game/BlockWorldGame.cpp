#include "BlockWorldGame.h"

#include "TaskScheduler.h"
#include "../UI/GUI.h"
#include "../Input/InputManager.h"
#include "../Renderer/Empty/EWindow.h"

BlockWorldGame::BlockWorldGame(IWindow *window) : window(window), gui(nullptr) {
    // Initialize class members
    gui = new GUI(this);
    camera = new Camera(window->getAspectRatio());

    // Initialize the input manager
    InputManager::initialize(this);
    setDebug(false);

    // Log hardware threads
    unsigned int hardwareThreads = std::thread::hardware_concurrency();
    if (hardwareThreads == 0) {
        spdlog::warn("Could not detect number of hardware threads");
    } else {
        spdlog::info("Suggested number of threads: {}", hardwareThreads);
    }

    // Initialize task schedulers
    TaskSchedulers::WORLD_GEN.initialize("WorldGen", 4);
    TaskSchedulers::MESH_GEN.initialize("MeshGen", 4);
}

BlockWorldGame::~BlockWorldGame() {
    spdlog::info("Terminating BlockWorld instance");

    // Stop the task schedulers
    TaskSchedulers::WORLD_GEN.stop();
    TaskSchedulers::MESH_GEN.stop();

    // Destroy the world
    destroyWorld();

    // Delete GUI and camera
    delete camera;
    delete gui;
}

void BlockWorldGame::run() {
    spdlog::info("Done! Running BlockWorld...");

    auto lastTime = std::chrono::high_resolution_clock::now();
    int frameCount = 0;

    while (!window->shouldClose()) {
        // Calculate FPS
        auto currentTime = std::chrono::high_resolution_clock::now();
        std::chrono::duration<float> deltaTime = currentTime - lastTime;
        frameCount++;
        if (deltaTime.count() >= 1.0f) {
            fps = static_cast<float>(frameCount) / deltaTime.count();
            frameCount = 0;
            lastTime = currentTime;
        }

        // In case of EWindow, allow terminal input
        if (auto *eWindow = dynamic_cast<EWindow*>(window)) {
            eWindow->interactWithTerminal(world);
        }

        // Poll events
        window->pollEvents();

        // Handle input
        InputManager::checkConstantInput();

        // Update the world
        if (world != nullptr) {
            // TODO: world->update();
        }

        // Clear the screen
        window->clear(0.2f, 0.2f, 0.2f, 1.0f);

        // Render the world
        if (world != nullptr) {
            vertexCount = world->render(camera);
        }

        // Render the UI
        if (debugMode) {
            // Draw the imgui GUI if debug mode is enabled
            gui->drawGUI();
        }

        // Swap buffers
        window->swapBuffers();
    }
}

void BlockWorldGame::setDebug(bool debug) {
    this->debugMode = debug;
    window->lockCursor(!debug);
}

void BlockWorldGame::createWorld() {
    spdlog::info("Generating new world...");

    // If a different world was loaded, destroy it
    destroyWorld();

    // Create a new world
    // TODO: Remove camera from world constructor?
    world = new World(camera);

    // const int area = 10;
    // for (int cx = -area; cx <= area; cx++) {
    //     for (int cz = -area; cz <= area; cz++) {
    //         world->generateChunk(cx, cz);
    //     }
    // }

    // Add a single sample entity
    // world->spawnEntity({0, -3, 0});
}

void BlockWorldGame::loadWorld(std::string filename) {
    // Not implemented
}

void BlockWorldGame::saveWorld(std::string filename) {
    // Not implemented
}

void BlockWorldGame::destroyWorld() {
    if (world != nullptr) {
        delete world;
        world = nullptr;
    }
}
