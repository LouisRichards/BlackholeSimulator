#include "include/Application.h"
#include "include/GLFWWindow.h"
#include "include/GravityRenderer.h"
#include "include/GravitySimulation.h"
#include "include/WindowProperties.h"
#include <memory>
#include <iostream>
#include <chrono>
#include <thread>
#include <GLFW/glfw3.h>

/**
 * @brief Custom application that integrates gravity simulation
 */
class GravityApplication : public Application {
public:
    GravityApplication(std::unique_ptr<IWindow> window, std::unique_ptr<GravityRenderer> renderer)
        : Application(std::move(window), std::move(renderer)) {
        gravityRenderer = static_cast<GravityRenderer*>(this->renderer.get());
        gravitySimulation = std::make_unique<GravitySimulation>(800.0f, 600.0f, 25);
    }

    bool initialize() override {
        if (!Application::initialize()) {
            return false;
        }
        
        gravitySimulation->initialize();
        setupSimulationRenderer();
        return true;
    }

    void run() override {
        if (!isInitialized) {
            std::cerr << "Application not initialized. Call initialize() first.\n";
            return;
        }

        std::cout << "Starting main loop (Press ESC to exit)\n";
        
        float deltaTime = 0.016f; // ~60 FPS
        
        // Main application loop with simulation updates
        while (!window->shouldClose()) {
            // Check for ESC key to exit
            if (glfwGetKey(static_cast<GLFWwindow*>(window->getNativeWindow()), GLFW_KEY_ESCAPE) == GLFW_PRESS) {
                glfwSetWindowShouldClose(static_cast<GLFWwindow*>(window->getNativeWindow()), GLFW_TRUE);
            }
            
            // Update simulation
            gravitySimulation->update(deltaTime);
            
            // Render
            renderer->render();
            
            // Swap buffers and poll events
            window->swapBuffers();
            window->pollEvents();
            
            // Small delay to control frame rate
            std::this_thread::sleep_for(std::chrono::milliseconds(16));
        }
        
        std::cout << "Main loop ended\n";
    }

private:
    std::unique_ptr<GravitySimulation> gravitySimulation;
    GravityRenderer* gravityRenderer; // Non-owning pointer

    void setupSimulationRenderer() {
        auto gravityRendererShared = std::shared_ptr<GravityRenderer>(
            gravityRenderer, [](GravityRenderer*){} // Empty deleter since we don't own it
        );
        gravitySimulation->setupRenderer(gravityRendererShared);
    }
};

int main() {
    try {
        // Create window properties for the gravity simulation
        WindowProperties props(800, 600, "Blackhole Simulator - Gravity Grid");
        
        // Create window instance
        auto window = std::make_unique<GLFWWindow>(props);
        
        // Create gravity renderer with viewport dimensions
        auto gravityRenderer = std::make_unique<GravityRenderer>(800.0f, 600.0f);
        
        // Create gravity application with injected dependencies
        GravityApplication app(std::move(window), std::move(gravityRenderer));
        
        // Initialize and run
        if (!app.initialize()) {
            std::cerr << "Failed to initialize application\n";
            return -1;
        }
        
        std::cout << "Starting gravity simulation...\n";
        std::cout << "The grid shows gravitational field strength:\n";
        std::cout << "  - Blue areas: Low gravitational force\n";
        std::cout << "  - Red areas: High gravitational force\n";
        std::cout << "  - Yellow circles: Gravitational bodies\n";
        
        app.run();
        
        // Cleanup is handled automatically by destructors
        std::cout << "Gravity simulation finished successfully\n";
        return 0;
        
    } catch (const std::exception& e) {
        std::cerr << "Exception occurred: " << e.what() << std::endl;
        return -1;
    }
}