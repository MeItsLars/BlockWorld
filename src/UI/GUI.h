#pragma once

#include <imgui.h>
#include <ImGuiFileDialog.h>

class BlockWorldGame;

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
