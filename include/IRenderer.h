/**
 * @file IRenderer.h
 * @brief Abstract interface for rendering systems
 * @author BlackholeSimulator Team
 * @date 2025-08-11
 */

#pragma once

/**
 * @class IRenderer
 * @brief Abstract interface for renderer implementations
 * 
 * This interface defines the contract that all renderer implementations must follow.
 * It adheres to the Interface Segregation Principle by providing only rendering-related
 * methods and supports the Open/Closed Principle by allowing new renderer types
 * without modifying existing code.
 */
class IRenderer {
public:
    /**
     * @brief Virtual destructor for proper cleanup of derived classes
     */
    virtual ~IRenderer() = default;
    
    /**
     * @brief Renders a single frame
     * 
     * This method is called once per frame in the main rendering loop.
     * Implementations should perform all necessary drawing operations here.
     */
    virtual void render() = 0;
    
    /**
     * @brief Initializes the renderer
     * 
     * Called once before rendering begins. Implementations should set up
     * any necessary OpenGL state, shaders, or other rendering resources.
     */
    virtual void initialize() = 0;
    
    /**
     * @brief Cleans up renderer resources
     * 
     * Called when the renderer is no longer needed. Implementations should
     * release any allocated resources, delete shaders, etc.
     */
    virtual void cleanup() = 0;
};
