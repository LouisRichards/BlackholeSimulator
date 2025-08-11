#include "../include/GravityBody.h"
#include <algorithm>

GravityBody::GravityBody(const Vec2& position, float mass, float radius)
    : position(position), mass(mass), radius(radius) {}

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
