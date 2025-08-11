/**
 * @file WindowProperties.h
 * @brief Defines the WindowProperties structure for window configuration
 * @author BlackholeSimulator Team
 * @date 2025-08-11
 */

#pragma once

/**
 * @struct WindowProperties
 * @brief Structure containing window configuration parameters
 * 
 * This structure encapsulates all the properties needed to create and configure
 * a window, including dimensions and title. It follows the Single Responsibility
 * Principle by only handling window configuration data.
 */
struct WindowProperties {
    int width;          ///< Width of the window in pixels
    int height;         ///< Height of the window in pixels
    const char* title;  ///< Title displayed in the window's title bar
    
    /**
     * @brief Constructs WindowProperties with specified or default values
     * @param w Window width in pixels (default: 800)
     * @param h Window height in pixels (default: 600)
     * @param t Window title (default: "OpenGL Application")
     */
    WindowProperties(int w = 800, int h = 600, const char* t = "OpenGL Application")
        : width(w), height(h), title(t) {}
};
