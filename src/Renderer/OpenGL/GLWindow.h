#pragma once
#include "GLFW/glfw3.h"
#include "../IWindow.h"

class GLWindow : public IWindow {
public:
    ~GLWindow() override;

    bool initialize(int width, int height, const char *title) override;

    void initializeCallbacks() override;

    void reshape(int width, int height) override;

    float getAspectRatio() override;

    bool isVSync() override;

    bool setVSync(bool enable) override;

    void lockCursor(bool lock) override;

    void clear(float r, float g, float b, float a) override;

    void swapBuffers() override;

    bool shouldClose() override;

    void pollEvents() override;

    void initUI() override;

    void beginUIFrame() override;

    void endUIFrame() override;

    void destroyUI() override;

    void close() override;
private:
    static void errorCallback(int error, const char *description);

    GLFWwindow *glfwWindow = nullptr;
    int width = 0;
    int height = 0;
    bool vsync = true;
};
