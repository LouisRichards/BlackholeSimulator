/**
 * @file GravityGrid.h
 * @brief 2D grid system for visualizing gravitational fields
 * @author BlackholeSimulator Team
 * @date 2025-08-11
 */

#pragma once
#include "Vec2.h"
#include "GravityBody.h"
#include <vector>
#include <memory>

/**
 * @class GravityGrid
 * @brief 2D grid that visualizes gravitational fields from bodies
 * 
 * This class creates a 2D grid where each point represents the gravitational
 * force at that location, influenced by all gravitational bodies in the simulation.
 * Follows the Single Responsibility Principle by handling only grid calculations.
 */
class GravityGrid {
public:
    /**
     * @brief Constructs a gravity grid with specified dimensions
     * @param width Width of the simulation area
     * @param height Height of the simulation area
     * @param gridResolution Number of grid points per unit length
     */
    GravityGrid(float width, float height, int gridResolution = 20);

    /**
     * @brief Updates the gravity grid based on current bodies
     * @param bodies Vector of gravitational bodies to consider
     * @param gravitationalConstant G constant for force calculations
     */
    void updateGrid(const std::vector<std::shared_ptr<GravityBody>>& bodies, 
                    float gravitationalConstant = 100.0f);

    /**
     * @brief Gets the grid dimensions
     * @return Pair of (width, height) in grid points
     */
    std::pair<int, int> getGridDimensions() const { 
        return {gridWidth, gridHeight}; 
    }

    /**
     * @brief Gets the world dimensions
     * @return Pair of (width, height) in world units
     */
    std::pair<float, float> getWorldDimensions() const { 
        return {worldWidth, worldHeight}; 
    }

    /**
     * @brief Gets the gravitational force at a specific grid point
     * @param gridX Grid X coordinate
     * @param gridY Grid Y coordinate
     * @return Force vector at that grid point
     */
    Vec2 getForceAtGridPoint(int gridX, int gridY) const;

    /**
     * @brief Gets the magnitude of force at a grid point (for visualization)
     * @param gridX Grid X coordinate
     * @param gridY Grid Y coordinate
     * @return Force magnitude for color mapping
     */
    float getForceMagnitudeAtGridPoint(int gridX, int gridY) const;

    /**
     * @brief Converts world coordinates to grid coordinates
     * @param worldPos Position in world space
     * @return Grid coordinates as integers
     */
    std::pair<int, int> worldToGrid(const Vec2& worldPos) const;

    /**
     * @brief Converts grid coordinates to world coordinates
     * @param gridX Grid X coordinate
     * @param gridY Grid Y coordinate
     * @return World position
     */
    Vec2 gridToWorld(int gridX, int gridY) const;

private:
    float worldWidth, worldHeight;           ///< Dimensions in world units
    int gridWidth, gridHeight;               ///< Dimensions in grid points
    float gridSpacing;                       ///< Distance between grid points
    std::vector<std::vector<Vec2>> forces;   ///< 2D array of force vectors

    /**
     * @brief Calculates the index for the forces array
     * @param gridX Grid X coordinate
     * @param gridY Grid Y coordinate
     * @return True if coordinates are valid
     */
    bool isValidGridPoint(int gridX, int gridY) const;
};
