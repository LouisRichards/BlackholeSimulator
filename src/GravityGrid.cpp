#include "../include/GravityGrid.h"
#include <algorithm>
#include <cmath>

GravityGrid::GravityGrid(float width, float height, int gridResolution)
    : worldWidth(width), worldHeight(height) {
    
    gridWidth = static_cast<int>(width * gridResolution / 100.0f);
    gridHeight = static_cast<int>(height * gridResolution / 100.0f);
    
    // Ensure minimum grid size
    gridWidth = std::max(gridWidth, 10);
    gridHeight = std::max(gridHeight, 10);
    
    gridSpacing = width / static_cast<float>(gridWidth - 1);
    
    // Initialize the forces grid
    forces.resize(gridHeight);
    for (auto& row : forces) {
        row.resize(gridWidth, Vec2(0.0f, 0.0f));
    }
}

void GravityGrid::updateGrid(const std::vector<std::shared_ptr<GravityBody>>& bodies, 
                            float gravitationalConstant) {
    // Reset all forces to zero
    for (int y = 0; y < gridHeight; ++y) {
        for (int x = 0; x < gridWidth; ++x) {
            forces[y][x] = Vec2(0.0f, 0.0f);
        }
    }
    
    // Calculate forces at each grid point
    for (int y = 0; y < gridHeight; ++y) {
        for (int x = 0; x < gridWidth; ++x) {
            Vec2 gridWorldPos = gridToWorld(x, y);
            Vec2 totalForce(0.0f, 0.0f);
            
            // Sum forces from all bodies
            for (const auto& body : bodies) {
                Vec2 force = body->calculateGravitationalForce(gridWorldPos, gravitationalConstant);
                totalForce += force;
            }
            
            forces[y][x] = totalForce;
        }
    }
}

Vec2 GravityGrid::getForceAtGridPoint(int gridX, int gridY) const {
    if (isValidGridPoint(gridX, gridY)) {
        return forces[gridY][gridX];
    }
    return Vec2(0.0f, 0.0f);
}

float GravityGrid::getForceMagnitudeAtGridPoint(int gridX, int gridY) const {
    if (isValidGridPoint(gridX, gridY)) {
        return forces[gridY][gridX].magnitude();
    }
    return 0.0f;
}

std::pair<int, int> GravityGrid::worldToGrid(const Vec2& worldPos) const {
    int gridX = static_cast<int>((worldPos.x / worldWidth) * (gridWidth - 1));
    int gridY = static_cast<int>((worldPos.y / worldHeight) * (gridHeight - 1));
    
    // Clamp to valid range
    gridX = std::max(0, std::min(gridX, gridWidth - 1));
    gridY = std::max(0, std::min(gridY, gridHeight - 1));
    
    return {gridX, gridY};
}

Vec2 GravityGrid::gridToWorld(int gridX, int gridY) const {
    float worldX = (static_cast<float>(gridX) / (gridWidth - 1)) * worldWidth;
    float worldY = (static_cast<float>(gridY) / (gridHeight - 1)) * worldHeight;
    
    return Vec2(worldX, worldY);
}

bool GravityGrid::isValidGridPoint(int gridX, int gridY) const {
    return gridX >= 0 && gridX < gridWidth && gridY >= 0 && gridY < gridHeight;
}
