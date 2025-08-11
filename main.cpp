#include "include/Application.h"
#include "include/GLFWWindow.h"
#include "include/BasicRenderer.h"
#include "include/WindowProperties.h"
#include <memory>
#include <iostream>

int main() {
    try {
        // Create window properties
        WindowProperties props(800, 600, "Blackhole Simulator");
        
        // Create window and renderer instances (Dependency Injection)
        auto window = std::make_unique<GLFWWindow>(props);
        auto renderer = std::make_unique<BasicRenderer>();
        
        // Create application with injected dependencies
        Application app(std::move(window), std::move(renderer));
        
        // Initialize and run
        if (!app.initialize()) {
            std::cerr << "Failed to initialize application\n";
            return -1;
        }
        
        app.run();
        
        // Cleanup is handled automatically by destructors
        std::cout << "Application finished successfully\n";
        return 0;
        
    } catch (const std::exception& e) {
        std::cerr << "Exception occurred: " << e.what() << std::endl;
        return -1;
    }
}