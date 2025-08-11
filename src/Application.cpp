#include "../include/Application.h"
#include <iostream>

Application::Application(std::unique_ptr<IWindow> win, std::unique_ptr<IRenderer> rend)
    : window(std::move(win)), renderer(std::move(rend)), isInitialized(false) {}

bool Application::initialize() {
    if (isInitialized) {
        return true;
    }

    if (!window || !window->initialize()) {
        std::cerr << "Failed to initialize window\n";
        return false;
    }

    if (!renderer) {
        std::cerr << "No renderer provided\n";
        return false;
    }

    renderer->initialize();
    isInitialized = true;
    
    std::cout << "Application initialized successfully\n";
    return true;
}

void Application::run() {
    if (!isInitialized) {
        std::cerr << "Application not initialized. Call initialize() first.\n";
        return;
    }

    std::cout << "Starting main loop\n";
    
    // Main application loop
    while (!window->shouldClose()) {
        // Render
        renderer->render();
        
        // Swap buffers and poll events
        window->swapBuffers();
        window->pollEvents();
    }
    
    std::cout << "Main loop ended\n";
}

void Application::cleanup() {
    if (renderer) {
        renderer->cleanup();
    }
    
    if (window) {
        window->cleanup();
    }
    
    isInitialized = false;
    std::cout << "Application cleaned up\n";
}
