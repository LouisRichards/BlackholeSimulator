#include "../include/GravitySimulation.h"
#include <iostream>

GravitySimulation::GravitySimulation(float worldWidth, float worldHeight, int gridResolution)
    : worldWidth(worldWidth), worldHeight(worldHeight), 
      gravitationalConstant(100.0f), needsGridUpdate(true) {
    
    gravityGrid = std::make_shared<GravityGrid>(worldWidth, worldHeight, gridResolution);
}

void GravitySimulation::initialize() {
    // Create some default bodies for demonstration
    createDefaultBodies();
    
    // Initial grid calculation
    gravityGrid->updateGrid(bodies, gravitationalConstant);
    needsGridUpdate = false;
    
    std::cout << "Gravity simulation initialized with " << bodies.size() << " bodies\n";
}

void GravitySimulation::update(float deltaTime) {
    // Update body positions (placeholder for future orbital mechanics)
    updateBodies(deltaTime);
    
    // Recalculate gravity grid if needed
    if (needsGridUpdate) {
        gravityGrid->updateGrid(bodies, gravitationalConstant);
        needsGridUpdate = false;
    }
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
    // Create a central massive body (black hole)
    auto centralBody = std::make_shared<GravityBody>(
        Vec2(worldWidth * 0.5f, worldHeight * 0.5f), // Center position
        1000.0f,  // Large mass
        15.0f     // Visual radius
    );
    addBody(centralBody);
    
    // Create a smaller body to the left
    auto smallBody1 = std::make_shared<GravityBody>(
        Vec2(worldWidth * 0.25f, worldHeight * 0.3f),
        200.0f,   // Smaller mass
        8.0f      // Smaller radius
    );
    addBody(smallBody1);
    
    // Create another small body to the right
    auto smallBody2 = std::make_shared<GravityBody>(
        Vec2(worldWidth * 0.75f, worldHeight * 0.7f),
        300.0f,   // Medium mass
        10.0f     // Medium radius
    );
    addBody(smallBody2);
    
    std::cout << "Created default gravity bodies:\n";
    std::cout << "  - Central body: mass=" << centralBody->getMass() 
              << " at (" << centralBody->getPosition().x << ", " << centralBody->getPosition().y << ")\n";
    std::cout << "  - Small body 1: mass=" << smallBody1->getMass() 
              << " at (" << smallBody1->getPosition().x << ", " << smallBody1->getPosition().y << ")\n";
    std::cout << "  - Small body 2: mass=" << smallBody2->getMass() 
              << " at (" << smallBody2->getPosition().x << ", " << smallBody2->getPosition().y << ")\n";
}

void GravitySimulation::updateBodies(float deltaTime) {
    // Placeholder for future orbital mechanics
    // Currently bodies are stationary, but this is where you would:
    // 1. Calculate forces between bodies
    // 2. Update velocities based on forces
    // 3. Update positions based on velocities
    // 4. Handle collisions or mergers
    
    // For now, we'll just mark that grid needs update if bodies move
    // needsGridUpdate = true;
}
