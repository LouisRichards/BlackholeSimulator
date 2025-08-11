/**
 * @file GravitySimulation.h
 * @brief Main gravity simulation coordinator
 * @author BlackholeSimulator Team
 * @date 2025-08-11
 */

#pragma once
#include "GravityGrid.h"
#include "GravityBody.h"
#include "GravityRenderer.h"
#include <vector>
#include <memory>

/**
 * @class GravitySimulation
 * @brief Coordinates the gravity simulation with grid and bodies
 * 
 * This class manages the entire gravity simulation, including bodies,
 * grid calculations, and rendering coordination. Follows the Single
 * Responsibility Principle by focusing on simulation management.
 */
class GravitySimulation {
public:
    /**
     * @brief Constructs a gravity simulation with specified parameters
     * @param worldWidth Width of the simulation world
     * @param worldHeight Height of the simulation world
     * @param gridResolution Resolution of the gravity grid
     */
    GravitySimulation(float worldWidth, float worldHeight, int gridResolution = 20);

    /**
     * @brief Initializes the simulation with default bodies
     */
    void initialize();

    /**
     * @brief Updates the simulation by one time step
     * @param deltaTime Time step for the update
     */
    void update(float deltaTime);

    /**
     * @brief Sets up the renderer with current simulation state
     * @param renderer Gravity renderer to configure
     */
    void setupRenderer(std::shared_ptr<GravityRenderer> renderer);

    /**
     * @brief Adds a gravitational body to the simulation
     * @param body Shared pointer to the body to add
     */
    void addBody(std::shared_ptr<GravityBody> body);

    /**
     * @brief Removes all bodies from the simulation
     */
    void clearBodies();

    /**
     * @brief Gets all gravitational bodies in the simulation
     * @return Vector of gravitational bodies
     */
    const std::vector<std::shared_ptr<GravityBody>>& getBodies() const { 
        return bodies; 
    }

    /**
     * @brief Gets the gravity grid
     * @return Shared pointer to the gravity grid
     */
    std::shared_ptr<GravityGrid> getGravityGrid() const { 
        return gravityGrid; 
    }

    /**
     * @brief Sets the gravitational constant for the simulation
     * @param g New gravitational constant value
     */
    void setGravitationalConstant(float g) { 
        gravitationalConstant = g; 
    }

    /**
     * @brief Gets the current gravitational constant
     * @return Current gravitational constant value
     */
    float getGravitationalConstant() const { 
        return gravitationalConstant; 
    }

private:
    std::shared_ptr<GravityGrid> gravityGrid;              ///< The gravity grid
    std::vector<std::shared_ptr<GravityBody>> bodies;      ///< Gravitational bodies
    float worldWidth, worldHeight;                         ///< World dimensions
    float gravitationalConstant;                           ///< G constant for physics
    bool needsGridUpdate;                                  ///< Flag for grid updates

    /**
     * @brief Creates default demonstration bodies
     */
    void createDefaultBodies();

    /**
     * @brief Updates body positions (for future animation)
     * @param deltaTime Time step for updates
     */
    void updateBodies(float deltaTime);
};
