/**
 * @file GravityBody.h
 * @brief Gravitational body representation for physics simulation
 * @author BlackholeSimulator Team
 * @date 2025-08-11
 */

#pragma once
#include "Vec2.h"

/**
 * @class GravityBody
 * @brief Represents a gravitational body with mass and position
 * 
 * This class encapsulates the properties of a gravitational body that can
 * influence the gravity grid. Follows the Single Responsibility Principle
 * by handling only the physics properties of a gravitational object.
 */
class GravityBody {
public:
    /**
     * @brief Constructs a gravity body with specified properties
     * @param position Initial position of the body
     * @param mass Mass of the body (affects gravitational strength)
     * @param radius Visual radius for rendering
     */
    GravityBody(const Vec2& position, float mass, float radius = 5.0f);

    /**
     * @brief Gets the current position of the body
     * @return Position vector
     */
    const Vec2& getPosition() const { return position; }

    /**
     * @brief Gets the mass of the body
     * @return Mass value
     */
    float getMass() const { return mass; }

    /**
     * @brief Gets the visual radius of the body
     * @return Radius value
     */
    float getRadius() const { return radius; }

    /**
     * @brief Sets a new position for the body
     * @param newPosition New position vector
     */
    void setPosition(const Vec2& newPosition) { position = newPosition; }

    /**
     * @brief Gets the current velocity of the body
     * @return Velocity vector
     */
    const Vec2& getVelocity() const { return velocity; }

    /**
     * @brief Sets the velocity of the body
     * @param newVelocity New velocity vector
     */
    void setVelocity(const Vec2& newVelocity) { velocity = newVelocity; }

    /**
     * @brief Applies a force to the body for one timestep
     * @param force Force vector to apply
     * @param deltaTime Time step duration
     */
    void applyForce(const Vec2& force, float deltaTime);

    /**
     * @brief Updates position based on current velocity
     * @param deltaTime Time step duration
     */
    void updatePosition(float deltaTime);

    /**
     * @brief Calculates gravitational force at a given point
     * @param point Position to calculate force at
     * @param gravitationalConstant G constant for force calculation
     * @return Force vector pointing toward this body
     */
    Vec2 calculateGravitationalForce(const Vec2& point, float gravitationalConstant = 100.0f) const;

    /**
     * @brief Calculates gravitational force between this body and another
     * @param other The other gravitational body
     * @param gravitationalConstant G constant for force calculation
     * @return Force vector that should be applied to this body
     */
    Vec2 calculateForceFrom(const GravityBody& other, float gravitationalConstant = 100.0f) const;

private:
    Vec2 position;   ///< Current position of the body
    Vec2 velocity;   ///< Current velocity of the body
    float mass;      ///< Mass of the body
    float radius;    ///< Visual radius for rendering
};
