#include "GL/glew.h"
#include "GLWindow.h"
#include "spdlog/spdlog.h"

#include <iostream>

#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"
#include "../../Input/InputManager.h"

bool GLWindow::initialize(int width, int height, const char *title) {
    spdlog::info("Initializing OpenGL window");
    this->width = width;
    this->height = height;

    // Initialize glfw
    if (!glfwInit()) {
        exit(EXIT_FAILURE);
    }

    // Set minimum supported OpenGL version and OpenGL profile
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);

    // Create maximized OpenGL window
    glfwWindow = glfwCreateWindow(width, height, title, nullptr, nullptr);
    if (glfwWindow == nullptr) {
        glfwTerminate();
        spdlog::error("Could not open window or initialize OpenGL context.");
        exit(EXIT_FAILURE);
    }
    glfwSetWindowUserPointer(glfwWindow, this);

    glfwMakeContextCurrent(glfwWindow);
    glfwSwapInterval(1);

    // Initialize glew
    glewExperimental = GL_TRUE;
    GLenum glewError = glewInit();
    if (glewError != GLEW_OK) {
        spdlog::error("Error initializing GLEW: {}", reinterpret_cast<const char *>(glewGetErrorString(glewError)));

        exit(EXIT_FAILURE);
    }

    if (!GLEW_VERSION_4_3) {
        spdlog::warn("Warning: OpenGL 4.3+ not supported by the GPU!");
    }

    glEnable(GL_DEPTH_TEST);
    glEnable(GL_CULL_FACE);
    glCullFace(GL_BACK);
    glFrontFace(GL_CCW);

    spdlog::info("OpenGL loaded. Version: {}", reinterpret_cast<const char *>(glGetString(GL_VERSION)));

    // Set graphics attributes
    glPointSize(5.0); // Unsupported in OpenGL ES 2.0
    glLineWidth(1.0);
    setVSync(true);
    return true;
}

void GLWindow::initializeCallbacks() {
    spdlog::info("Initializing GLFW callbacks");
    glfwSetErrorCallback(errorCallback);
    glfwSetFramebufferSizeCallback(glfwWindow, [](GLFWwindow *window, int width, int height) {
        auto instance = static_cast<GLWindow *>(glfwGetWindowUserPointer(window));
        instance->reshape(width, height);
    });
    glfwSetKeyCallback(glfwWindow, InputManager::keyCallback);
    glfwSetScrollCallback(glfwWindow, InputManager::scrollCallback);
    glfwSetMouseButtonCallback(glfwWindow, InputManager::mouseButtonCallback);
    glfwSetCursorPosCallback(glfwWindow, InputManager::cursorPosCallback);
}

void GLWindow::errorCallback(int error, const char *description) {
    spdlog::error("GLFW Error {}: {}", error, description);
}

GLWindow::~GLWindow() {
    spdlog::info("Destroying OpenGL window");
    glfwDestroyWindow(glfwWindow);
    glfwTerminate();
}

void GLWindow::reshape(int width, int height) {
    this->width = width;
    this->height = height;
    glViewport(0, 0, width, height);
    InputManager::resizeCallback(glfwWindow, width, height);
}

float GLWindow::getAspectRatio() {
    return static_cast<float>(width) / static_cast<float>(height);
}

bool GLWindow::isVSync() {
    return vsync;
}

bool GLWindow::setVSync(bool enable) {
    vsync = enable;
    glfwSwapInterval(enable ? 1 : 0);
    return enable;
}

void GLWindow::lockCursor(bool lock) {
    if (lock) {
        glfwSetInputMode(glfwWindow, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
    } else {
        glfwSetInputMode(glfwWindow, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
    }
}

void GLWindow::clear(const float r, const float g, const float b, const float a) {
    glClearColor(r, g, b, a);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

void GLWindow::swapBuffers() {
    glfwSwapBuffers(glfwWindow);
}

bool GLWindow::shouldClose() {
    return glfwWindowShouldClose(glfwWindow);
}

void GLWindow::pollEvents() {
    glfwPollEvents();
}

void GLWindow::initUI() {
    // Setup GLFW and OpenGL bindings
    ImGui_ImplGlfw_InitForOpenGL(glfwWindow, true);
    ImGui_ImplOpenGL3_Init(nullptr);
}

void GLWindow::beginUIFrame() {
    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplGlfw_NewFrame();
}

void GLWindow::endUIFrame() {
    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
}

void GLWindow::destroyUI() {
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
}

void GLWindow::close() {
    glfwSetWindowShouldClose(glfwWindow, GLFW_TRUE);
}
