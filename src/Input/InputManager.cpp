#include "InputManager.h"

BlockWorldGame *InputManager::game = nullptr;
ImGuiIO *InputManager::io = nullptr;

double InputManager::lastX = 0;
double InputManager::lastY = 0;

std::unordered_set<int> InputManager::pressedKeys;
