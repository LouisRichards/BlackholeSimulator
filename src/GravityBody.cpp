#include "../include/GravityBody.h"
#include <algorithm>

GravityBody::GravityBody(const Vec2& position, float mass, float radius)
    : position(position), velocity(0.0f, 0.0f), mass(mass), radius(radius) {}

Vec2 GravityBody::calculateGravitationalForce(const Vec2& point, float gravitationalConstant) const {
    Vec2 direction = position - point;
    float distanceSquared = direction.magnitudeSquared();
    
    // Avoid division by zero and singularities
    const float minDistance = 1.0f;
    if (distanceSquared < minDistance * minDistance) {
        distanceSquared = minDistance * minDistance;
    }
    
    // F = G * m1 * m2 / r^2, but we assume unit mass at the point
    float forceMagnitude = (gravitationalConstant * mass) / distanceSquared;
    
    // Limit maximum force to prevent numerical instabilities
    const float maxForce = 1000.0f;
    forceMagnitude = std::min(forceMagnitude, maxForce);
    
    return direction.normalized() * forceMagnitude;
}

Vec2 GravityBody::calculateForceFrom(const GravityBody& other, float gravitationalConstant) const {
    Vec2 direction = other.position - position;
    float distanceSquared = direction.magnitudeSquared();
    
    // Avoid division by zero and prevent bodies from getting too close
    const float minDistance = 10.0f; // Minimum separation distance
    if (distanceSquared < minDistance * minDistance) {
        distanceSquared = minDistance * minDistance;
    }
    
    // F = G * m1 * m2 / r^2
    float forceMagnitude = (gravitationalConstant * mass * other.mass) / distanceSquared;
    
    // Limit maximum force to prevent numerical instabilities
    const float maxForce = 5000.0f;
    forceMagnitude = std::min(forceMagnitude, maxForce);
    
    return direction.normalized() * forceMagnitude;
}

void GravityBody::applyForce(const Vec2& force, float deltaTime) {
    // F = ma, so a = F/m
    Vec2 acceleration = force / mass;
    
    // Update velocity: v = v0 + a*dt
    velocity = velocity + acceleration * deltaTime;
    
    // Apply very light velocity damping to prevent numerical instabilities
    // (reduced from 0.999f to preserve orbital motion)
    const float dampingFactor = 0.9999f;
    velocity = velocity * dampingFactor;
}

void GravityBody::updatePosition(float deltaTime) {
    // Update position: x = x0 + v*dt
    // This should apply the current velocity immediately, including initial velocity
    position = position + velocity * deltaTime;
}
