/**
 * @file GLFWWindow.h
 * @brief GLFW-based window implementation
 * @author BlackholeSimulator Team
 * @date 2025-08-11
 */

#pragma once
#include "IWindow.h"
#include "WindowProperties.h"
#include <GL/glew.h>
#include <GLFW/glfw3.h>

/**
 * @class GLFWWindow
 * @brief GLFW implementation of the IWindow interface
 * 
 * This class provides a concrete implementation of window management using
 * the GLFW library. It handles window creation, OpenGL context setup,
 * event processing, and cleanup. The class follows the Single Responsibility
 * Principle by focusing solely on GLFW window management.
 */
class GLFWWindow : public IWindow {
public:
    /**
     * @brief Constructs a GLFWWindow with specified properties
     * @param properties Configuration for the window (size, title, etc.)
     */
    explicit GLFWWindow(const WindowProperties& properties);
    
    /**
     * @brief Destructor that ensures proper cleanup
     */
    ~GLFWWindow();

    /**
     * @brief Initializes GLFW, creates the window, and sets up OpenGL context
     * @return true if initialization was successful, false otherwise
     * 
     * This method performs the complete initialization sequence:
     * 1. Initialize GLFW library
     * 2. Set OpenGL context hints
     * 3. Create the window
     * 4. Set up OpenGL context
     * 5. Initialize GLEW for OpenGL extensions
     */
    bool initialize() override;
    
    /**
     * @brief Cleans up GLFW window and terminates GLFW
     * 
     * Destroys the window and terminates the GLFW library.
     * Safe to call multiple times.
     */
    void cleanup() override;
    
    /**
     * @brief Checks if the user has requested to close the window
     * @return true if the window should close, false otherwise
     */
    bool shouldClose() const override;
    
    /**
     * @brief Swaps the front and back framebuffers
     * 
     * Displays the completed frame by swapping buffers.
     * This is part of the double-buffering rendering technique.
     */
    void swapBuffers() override;
    
    /**
     * @brief Processes all pending GLFW events
     * 
     * Polls for and processes events like keyboard input, mouse movement,
     * window resize, etc. Keeps the application responsive.
     */
    void pollEvents() override;

private:
    WindowProperties properties;  ///< Window configuration properties
    GLFWwindow* window;          ///< Pointer to the GLFW window object
    bool isInitialized;          ///< Flag tracking initialization state

    /**
     * @brief Initializes the GLFW library and sets OpenGL hints
     * @return true if GLFW initialization was successful
     */
    bool initializeGLFW();
    
    /**
     * @brief Creates the GLFW window and sets up the OpenGL context
     * @return true if window creation was successful
     */
    bool createWindow();
    
    /**
     * @brief Initializes GLEW for OpenGL extension loading
     * @return true if GLEW initialization was successful
     */
    bool initializeGLEW();
};
