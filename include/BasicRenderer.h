/**
 * @file BasicRenderer.h
 * @brief Basic OpenGL renderer implementation
 * @author BlackholeSimulator Team
 * @date 2025-08-11
 */

#pragma once
#include "IRenderer.h"
#include <GL/glew.h>

/**
 * @class BasicRenderer
 * @brief Basic implementation of the IRenderer interface using OpenGL
 * 
 * This class provides a simple OpenGL renderer that clears the screen with
 * a solid color. It demonstrates the Single Responsibility Principle by
 * focusing solely on basic rendering operations and follows the Liskov
 * Substitution Principle by being fully substitutable for IRenderer.
 */
class BasicRenderer : public IRenderer {
public:
    /**
     * @brief Initializes the basic renderer
     * 
     * Sets up any necessary OpenGL state for basic rendering.
     * Currently performs minimal setup but can be extended for
     * more complex rendering initialization.
     */
    void initialize() override;
    
    /**
     * @brief Renders a single frame
     * 
     * Clears the screen with a dark blue color. This method is called
     * once per frame in the main rendering loop.
     */
    void render() override;
    
    /**
     * @brief Cleans up renderer resources
     * 
     * Releases any allocated rendering resources. Currently minimal
     * but provides the foundation for more complex cleanup operations.
     */
    void cleanup() override;

private:
    bool isInitialized = false;  ///< Flag tracking initialization state
};
