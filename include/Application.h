/**
 * @file Application.h
 * @brief Main application class that coordinates window and renderer
 * @author BlackholeSimulator Team
 * @date 2025-08-11
 */

#pragma once
#include "IWindow.h"
#include "IRenderer.h"
#include <memory>

/**
 * @class Application
 * @brief Main application class that orchestrates the window and renderer
 * 
 * This class serves as the main coordinator for the application, managing
 * the interaction between the window system and the renderer. It follows
 * the Single Responsibility Principle by focusing on application lifecycle
 * management and demonstrates the Dependency Inversion Principle by depending
 * on abstractions (IWindow, IRenderer) rather than concrete implementations.
 */
class Application {
public:
    /**
     * @brief Constructs an Application with injected dependencies
     * @param window Unique pointer to a window implementation
     * @param renderer Unique pointer to a renderer implementation
     * 
     * This constructor uses dependency injection, allowing different window
     * and renderer implementations to be used without modifying the Application class.
     */
    Application(std::unique_ptr<IWindow> window, std::unique_ptr<IRenderer> renderer);
    
    /**
     * @brief Default destructor
     * 
     * Cleanup is handled automatically by the unique_ptr members and
     * explicit cleanup methods.
     */
    ~Application() = default;

    /**
     * @brief Initializes the application, window, and renderer
     * @return true if initialization was successful, false otherwise
     * 
     * This method performs the initialization sequence:
     * 1. Initialize the window system
     * 2. Initialize the renderer
     * 3. Mark application as ready to run
     */
    bool initialize();
    
    /**
     * @brief Runs the main application loop
     * 
     * Executes the main rendering loop until the window should close:
     * 1. Render a frame using the renderer
     * 2. Swap window buffers to display the frame
     * 3. Process window events (input, resize, etc.)
     * 
     * The application must be initialized before calling this method.
     */
    void run();
    
    /**
     * @brief Cleans up application resources
     * 
     * Performs cleanup of renderer and window resources.
     * Safe to call multiple times.
     */
    void cleanup();

private:
    std::unique_ptr<IWindow> window;     ///< Window management implementation
    std::unique_ptr<IRenderer> renderer; ///< Rendering implementation
    bool isInitialized;                  ///< Flag tracking initialization state
};
