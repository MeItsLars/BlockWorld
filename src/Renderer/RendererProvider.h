#pragma once
#include <memory>

#include "IRenderable.h"
#include "IWindow.h"
#include "OpenGL/GLRenderable.h"
#include "OpenGL/GLShader.h"
#include "OpenGL/GLWindow.h"
#include "Empty/ERenderable.h"
#include "Empty/EShader.h"
#include "Empty/EWindow.h"
#include "spdlog/spdlog.h"

enum class RendererBackend {
    NONE,
    OPEN_GL,
    // VULKAN,
    // DIRECT_X,
};

class RendererProvider {
public:
    static RendererBackend getRendererBackend() {
        return renderBackend;
    }

    static void setRendererBackend(const RendererBackend backend) {
        renderBackend = backend;
        switch (backend) {
            case RendererBackend::NONE:
                spdlog::info("Renderer backend set to None");
                break;
            case RendererBackend::OPEN_GL:
                spdlog::info("Renderer backend set to OpenGL");
                break;
            default:
                spdlog::error("Invalid renderer backend");
                exit(-1);
        }
    }

    static std::unique_ptr<IWindow> createWindow() {
        switch (renderBackend) {
            case RendererBackend::NONE:
                return std::make_unique<EWindow>();
            case RendererBackend::OPEN_GL:
                return std::make_unique<GLWindow>();
            default:
                spdlog::error("Invalid renderer backend when creating window");
            exit(-1);
        }
    }

    static std::shared_ptr<IShader> createShader() {
        switch (renderBackend) {
            case RendererBackend::NONE:
                return std::make_shared<EShader>();
            case RendererBackend::OPEN_GL:
                return std::make_shared<GLShader>();
            default:
                spdlog::error("Invalid renderer backend when creating shader");
                exit(-1);
        }
    }

    static std::unique_ptr<IRenderable> createRenderable() {
        switch (renderBackend) {
            case RendererBackend::NONE:
                return std::make_unique<ERenderable>();
            case RendererBackend::OPEN_GL:
                return std::make_unique<GLRenderable>();
            default:
                spdlog::error("Invalid renderer backend when creating renderable");
                exit(-1);
        }
    }

private:
    static RendererBackend renderBackend;
};
