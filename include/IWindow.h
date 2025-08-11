/**
 * @file IWindow.h
 * @brief Abstract interface for window management systems
 * @author BlackholeSimulator Team
 * @date 2025-08-11
 */

#pragma once

/**
 * @class IWindow
 * @brief Abstract interface for window management implementations
 * 
 * This interface defines the contract that all window management implementations
 * must follow. It adheres to the Interface Segregation Principle by providing
 * only window-related methods and supports the Open/Closed Principle by allowing
 * new window systems (SDL, GLFW, Win32, etc.) without modifying existing code.
 */
class IWindow {
public:
    /**
     * @brief Virtual destructor for proper cleanup of derived classes
     */
    virtual ~IWindow() = default;
    
    /**
     * @brief Initializes the window system and creates the window
     * @return true if initialization was successful, false otherwise
     * 
     * This method should create the window, set up the OpenGL context,
     * and perform any necessary initialization for the windowing system.
     */
    virtual bool initialize() = 0;
    
    /**
     * @brief Cleans up window resources
     * 
     * Called when the window is no longer needed. Implementations should
     * destroy the window, release the OpenGL context, and clean up any
     * windowing system resources.
     */
    virtual void cleanup() = 0;
    
    /**
     * @brief Checks if the window should be closed
     * @return true if the window should close (user clicked X, Alt+F4, etc.)
     * 
     * This method is typically called in the main loop to determine when
     * the application should exit.
     */
    virtual bool shouldClose() const = 0;
    
    /**
     * @brief Swaps the front and back buffers
     * 
     * In double-buffered rendering, this displays the completed frame
     * by swapping the back buffer (where rendering occurs) with the
     * front buffer (what's displayed on screen).
     */
    virtual void swapBuffers() = 0;
    
    /**
     * @brief Processes pending window events
     * 
     * Polls the event queue for user input (keyboard, mouse) and window
     * events (resize, focus changes). This keeps the application responsive
     * and should be called every frame.
     */
    virtual void pollEvents() = 0;
};
