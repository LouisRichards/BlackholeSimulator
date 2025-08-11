#include "../include/GLFWWindow.h"
#include <iostream>

GLFWWindow::GLFWWindow(const WindowProperties& props)
    : properties(props), window(nullptr), isInitialized(false) {}

GLFWWindow::~GLFWWindow() {
    cleanup();
}

bool GLFWWindow::initialize() {
    if (isInitialized) {
        return true;
    }

    if (!initializeGLFW()) {
        return false;
    }

    if (!createWindow()) {
        cleanup();
        return false;
    }

    if (!initializeGLEW()) {
        cleanup();
        return false;
    }

    isInitialized = true;
    return true;
}

bool GLFWWindow::initializeGLFW() {
    if (!glfwInit()) {
        std::cerr << "Failed to initialize GLFW\n";
        return false;
    }

    // Set OpenGL version and profile
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    return true;
}

bool GLFWWindow::createWindow() {
    window = glfwCreateWindow(properties.width, properties.height, properties.title, nullptr, nullptr);
    if (!window) {
        std::cerr << "Failed to create GLFW window\n";
        return false;
    }

    glfwMakeContextCurrent(window);
    return true;
}

bool GLFWWindow::initializeGLEW() {
    if (glewInit() != GLEW_OK) {
        std::cerr << "Failed to initialize GLEW\n";
        return false;
    }
    return true;
}

void GLFWWindow::cleanup() {
    if (window) {
        glfwDestroyWindow(window);
        window = nullptr;
    }
    
    if (isInitialized) {
        glfwTerminate();
        isInitialized = false;
    }
}

bool GLFWWindow::shouldClose() const {
    return window ? glfwWindowShouldClose(window) : true;
}

void GLFWWindow::swapBuffers() {
    if (window) {
        glfwSwapBuffers(window);
    }
}

void GLFWWindow::pollEvents() {
    glfwPollEvents();
}
