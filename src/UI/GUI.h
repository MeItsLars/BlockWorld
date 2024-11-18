#pragma once

#include <imgui.h>
#include <ImGuiFileDialog.h>

class BlockWorldGame;

#ifdef _WIN32
#include <windows.h>
#include <psapi.h>
#include <iostream>

static size_t getUsedMemory() {
    PROCESS_MEMORY_COUNTERS pmc;
    if (GetProcessMemoryInfo(GetCurrentProcess(), &pmc, sizeof(pmc))) {
        return pmc.WorkingSetSize;
    }
    return 0;
}

#else
#include <fstream>
#include <string>

static size_t getUsedMemory() {
    std::ifstream statusFile("/proc/self/status");
    if (!statusFile.is_open()) {
        return 0;
    }

    std::string line;
    size_t memory = 0;
    while (std::getline(statusFile, line)) {
        if (line.find("VmRSS") != std::string::npos) {
            size_t value = 0;
            // Extract the memory value (in kilobytes)
            sscanf(line.c_str(), "VmRSS: %zu kB", &value);
            memory = value * 1024;  // Convert to bytes
            break;
        }
    }

    statusFile.close();
    return memory;
}
#endif

class GUI {
public:
    explicit GUI(BlockWorldGame *game);

    ~GUI();

    void setStyle(ImGuiIO &io);

    void drawGUI();

private:
    BlockWorldGame *game;
    ImFont *font;
    ImGuiFileDialog fileDialog;
};
