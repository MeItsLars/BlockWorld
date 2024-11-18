#define STB_IMAGE_IMPLEMENTATION

#include <iostream>

#include "spdlog/spdlog.h"
#include "Renderer/RendererProvider.h"
#include "Renderer/IWindow.h"
#include "Game/BlockWorldGame.h"
#include "Resources/ResourceManager.h"

int main() {
    spdlog::info("Initializing BlockWorld");
    RendererProvider::setRendererBackend(RendererBackend::OPEN_GL);

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
    return 0;
}