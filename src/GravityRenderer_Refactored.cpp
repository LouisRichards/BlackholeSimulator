/**
 * @file GravityRenderer.cpp
 * @brief Refactored implementation following SOLID principles
 * @author BlackholeSimulator Team
 * @date 2025-08-11
 */

#include "../include/GravityRenderer.h"
#include <iostream>
#include <cmath>
#include <algorithm>

#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif

GravityRenderer::GravityRenderer(float viewportWidth, float viewportHeight)
    : viewportWidth(viewportWidth), viewportHeight(viewportHeight), 
      maxForceVisualization(500.0f), isInitialized(false) {}

void GravityRenderer::initialize() {
    // Set up OpenGL state for 3D rendering
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    
    setup3DProjection();
    
    // Set viewport
    glViewport(0, 0, static_cast<GLsizei>(viewportWidth), static_cast<GLsizei>(viewportHeight));
    
    isInitialized = true;
    std::cout << "GravityRenderer initialized with " << viewportWidth << "x" << viewportHeight << " viewport" << std::endl;
}

void GravityRenderer::render(const CameraController& cameraController, UIRenderer* uiRenderer) {
    if (!isInitialized) {
        std::cerr << "Warning: GravityRenderer not initialized" << std::endl;
        return;
    }

    // Clear buffers
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glClearColor(0.0f, 0.0f, 0.05f, 1.0f); // Dark blue background

    // Set up 3D perspective projection and apply camera transform
    setup3DProjection();
    
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    
    // Apply camera transformation (delegated to CameraController)
    cameraController.applyTransform();

    // Render 3D gravity visualization
    render3DSpacetimeGrid();
    render3DGravityBodies();
    
    // Render UI overlay if provided
    if (uiRenderer) {
        uiRenderer->beginUIMode();
        // UI rendering is handled by the UIRenderer component
        uiRenderer->endUIMode();
    }
}

void GravityRenderer::render() {
    // Legacy interface - render without camera or UI components
    if (!isInitialized) {
        std::cerr << "Warning: GravityRenderer not initialized" << std::endl;
        return;
    }

    // Basic rendering without external components
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glClearColor(0.0f, 0.0f, 0.05f, 1.0f);

    setup3DProjection();
    
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    
    // Simple default camera position
    glTranslatef(0.0f, 0.0f, -800.0f);

    render3DSpacetimeGrid();
    render3DGravityBodies();
}

void GravityRenderer::cleanup() {
    // Clean up any OpenGL resources if needed
    isInitialized = false;
    std::cout << "GravityRenderer cleaned up" << std::endl;
}

void GravityRenderer::setViewportSize(float width, float height) {
    viewportWidth = width;
    viewportHeight = height;
    
    if (isInitialized) {
        setup3DProjection();
        glViewport(0, 0, static_cast<GLsizei>(width), static_cast<GLsizei>(height));
    }
}

void GravityRenderer::setup3DProjection() {
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    
    // Set up perspective projection
    float fovy = 60.0f; // Field of view in degrees
    float aspect = viewportWidth / viewportHeight;
    float zNear = 1.0f;
    float zFar = 2000.0f;
    
    float fH = tan(fovy / 360.0f * M_PI) * zNear;
    float fW = fH * aspect;
    
    glFrustum(-fW, fW, -fH, fH, zNear, zFar);
}

void GravityRenderer::render3DSpacetimeGrid() {
    if (!gravityGrid) return;

    // Enable polygon offset to avoid z-fighting
    glEnable(GL_POLYGON_OFFSET_FILL);
    glPolygonOffset(1.0f, 1.0f);
    
    // Get grid properties
    float gridSizeX = static_cast<float>(gravityGrid->getWidth());
    float gridSizeY = static_cast<float>(gravityGrid->getHeight());
    float gridScale = 600.0f; // Scale factor for visualization
    
    // Calculate grid spacing
    float spacingX = gridScale / gridSizeX;
    float spacingY = gridScale / gridSizeY;
    
    // Render grid as a mesh of quads with height representing gravitational potential
    glBegin(GL_QUADS);
    
    for (int x = 0; x < gravityGrid->getWidth() - 1; ++x) {
        for (int y = 0; y < gravityGrid->getHeight() - 1; ++y) {
            // Calculate world positions
            float worldX1 = (x - gridSizeX/2) * spacingX;
            float worldY1 = (y - gridSizeY/2) * spacingY;
            float worldX2 = ((x+1) - gridSizeX/2) * spacingX;
            float worldY2 = ((y+1) - gridSizeY/2) * spacingY;
            
            // Get gravitational field strengths at grid points
            Vec2 force1 = gravityGrid->getForceAt(x, y);
            Vec2 force2 = gravityGrid->getForceAt(x+1, y);
            Vec2 force3 = gravityGrid->getForceAt(x+1, y+1);
            Vec2 force4 = gravityGrid->getForceAt(x, y+1);
            
            // Calculate heights based on force magnitude (gravitational potential)
            float height1 = sqrt(force1.x * force1.x + force1.y * force1.y) * 0.5f;
            float height2 = sqrt(force2.x * force2.x + force2.y * force2.y) * 0.5f;
            float height3 = sqrt(force3.x * force3.x + force3.y * force3.y) * 0.5f;
            float height4 = sqrt(force4.x * force4.x + force4.y * force4.y) * 0.5f;
            
            // Clamp heights for visualization
            height1 = std::min(height1, 200.0f);
            height2 = std::min(height2, 200.0f);
            height3 = std::min(height3, 200.0f);
            height4 = std::min(height4, 200.0f);
            
            // Apply color based on gravitational field strength
            Vec2 avgForce = forceToColor((height1 + height2 + height3 + height4) / 4.0f);
            glColor3f(avgForce.x, 0.2f, avgForce.y); // Red-Blue gradient
            
            // Create warped grid quad
            glVertex3f(worldX1, -height1, worldY1);  // Bottom-left
            glVertex3f(worldX2, -height2, worldY1);  // Bottom-right
            glVertex3f(worldX2, -height3, worldY2);  // Top-right
            glVertex3f(worldX1, -height4, worldY2);  // Top-left
        }
    }
    
    glEnd();
    
    // Render grid lines for better visualization
    glColor3f(0.3f, 0.3f, 0.3f);
    glBegin(GL_LINES);
    
    // Horizontal lines
    for (int y = 0; y < gravityGrid->getHeight(); ++y) {
        for (int x = 0; x < gravityGrid->getWidth() - 1; ++x) {
            float worldX1 = (x - gridSizeX/2) * spacingX;
            float worldY = (y - gridSizeY/2) * spacingY;
            float worldX2 = ((x+1) - gridSizeX/2) * spacingX;
            
            Vec2 force1 = gravityGrid->getForceAt(x, y);
            Vec2 force2 = gravityGrid->getForceAt(x+1, y);
            
            float height1 = std::min(sqrt(force1.x * force1.x + force1.y * force1.y) * 0.5f, 200.0f);
            float height2 = std::min(sqrt(force2.x * force2.x + force2.y * force2.y) * 0.5f, 200.0f);
            
            glVertex3f(worldX1, -height1, worldY);
            glVertex3f(worldX2, -height2, worldY);
        }
    }
    
    // Vertical lines
    for (int x = 0; x < gravityGrid->getWidth(); ++x) {
        for (int y = 0; y < gravityGrid->getHeight() - 1; ++y) {
            float worldX = (x - gridSizeX/2) * spacingX;
            float worldY1 = (y - gridSizeY/2) * spacingY;
            float worldY2 = ((y+1) - gridSizeY/2) * spacingY;
            
            Vec2 force1 = gravityGrid->getForceAt(x, y);
            Vec2 force2 = gravityGrid->getForceAt(x, y+1);
            
            float height1 = std::min(sqrt(force1.x * force1.x + force1.y * force1.y) * 0.5f, 200.0f);
            float height2 = std::min(sqrt(force2.x * force2.x + force2.y * force2.y) * 0.5f, 200.0f);
            
            glVertex3f(worldX, -height1, worldY1);
            glVertex3f(worldX, -height2, worldY2);
        }
    }
    
    glEnd();
    glDisable(GL_POLYGON_OFFSET_FILL);
}

void GravityRenderer::render3DGravityBodies() {
    for (const auto& body : gravityBodies) {
        if (!body) continue;
        
        // Get body properties
        Vec2 bodyPos = body->getPosition();
        float mass = body->getMass();
        
        // Convert to 3D world coordinates
        Vec3 worldPos(bodyPos.x - 300.0f, 50.0f, bodyPos.y - 300.0f); // Center and elevate
        float radius = std::max(5.0f, std::min(50.0f, mass / 1000.0f)); // Scale radius by mass
        
        // Set color based on mass
        float massRatio = std::min(1.0f, mass / 50000.0f);
        glColor3f(1.0f, 1.0f - massRatio, 0.0f); // Yellow to red gradient
        
        // Render 3D sphere
        render3DSphere(worldPos, radius);
        
        // Render glow effect
        glColor4f(1.0f, 0.8f, 0.0f, 0.3f);
        render3DSphere(worldPos, radius * 1.5f, 8);
    }
}

void GravityRenderer::render3DSphere(const Vec3& center, float radius, int segments) {
    // Render a simple 3D sphere using latitude/longitude approach
    for (int i = 0; i < segments; ++i) {
        float lat1 = (i / float(segments)) * M_PI - M_PI/2;
        float lat2 = ((i + 1) / float(segments)) * M_PI - M_PI/2;
        
        glBegin(GL_QUAD_STRIP);
        for (int j = 0; j <= segments; ++j) {
            float lng = (j / float(segments)) * 2 * M_PI;
            
            float x1 = cos(lat1) * cos(lng);
            float y1 = sin(lat1);
            float z1 = cos(lat1) * sin(lng);
            
            float x2 = cos(lat2) * cos(lng);
            float y2 = sin(lat2);
            float z2 = cos(lat2) * sin(lng);
            
            glVertex3f(center.x + radius * x1, center.y + radius * y1, center.z + radius * z1);
            glVertex3f(center.x + radius * x2, center.y + radius * y2, center.z + radius * z2);
        }
        glEnd();
    }
}

Vec2 GravityRenderer::worldToNDC(const Vec2& worldPos) const {
    // Simple world to normalized device coordinate conversion
    float ndcX = (worldPos.x / (viewportWidth / 2.0f)) - 1.0f;
    float ndcY = 1.0f - (worldPos.y / (viewportHeight / 2.0f));
    return Vec2(ndcX, ndcY);
}

Vec2 GravityRenderer::forceToColor(float forceMagnitude) const {
    // Map force magnitude to color (blue to red gradient)
    float normalizedForce = std::min(1.0f, forceMagnitude / maxForceVisualization);
    
    // Blue to red gradient
    float red = normalizedForce;
    float blue = 1.0f - normalizedForce;
    
    return Vec2(red, blue);
}

void GravityRenderer::renderCircle(const Vec2& center, float radius, int segments) {
    glBegin(GL_TRIANGLE_FAN);
    glVertex2f(center.x, center.y); // Center vertex
    
    for (int i = 0; i <= segments; ++i) {
        float angle = (i / float(segments)) * 2.0f * M_PI;
        float x = center.x + radius * cos(angle);
        float y = center.y + radius * sin(angle);
        glVertex2f(x, y);
    }
    
    glEnd();
}
