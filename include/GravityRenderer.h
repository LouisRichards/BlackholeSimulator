/**
 * @file GravityRenderer.h
 * @brief Renderer for gravity grid and gravitational bodies
 * @author BlackholeSimulator Team
 * @date 2025-08-11
 */

#pragma once
#include "IRenderer.h"
#include "GravityGrid.h"
#include "GravityBody.h"
#include <GL/glew.h>
#include <vector>
#include <memory>

/**
 * @class GravityRenderer
 * @brief Advanced renderer for gravity simulation visualization
 * 
 * This renderer extends the basic renderer to visualize gravitational fields
 * using a color-coded grid and render gravitational bodies. Follows the
 * Single Responsibility Principle by focusing on gravity visualization.
 */
class GravityRenderer : public IRenderer {
public:
    /**
     * @brief Constructs a gravity renderer with specified viewport
     * @param viewportWidth Width of the rendering viewport
     * @param viewportHeight Height of the rendering viewport
     */
    GravityRenderer(float viewportWidth, float viewportHeight);

    /**
     * @brief Initializes OpenGL state for gravity rendering
     */
    void initialize() override;

    /**
     * @brief Renders the gravity grid and bodies
     */
    void render() override;

    /**
     * @brief Cleans up rendering resources
     */
    void cleanup() override;

    /**
     * @brief Sets the gravity grid to render
     * @param grid Shared pointer to the gravity grid
     */
    void setGravityGrid(std::shared_ptr<GravityGrid> grid) { 
        gravityGrid = grid; 
    }

    /**
     * @brief Sets the gravitational bodies to render
     * @param bodies Vector of gravitational bodies
     */
    void setGravityBodies(const std::vector<std::shared_ptr<GravityBody>>& bodies) { 
        gravityBodies = bodies; 
    }

    /**
     * @brief Sets the maximum force value for color scaling
     * @param maxForce Maximum force magnitude for visualization
     */
    void setMaxForceForVisualization(float maxForce) { 
        maxForceVisualization = maxForce; 
    }

private:
    float viewportWidth, viewportHeight;                         ///< Viewport dimensions
    std::shared_ptr<GravityGrid> gravityGrid;                   ///< Grid to visualize
    std::vector<std::shared_ptr<GravityBody>> gravityBodies;    ///< Bodies to render
    float maxForceVisualization;                                ///< Max force for color scaling
    bool isInitialized;                                         ///< Initialization state

    // Forward declaration for 3D vector
    struct Vec3 { float x, y, z; Vec3(float x=0, float y=0, float z=0) : x(x), y(y), z(z) {} };

    /**
     * @brief Renders the 3D spacetime grid that warps around massive objects
     */
    void render3DSpacetimeGrid();

    /**
     * @brief Renders a pseudo-3D spacetime grid using 2D techniques
     */
    void renderPseudo3DSpacetimeGrid();

    /**
     * @brief Renders gravitational bodies as 3D spheres above the grid
     */
    void render3DGravityBodies();

    /**
     * @brief Renders a 3D sphere at the specified position
     * @param center Center position in 3D space
     * @param radius Radius of the sphere
     * @param segments Number of segments for sphere detail
     */
    void render3DSphere(const Vec3& center, float radius, int segments = 12);

    /**
     * @brief Converts world coordinates to OpenGL normalized coordinates
     * @param worldPos Position in world space
     * @return Normalized device coordinates (-1 to 1)
     */
    Vec2 worldToNDC(const Vec2& worldPos) const;

    /**
     * @brief Maps force magnitude to a color (blue to red gradient)
     * @param forceMagnitude Force magnitude to map
     * @return RGB color components (0-1 range)
     */
    Vec2 forceToColor(float forceMagnitude) const;  // Returns RGB as Vec2 for simplicity

    /**
     * @brief Renders a circle at the specified position
     * @param center Center position in world coordinates
     * @param radius Radius in world units
     * @param segments Number of segments for circle approximation
     */
    void renderCircle(const Vec2& center, float radius, int segments = 16);
};
