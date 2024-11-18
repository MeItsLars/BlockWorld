#pragma once

class IWindow {
public:
    virtual ~IWindow() = default;

    virtual bool initialize(int width, int height, const char *title) = 0;

    virtual void initializeCallbacks() = 0;

    virtual void reshape(int width, int height) = 0;

    virtual float getAspectRatio() = 0;

    virtual bool isVSync() = 0;

    virtual bool setVSync(bool enable) = 0;

    virtual void lockCursor(bool lock) = 0;

    virtual void clear(float r, float g, float b, float a) = 0;

    virtual void swapBuffers() = 0;

    virtual bool shouldClose() = 0;

    virtual void pollEvents() = 0;

    virtual void initUI() = 0;

    virtual void beginUIFrame() = 0;

    virtual void endUIFrame() = 0;

    virtual void destroyUI() = 0;

    virtual void close() = 0;
};
