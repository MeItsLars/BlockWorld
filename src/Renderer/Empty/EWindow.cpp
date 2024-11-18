#include "EWindow.h"

#include <iostream>

#include "../../Game/World/World.h"
#include "spdlog/spdlog.h"
#include "../../UI/GUI.h"

void EWindow::interactWithTerminal(World *world) {
    size_t memory = getUsedMemory();
    size_t memoryMB = memory / 1024 / 1024;
    std::cout << "Terminal-only instance: render frame " << frame << " Memory used: " << memoryMB << "MB. Actions:" << std::endl;
    std::cout << "[0]: Continue to next frame" << std::endl;
    std::cout << "[1] [RD]: Set render distance" << std::endl;
    std::cout << "[2]: Quit" << std::endl;

    int choice;
    std::cin >> choice;

    if (choice == 1) {
        int newRenderDistance;
        std::cin >> newRenderDistance;
        world->setRenderDistance(newRenderDistance);
    } else if (choice == 2) {
        close();
    }

    frame++;
}


bool EWindow::initialize(int width, int height, const char *title) {
    return true;
}

void EWindow::initializeCallbacks() {
    spdlog::info("Ignoring empty window callbacks");
}

EWindow::~EWindow() {
    spdlog::info("Destroying EWindow");
}

void EWindow::reshape(int width, int height) {
}

float EWindow::getAspectRatio() {
    return 0.0f;
}

bool EWindow::isVSync() {
    return false;
}

bool EWindow::setVSync(bool enable) {
    return false;
}

void EWindow::lockCursor(bool lock) {
}

void EWindow::clear(const float r, const float g, const float b, const float a) {
}

void EWindow::swapBuffers() {
}

bool EWindow::shouldClose() {
    return m_close;
}

void EWindow::pollEvents() {
}

void EWindow::initUI() {
}

void EWindow::beginUIFrame() {
}

void EWindow::endUIFrame() {
}

void EWindow::destroyUI() {
}

void EWindow::close() {
    spdlog::info("Closing EWindow");
    m_close = true;
}
