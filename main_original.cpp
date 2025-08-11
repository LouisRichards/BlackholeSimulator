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
        std::cout << "Camera Controls:\n";
        std::cout << "  - M: Open camera mode menu\n";
        std::cout << "  - Mouse: Hold left button and drag to rotate camera\n";
        std::cout << "  - W/S: Move forward/backward\n";
        std::cout << "  - A/D: Move left/right\n";
        std::cout << "  - Q/E: Move up/down (Free-flight) or adjust eye height (Game-style)\n";
        std::cout << "  - Shift+W/S: Zoom in/out (Free-flight mode only)\n";
        std::cout << "Current Mode: Free-flight\n";
        
        float deltaTime = 0.016f; // ~60 FPS
        
        // Variables for mouse tracking
        double lastMouseX = 0.0, lastMouseY = 0.0;
        bool firstMouse = true;
        GLFWwindow* glfwWindow = static_cast<GLFWwindow*>(window->getNativeWindow());
        
        // Main application loop with simulation updates
        while (!window->shouldClose()) {
            // Check for ESC key to exit
            if (glfwGetKey(glfwWindow, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
                glfwSetWindowShouldClose(glfwWindow, GLFW_TRUE);
            }
            
            // Handle camera input
            handleCameraInput(glfwWindow, lastMouseX, lastMouseY, firstMouse, deltaTime);
            
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

    void handleCameraInput(GLFWwindow* glfwWindow, double& lastMouseX, double& lastMouseY, bool& firstMouse, float deltaTime) {
        // Check for menu toggle (M key)
        static bool mPressed = false;
        bool mCurrentlyPressed = glfwGetKey(glfwWindow, GLFW_KEY_M) == GLFW_PRESS;
        
        if (mCurrentlyPressed && !mPressed) {
            gravityRenderer->toggleMenu();
        }
        mPressed = mCurrentlyPressed;
        
        // Handle mouse input
        double mouseX, mouseY;
        glfwGetCursorPos(glfwWindow, &mouseX, &mouseY);
        
        // Check for mouse clicks
        static bool mousePressed = false;
        bool mouseCurrentlyPressed = glfwGetMouseButton(glfwWindow, GLFW_MOUSE_BUTTON_LEFT) == GLFW_PRESS;
        bool mouseClicked = mouseCurrentlyPressed && !mousePressed;
        mousePressed = mouseCurrentlyPressed;
        
        // Handle menu interaction
        gravityRenderer->handleMenu(mouseX, mouseY, mouseClicked);
        
        // Only handle camera movement if menu is not visible
        if (!gravityRenderer->isMenuVisible()) {
            if (firstMouse) {
                lastMouseX = mouseX;
                lastMouseY = mouseY;
                firstMouse = false;
            }
            
            double deltaX = mouseX - lastMouseX;
            double deltaY = lastMouseY - mouseY; // Reversed since y-coordinates go from bottom to top
            lastMouseX = mouseX;
            lastMouseY = mouseY;
            
            bool isMouseHeld = glfwGetMouseButton(glfwWindow, GLFW_MOUSE_BUTTON_LEFT) == GLFW_PRESS;
            gravityRenderer->updateCamera(static_cast<float>(deltaX), static_cast<float>(deltaY), isMouseHeld);
            
            // Keyboard camera movement
            float forward = 0.0f, right = 0.0f, up = 0.0f;
            
            // WASD movement
            if (glfwGetKey(glfwWindow, GLFW_KEY_W) == GLFW_PRESS) {
                if (glfwGetKey(glfwWindow, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS && 
                    gravityRenderer->getCameraMode() == GravityRenderer::CameraMode::FreeFlight) {
                    // Zoom in when Shift+W is pressed (free-flight mode only)
                    forward = 1.0f;
                } else {
                    forward = 1.0f;
                }
            }
            if (glfwGetKey(glfwWindow, GLFW_KEY_S) == GLFW_PRESS) {
                if (glfwGetKey(glfwWindow, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS && 
                    gravityRenderer->getCameraMode() == GravityRenderer::CameraMode::FreeFlight) {
                    // Zoom out when Shift+S is pressed (free-flight mode only)
                    forward = -1.0f;
                } else {
                    forward = -1.0f;
                }
            }
            if (glfwGetKey(glfwWindow, GLFW_KEY_A) == GLFW_PRESS) right = -1.0f;
            if (glfwGetKey(glfwWindow, GLFW_KEY_D) == GLFW_PRESS) right = 1.0f;
            
            // QE for up/down movement
            if (glfwGetKey(glfwWindow, GLFW_KEY_Q) == GLFW_PRESS) up = 1.0f;
            if (glfwGetKey(glfwWindow, GLFW_KEY_E) == GLFW_PRESS) up = -1.0f;
            
            gravityRenderer->moveCameraKeyboard(forward, right, up);
        }
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