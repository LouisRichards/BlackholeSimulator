#include "../include/BasicRenderer.h"
#include <iostream>

void BasicRenderer::initialize() {
    // OpenGL state setup could go here
    std::cout << "Basic renderer initialized\n";
    isInitialized = true;
}

void BasicRenderer::render() {
    if (!isInitialized) {
        std::cerr << "Renderer not initialized!\n";
        return;
    }
    
    // Clear the screen with a dark blue color
    glClearColor(0.1f, 0.1f, 0.2f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);
}

void BasicRenderer::cleanup() {
    // Cleanup any renderer resources here
    isInitialized = false;
    std::cout << "Basic renderer cleaned up\n";
}
