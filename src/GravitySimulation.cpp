#include "../include/GravitySimulation.h"
#include <iostream>
#include <cmath>

GravitySimulation::GravitySimulation(float worldWidth, float worldHeight, int gridResolution)
    : worldWidth(worldWidth), worldHeight(worldHeight), 
      gravitationalConstant(80.0f), needsGridUpdate(true) { // Reduced G for solar system scale
    
    gravityGrid = std::make_shared<GravityGrid>(worldWidth, worldHeight, gridResolution);
}

void GravitySimulation::initialize() {
    // Create some default bodies for demonstration
    createDefaultBodies();
    
    // Initial grid calculation
    gravityGrid->updateGrid(bodies, gravitationalConstant);
    needsGridUpdate = false;
    
    std::cout << "Solar System simulation initialized with " << bodies.size() << " celestial bodies\n";
}

void GravitySimulation::update(float deltaTime) {
    // Update body physics (N-body gravitational attraction)
    updateBodies(deltaTime);
    
    // Recalculate gravity grid since bodies may have moved
    gravityGrid->updateGrid(bodies, gravitationalConstant);
}

void GravitySimulation::setupRenderer(std::shared_ptr<GravityRenderer> renderer) {
    if (renderer) {
        renderer->setGravityGrid(gravityGrid);
        renderer->setGravityBodies(bodies);
        renderer->setMaxForceForVisualization(500.0f);
    }
}

void GravitySimulation::addBody(std::shared_ptr<GravityBody> body) {
    bodies.push_back(body);
    needsGridUpdate = true;
}

void GravitySimulation::clearBodies() {
    bodies.clear();
    needsGridUpdate = true;
}

void GravitySimulation::createDefaultBodies() {
    // Create a realistic Solar System with proper orbital mechanics
    // All distances and velocities calculated for stable circular orbits
    // Using v = sqrt(G*M_sun/r) for orbital velocity
    
    float centerX = worldWidth * 0.5f;
    float centerY = worldHeight * 0.5f;
    
    // Sun - Central massive body (stationary at center)
    auto sun = std::make_shared<GravityBody>(
        Vec2(centerX, centerY),
        5000.0f,  // Sun mass
        15.0f     // Sun visual radius
    );
    sun->setVelocity(Vec2(0.0f, 0.0f)); // Sun stays at center
    addBody(sun);
    
    // Calculate orbital velocities: v = sqrt(G*M_sun/r)
    // G = 80.0f (our gravitational constant)
    // M_sun = 5000.0f
    float G_times_M_sun = gravitationalConstant * 5000.0f; // 80 * 5000 = 400,000
    
    // Mercury - Closest planet, fastest orbit
    float mercury_distance = 60.0f;
    float mercury_orbital_speed = sqrt(G_times_M_sun / mercury_distance);
    auto mercury = std::make_shared<GravityBody>(
        Vec2(centerX + mercury_distance, centerY),
        8.0f,   // Small mass
        2.5f    // Small radius
    );
    mercury->setVelocity(Vec2(0.0f, mercury_orbital_speed));
    addBody(mercury);
    
    // Venus - Second planet
    float venus_distance = 85.0f;
    float venus_orbital_speed = sqrt(G_times_M_sun / venus_distance);
    auto venus = std::make_shared<GravityBody>(
        Vec2(centerX + venus_distance, centerY),
        18.0f,  // Reduced mass for stability
        3.8f    // Similar size to Earth
    );
    venus->setVelocity(Vec2(0.0f, venus_orbital_speed));
    addBody(venus);
    
    // Earth - Third planet, our home
    float earth_distance = 110.0f;
    float earth_orbital_speed = sqrt(G_times_M_sun / earth_distance);
    auto earth = std::make_shared<GravityBody>(
        Vec2(centerX + earth_distance, centerY),
        20.0f,  // Reduced Earth mass for stability
        4.0f    // Earth radius
    );
    earth->setVelocity(Vec2(0.0f, earth_orbital_speed));
    addBody(earth);
    
    // Mars - Fourth planet, the red planet
    float mars_distance = 140.0f;
    float mars_orbital_speed = sqrt(G_times_M_sun / mars_distance);
    auto mars = std::make_shared<GravityBody>(
        Vec2(centerX + mars_distance, centerY),
        10.0f,  // Reduced mass for stability
        3.2f    // Smaller radius
    );
    mars->setVelocity(Vec2(0.0f, mars_orbital_speed));
    addBody(mars);
    
    // Jupiter - Fifth planet, gas giant (farther out and lighter)
    float jupiter_distance = 220.0f; // Moved farther from Mars
    float jupiter_orbital_speed = sqrt(G_times_M_sun / jupiter_distance);
    auto jupiter = std::make_shared<GravityBody>(
        Vec2(centerX + jupiter_distance, centerY),
        80.0f,  // Reduced mass to prevent gravitational chaos
        8.0f    // Large radius
    );
    jupiter->setVelocity(Vec2(0.0f, jupiter_orbital_speed));
    addBody(jupiter);
    
    // Saturn - Much farther out to avoid Jupiter's influence
    float saturn_distance = 300.0f; // Much larger gap
    float saturn_orbital_speed = sqrt(G_times_M_sun / saturn_distance);
    auto saturn = std::make_shared<GravityBody>(
        Vec2(centerX + saturn_distance, centerY),
        60.0f,  // Significantly reduced mass
        7.0f    // Large radius
    );
    saturn->setVelocity(Vec2(0.0f, saturn_orbital_speed));
    addBody(saturn);
    
    // Skip Uranus and Neptune for stability - focus on inner 6 planets
    
    // Debug: Print the realistic solar system setup
    std::cout << "Created Stable Solar System (6 planets):\n";
    std::cout << "  Sun: mass=" << sun->getMass() << " (stationary)\n";
    std::cout << "  Mercury: distance=" << mercury_distance << " velocity=" << mercury_orbital_speed << "\n";
    std::cout << "  � Venus: distance=" << venus_distance << " velocity=" << venus_orbital_speed << "\n";
    std::cout << "  � Earth: distance=" << earth_distance << " velocity=" << earth_orbital_speed << "\n";
    std::cout << "  Mars: distance=" << mars_distance << " velocity=" << mars_orbital_speed << "\n";
    std::cout << "  Jupiter: distance=" << jupiter_distance << " velocity=" << jupiter_orbital_speed << "\n";
    std::cout << "  Saturn: distance=" << saturn_distance << " velocity=" << saturn_orbital_speed << "\n";
    std::cout << "Stable orbits without gravitational interference!\n";
    std::cout << "Hold SPACE to accelerate time by 5x!\n";
}

void GravitySimulation::updateBodies(float deltaTime) {
    // N-body gravitational simulation
    // First, calculate all forces acting on each body
    std::vector<Vec2> forces(bodies.size(), Vec2(0.0f, 0.0f));
    
    for (size_t i = 0; i < bodies.size(); ++i) {
        for (size_t j = 0; j < bodies.size(); ++j) {
            if (i != j) {
                // Calculate gravitational force from body j on body i
                Vec2 force = bodies[i]->calculateForceFrom(*bodies[j], gravitationalConstant);
                forces[i] = forces[i] + force;
            }
        }
    }
    
    // Apply forces and update positions
    for (size_t i = 0; i < bodies.size(); ++i) {
        bodies[i]->applyForce(forces[i], deltaTime);
        bodies[i]->updatePosition(deltaTime);
        
        // Keep bodies within world bounds (simple boundary handling)
        Vec2 pos = bodies[i]->getPosition();
        Vec2 vel = bodies[i]->getVelocity();
        bool bounced = false;
        
        if (pos.x < 0 || pos.x > worldWidth) {
            vel.x = -vel.x * 0.8f; // Bounce with some energy loss
            pos.x = std::max(0.0f, std::min(worldWidth, pos.x));
            bounced = true;
        }
        
        if (pos.y < 0 || pos.y > worldHeight) {
            vel.y = -vel.y * 0.8f; // Bounce with some energy loss
            pos.y = std::max(0.0f, std::min(worldHeight, pos.y));
            bounced = true;
        }
        
        if (bounced) {
            bodies[i]->setPosition(pos);
            bodies[i]->setVelocity(vel);
        }
    }
}
