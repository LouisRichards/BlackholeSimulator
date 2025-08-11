#include "../include/GravityRenderer.h"
#include <iostream>
#include <cmath>
#include <algorithm>

#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif

// Simple 3D vector structure for 3D rendering
struct Vec3 {
    float x, y, z;
    Vec3(float x = 0, float y = 0, float z = 0) : x(x), y(y), z(z) {}
};

GravityRenderer::GravityRenderer(float viewportWidth, float viewportHeight)
    : viewportWidth(viewportWidth), viewportHeight(viewportHeight), 
      maxForceVisualization(500.0f), isInitialized(false) {}

void GravityRenderer::initialize() {
    // Set up OpenGL state for 3D rendering
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    
    // Set up 3D perspective projection
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    
    // Simple perspective setup using trigonometry (like gluPerspective)
    float fovy = 60.0f; // Field of view in degrees
    float aspect = viewportWidth / viewportHeight;
    float zNear = 1.0f;
    float zFar = 2000.0f;
    
    float fH = tan(fovy / 360.0f * M_PI) * zNear;
    float fW = fH * aspect;
    
    glFrustum(-fW, fW, -fH, fH, zNear, zFar);
    
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    
    // Set viewport
    glViewport(0, 0, (int)viewportWidth, (int)viewportHeight);
    
    isInitialized = true;
    std::cout << "Gravity renderer initialized for 3D spacetime visualization\n";
}

void GravityRenderer::render() {
    if (!isInitialized) {
        std::cerr << "Gravity renderer not initialized!\n";
        return;
    }
    
    // Clear the screen and depth buffer
    glClearColor(0.02f, 0.02f, 0.05f, 1.0f); // Dark space background
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    
    glLoadIdentity();
    
    // Set up camera position to view the 3D grid
    glTranslatef(0.0f, 0.0f, -800.0f);    // Move camera back
    glRotatef(30.0f, 1.0f, 0.0f, 0.0f);   // Tilt down to see the surface
    glRotatef(-15.0f, 0.0f, 1.0f, 0.0f);  // Slight side angle
    
    // Center the scene
    glTranslatef(-viewportWidth/2.0f, -viewportHeight/2.0f, 0.0f);
    
    // Draw simple test axes to verify 3D is working
    glLineWidth(2.0f);
    glBegin(GL_LINES);
    // X axis (red)
    glColor3f(1.0f, 0.0f, 0.0f);
    glVertex3f(0, 0, 0); glVertex3f(200, 0, 0);
    // Y axis (green)
    glColor3f(0.0f, 1.0f, 0.0f);
    glVertex3f(0, 0, 0); glVertex3f(0, 200, 0);
    // Z axis (blue)
    glColor3f(0.0f, 0.0f, 1.0f);
    glVertex3f(0, 0, 0); glVertex3f(0, 0, 200);
    glEnd();
    
    // Render the 3D spacetime grid
    if (gravityGrid) {
        render3DSpacetimeGrid();
    }
    
    // Render gravitational bodies as 3D spheres
    render3DGravityBodies();
}

void GravityRenderer::cleanup() {
    isInitialized = false;
    std::cout << "Gravity renderer cleaned up\n";
}

void GravityRenderer::render3DSpacetimeGrid() {
    if (!gravityGrid) return;
    
    auto [gridWidth, gridHeight] = gravityGrid->getGridDimensions();
    auto [worldWidth, worldHeight] = gravityGrid->getWorldDimensions();
    
    // Debug output (once)
    static bool gridDebugPrinted = false;
    if (!gridDebugPrinted) {
        std::cout << "Rendering 3D spacetime grid: " << gridWidth << "x" << gridHeight << std::endl;
        gridDebugPrinted = true;
    }
    
    // Calculate spacing for the grid
    float stepX = worldWidth / std::max(1, gridWidth - 1);
    float stepZ = worldHeight / std::max(1, gridHeight - 1); // Using Z instead of Y for ground plane
    float maxDepth = 100.0f; // Maximum depth for gravity wells (downward)
    
    glLineWidth(1.0f);
    
    // Render grid lines along X direction (constant Z, varying X)
    for (int z = 0; z < gridHeight; z += 3) { // Skip lines for performance
        glBegin(GL_LINE_STRIP);
        for (int x = 0; x < gridWidth; x++) {
            float worldX = x * stepX;
            float worldZ = z * stepZ;
            
            // Get gravitational force magnitude to determine depth (Y displacement)
            float forceMagnitude = gravityGrid->getForceMagnitudeAtGridPoint(x, z);
            float depth = -std::min(forceMagnitude / maxForceVisualization * maxDepth, maxDepth);
            
            // Color intensity based on curvature (blue to red gradient)
            float intensity = std::min(forceMagnitude / (maxForceVisualization * 0.2f), 1.0f);
            glColor4f(0.3f + 0.7f * intensity, 0.7f - 0.5f * intensity, 1.0f - 0.8f * intensity, 0.8f);
            
            // Grid lies flat on XZ plane, Y is the vertical (depth) axis
            glVertex3f(worldX, depth, worldZ);
        }
        glEnd();
    }
    
    // Render grid lines along Z direction (constant X, varying Z)
    for (int x = 0; x < gridWidth; x += 3) { // Skip lines for performance
        glBegin(GL_LINE_STRIP);
        for (int z = 0; z < gridHeight; z++) {
            float worldX = x * stepX;
            float worldZ = z * stepZ;
            
            // Get gravitational force magnitude to determine depth (Y displacement)
            float forceMagnitude = gravityGrid->getForceMagnitudeAtGridPoint(x, z);
            float depth = -std::min(forceMagnitude / maxForceVisualization * maxDepth, maxDepth);
            
            // Color intensity based on curvature
            float intensity = std::min(forceMagnitude / (maxForceVisualization * 0.2f), 1.0f);
            glColor4f(0.3f + 0.7f * intensity, 0.7f - 0.5f * intensity, 1.0f - 0.8f * intensity, 0.8f);
            
            // Grid lies flat on XZ plane, Y is the vertical (depth) axis
            glVertex3f(worldX, depth, worldZ);
        }
        glEnd();
    }
}

void GravityRenderer::render3DGravityBodies() {
    for (const auto& body : gravityBodies) {
        Vec2 pos = body->getPosition();
        float radius = body->getRadius();
        float mass = body->getMass();
        
        // Position bodies above the horizontal grid surface
        float bodyHeight = 30.0f + (mass / 1000.0f) * 20.0f;
        
        // Color based on mass (larger mass = more golden)
        float massNormalized = std::min(mass / 1000.0f, 1.0f);
        glColor4f(1.0f, 0.8f + 0.2f * massNormalized, 0.2f + 0.3f * massNormalized, 0.9f);
        
        // Render as a 3D sphere positioned in XZ plane with Y elevation
        // pos.x -> X coordinate, pos.y from 2D -> Z coordinate, bodyHeight -> Y coordinate
        render3DSphere(Vec3(pos.x, bodyHeight, pos.y), radius);
    }
}

void GravityRenderer::render3DSphere(const Vec3& center, float radius, int segments) {
    glPushMatrix();
    glTranslatef(center.x, center.y, center.z);
    
    // Draw sphere using quad strips for better performance
    int latSegments = segments;
    int lonSegments = segments * 2;
    
    for (int lat = 0; lat < latSegments; lat++) {
        float lat0 = M_PI * (-0.5f + (float)lat / latSegments);
        float lat1 = M_PI * (-0.5f + (float)(lat + 1) / latSegments);
        float y0 = radius * sinf(lat0);
        float y1 = radius * sinf(lat1);
        float r0 = radius * cosf(lat0);
        float r1 = radius * cosf(lat1);
        
        glBegin(GL_QUAD_STRIP);
        for (int lon = 0; lon <= lonSegments; lon++) {
            float theta = 2.0f * M_PI * (float)lon / lonSegments;
            float x = cosf(theta);
            float z = sinf(theta);
            
            glVertex3f(x * r0, y0, z * r0);
            glVertex3f(x * r1, y1, z * r1);
        }
        glEnd();
    }
    
    glPopMatrix();
}

Vec2 GravityRenderer::worldToNDC(const Vec2& worldPos) const {
    // Convert world coordinates directly to screen coordinates
    // Since we set up orthographic projection to match world coordinates
    return Vec2(worldPos.x, worldPos.y);
}

Vec2 GravityRenderer::forceToColor(float forceMagnitude) const {
    float normalized = std::min(forceMagnitude / maxForceVisualization, 1.0f);
    return Vec2(normalized, 1.0f - normalized); // Red, Blue components
}

void GravityRenderer::renderCircle(const Vec2& center, float radius, int segments) {
    Vec2 screenCenter = worldToNDC(center);
    
    glBegin(GL_TRIANGLE_FAN);
    glVertex2f(screenCenter.x, screenCenter.y); // Center vertex
    
    for (int i = 0; i <= segments; ++i) {
        float angle = 2.0f * M_PI * i / segments;
        float x = screenCenter.x + radius * std::cos(angle);
        float y = screenCenter.y + radius * std::sin(angle);
        glVertex2f(x, y);
    }
    
    glEnd();
}
