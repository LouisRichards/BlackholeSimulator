/**
 * @file Vec2.h
 * @brief 2D vector math utilities for physics calculations
 * @author BlackholeSimulator Team
 * @date 2025-08-11
 */

#pragma once
#include <cmath>

/**
 * @struct Vec2
 * @brief 2D vector structure for position, velocity, and force calculations
 * 
 * Provides basic 2D vector operations needed for physics simulations.
 * Follows the Single Responsibility Principle by handling only vector math.
 */
struct Vec2 {
    float x, y;  ///< X and Y components of the vector

    /**
     * @brief Default constructor initializes to zero vector
     */
    Vec2() : x(0.0f), y(0.0f) {}

    /**
     * @brief Constructs vector with specified components
     * @param x X component
     * @param y Y component
     */
    Vec2(float x, float y) : x(x), y(y) {}

    /**
     * @brief Vector addition operator
     * @param other Vector to add
     * @return Sum of the two vectors
     */
    Vec2 operator+(const Vec2& other) const {
        return Vec2(x + other.x, y + other.y);
    }

    /**
     * @brief Vector subtraction operator
     * @param other Vector to subtract
     * @return Difference of the two vectors
     */
    Vec2 operator-(const Vec2& other) const {
        return Vec2(x - other.x, y - other.y);
    }

    /**
     * @brief Scalar multiplication operator
     * @param scalar Value to multiply by
     * @return Scaled vector
     */
    Vec2 operator*(float scalar) const {
        return Vec2(x * scalar, y * scalar);
    }

    /**
     * @brief Vector addition assignment operator
     * @param other Vector to add
     * @return Reference to this vector
     */
    Vec2& operator+=(const Vec2& other) {
        x += other.x;
        y += other.y;
        return *this;
    }

    /**
     * @brief Calculates the squared magnitude of the vector
     * @return Squared magnitude (avoids expensive sqrt operation)
     */
    float magnitudeSquared() const {
        return x * x + y * y;
    }

    /**
     * @brief Calculates the magnitude of the vector
     * @return Magnitude of the vector
     */
    float magnitude() const {
        return std::sqrt(magnitudeSquared());
    }

    /**
     * @brief Returns a normalized version of the vector
     * @return Unit vector in the same direction
     */
    Vec2 normalized() const {
        float mag = magnitude();
        if (mag > 0.0f) {
            return Vec2(x / mag, y / mag);
        }
        return Vec2(0.0f, 0.0f);
    }
};
