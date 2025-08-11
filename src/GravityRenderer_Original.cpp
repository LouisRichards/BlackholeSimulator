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
      maxForceVisualization(500.0f), isInitialized(false),
      cameraDistance(800.0f), cameraAngleX(30.0f), cameraAngleY(-15.0f),
      cameraPosX(0.0f), cameraPosY(0.0f), cameraPosZ(0.0f),
      cameraSpeed(10.0f), mouseSensitivity(0.5f),
      currentCameraMode(CameraMode::FreeFlight),
      gameYaw(0.0f), gamePitch(30.0f), gameEyeHeight(100.0f),
      menuVisible(false), menuX(10.0f), menuY(10.0f), 
      menuWidth(200.0f), menuHeight(100.0f) {}

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
    
    // Apply camera transformations based on current mode
    if (currentCameraMode == CameraMode::FreeFlight) {
        // Free-flight camera (original implementation)
        glTranslatef(0.0f, 0.0f, -cameraDistance);      // Distance from center
        glRotatef(cameraAngleX, 1.0f, 0.0f, 0.0f);      // Pitch rotation
        glRotatef(cameraAngleY, 0.0f, 1.0f, 0.0f);      // Yaw rotation
        glTranslatef(cameraPosX, cameraPosY, cameraPosZ); // Position offset
    } else {
        // Game-style camera
        glRotatef(-gamePitch, 1.0f, 0.0f, 0.0f);        // Pitch (look up/down)
        glRotatef(-gameYaw, 0.0f, 1.0f, 0.0f);          // Yaw (look left/right)
        glTranslatef(-cameraPosX, -(cameraPosY + gameEyeHeight), -cameraPosZ); // Position
    }
    
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
    
    // Render 2D menu overlay if visible
    if (menuVisible) {
        renderMenu();
    }
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

void GravityRenderer::updateCamera(float deltaX, float deltaY, bool isMousePressed) {
    if (isMousePressed) {
        if (currentCameraMode == CameraMode::FreeFlight) {
            // Free-flight camera mouse control
            cameraAngleY += deltaX * mouseSensitivity;
            cameraAngleX += deltaY * mouseSensitivity;
            
            // Clamp vertical rotation to prevent flipping
            if (cameraAngleX > 89.0f) cameraAngleX = 89.0f;
            if (cameraAngleX < -89.0f) cameraAngleX = -89.0f;
            
            // Wrap horizontal rotation
            if (cameraAngleY > 360.0f) cameraAngleY -= 360.0f;
            if (cameraAngleY < -360.0f) cameraAngleY += 360.0f;
        } else {
            // Game-style camera mouse control
            gameYaw += deltaX * mouseSensitivity;
            gamePitch += deltaY * mouseSensitivity;
            
            // Clamp pitch to prevent over-rotation
            if (gamePitch > 89.0f) gamePitch = 89.0f;
            if (gamePitch < -89.0f) gamePitch = -89.0f;
            
            // Wrap yaw
            if (gameYaw > 360.0f) gameYaw -= 360.0f;
            if (gameYaw < -360.0f) gameYaw += 360.0f;
        }
    }
}

void GravityRenderer::moveCameraKeyboard(float forward, float right, float up) {
    if (currentCameraMode == CameraMode::FreeFlight) {
        // Free-flight movement (original implementation)
        // Convert movement to camera space using trigonometry
        float radY = cameraAngleY * M_PI / 180.0f;
        float radX = cameraAngleX * M_PI / 180.0f;
        
        // Forward/backward movement (W/S keys)
        cameraPosX += forward * sin(radY) * cos(radX) * cameraSpeed;
        cameraPosY += forward * sin(radX) * cameraSpeed;
        cameraPosZ += forward * cos(radY) * cos(radX) * cameraSpeed;
        
        // Left/right strafe movement (A/D keys)
        cameraPosX += right * cos(radY) * cameraSpeed;
        cameraPosZ -= right * sin(radY) * cameraSpeed;
        
        // Up/down movement (Q/E keys)
        cameraPosY += up * cameraSpeed;
        
        // Mouse wheel zoom
        if (forward != 0.0f) {
            cameraDistance -= forward * cameraSpeed * 5.0f;
            if (cameraDistance < 50.0f) cameraDistance = 50.0f;
            if (cameraDistance > 2000.0f) cameraDistance = 2000.0f;
        }
    } else {
        // Game-style movement (ground-relative)
        float radYaw = gameYaw * M_PI / 180.0f;
        
        // Forward/backward relative to where we're looking (W/S keys)
        cameraPosX += forward * sin(radYaw) * cameraSpeed;
        cameraPosZ += forward * cos(radYaw) * cameraSpeed;
        
        // Left/right strafe movement (A/D keys)
        cameraPosX += right * cos(radYaw) * cameraSpeed;
        cameraPosZ -= right * sin(radYaw) * cameraSpeed;
        
        // Up/down movement (Q/E keys) - adjust eye height
        gameEyeHeight += up * cameraSpeed;
        if (gameEyeHeight < 10.0f) gameEyeHeight = 10.0f;   // Minimum eye height
        if (gameEyeHeight > 500.0f) gameEyeHeight = 500.0f; // Maximum eye height
    }
}

void GravityRenderer::setCameraMode(CameraMode mode) {
    CameraMode previousMode = currentCameraMode;
    currentCameraMode = mode;
    
    // Only synchronize if we're actually switching modes
    if (previousMode != mode) {
        if (mode == CameraMode::GameStyle) {
            // Switching from FreeFlight to GameStyle
            // Sync rotation angles
            gameYaw = cameraAngleY;
            gamePitch = cameraAngleX;
            
            // Set reasonable eye height while preserving X,Z position
            // cameraPosX and cameraPosZ remain the same
            // Convert cameraPosY to gameEyeHeight (eye height above ground)
            gameEyeHeight = std::max(10.0f, std::min(500.0f, -cameraPosY + 100.0f));
            
        } else if (mode == CameraMode::FreeFlight) {
            // Switching from GameStyle to FreeFlight  
            // Sync rotation angles
            cameraAngleY = gameYaw;
            cameraAngleX = gamePitch;
            
            // Convert gameEyeHeight back to cameraPosY
            // cameraPosX and cameraPosZ remain the same
            cameraPosY = -(gameEyeHeight - 100.0f);
        }
    }
}

GravityRenderer::CameraMode GravityRenderer::getCameraMode() const {
    return currentCameraMode;
}

void GravityRenderer::toggleMenu() {
    menuVisible = !menuVisible;
}

bool GravityRenderer::isMenuVisible() const {
    return menuVisible;
}

void GravityRenderer::handleMenu(double mouseX, double mouseY, bool isMouseClicked) {
    if (!menuVisible || !isMouseClicked) return;
    
    // Convert mouse coordinates to menu space
    float mx = static_cast<float>(mouseX);
    float my = static_cast<float>(viewportHeight - mouseY); // Flip Y coordinate
    
    // Check if click is within menu bounds
    if (mx >= menuX && mx <= menuX + menuWidth && my >= menuY && my <= menuY + menuHeight) {
        // Calculate which menu item was clicked
        float itemHeight = 30.0f;
        float clickY = my - menuY;
        
        if (clickY >= 20.0f && clickY <= 20.0f + itemHeight) {
            // Free-flight mode clicked
            setCameraMode(CameraMode::FreeFlight);
            menuVisible = false;
            std::cout << "Selected Free-flight camera mode\n";
        } else if (clickY >= 55.0f && clickY <= 55.0f + itemHeight) {
            // Game-style mode clicked  
            setCameraMode(CameraMode::GameStyle);
            menuVisible = false;
            std::cout << "Selected Game-style camera mode\n";
        }
    } else {
        // Click outside menu, close it
        menuVisible = false;
    }
}

void GravityRenderer::renderMenu() {
    // Save current OpenGL state
    glMatrixMode(GL_PROJECTION);
    glPushMatrix();
    glLoadIdentity();
    glOrtho(0, viewportWidth, 0, viewportHeight, -1, 1);
    
    glMatrixMode(GL_MODELVIEW);
    glPushMatrix();
    glLoadIdentity();
    
    glDisable(GL_DEPTH_TEST);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    
    // Render menu background
    glColor4f(0.2f, 0.2f, 0.2f, 0.9f);
    renderRect(menuX, menuY, menuWidth, menuHeight);
    
    // Render menu border
    glColor4f(0.8f, 0.8f, 0.8f, 1.0f);
    glLineWidth(2.0f);
    glBegin(GL_LINE_LOOP);
    glVertex2f(menuX, menuY);
    glVertex2f(menuX + menuWidth, menuY);
    glVertex2f(menuX + menuWidth, menuY + menuHeight);
    glVertex2f(menuX, menuY + menuHeight);
    glEnd();
    
    // Render menu title
    glColor4f(1.0f, 1.0f, 1.0f, 1.0f);
    renderText(menuX + 10.0f, menuY + menuHeight - 15.0f, "Camera Mode");
    
    // Render menu options
    // Free-flight option
    if (currentCameraMode == CameraMode::FreeFlight) {
        glColor4f(0.4f, 0.7f, 0.4f, 0.8f);
        renderRect(menuX + 5.0f, menuY + 20.0f, menuWidth - 10.0f, 30.0f);
    }
    glColor4f(1.0f, 1.0f, 1.0f, 1.0f);
    renderText(menuX + 10.0f, menuY + 35.0f, "Free-flight");
    
    // Game-style option  
    if (currentCameraMode == CameraMode::GameStyle) {
        glColor4f(0.4f, 0.7f, 0.4f, 0.8f);
        renderRect(menuX + 5.0f, menuY + 55.0f, menuWidth - 10.0f, 30.0f);
    }
    glColor4f(1.0f, 1.0f, 1.0f, 1.0f);
    renderText(menuX + 10.0f, menuY + 70.0f, "Game-style");
    
    // Restore OpenGL state
    glEnable(GL_DEPTH_TEST);
    glPopMatrix();
    glMatrixMode(GL_PROJECTION);
    glPopMatrix();
    glMatrixMode(GL_MODELVIEW);
}

void GravityRenderer::renderRect(float x, float y, float width, float height) {
    glBegin(GL_QUADS);
    glVertex2f(x, y);
    glVertex2f(x + width, y);
    glVertex2f(x + width, y + height);
    glVertex2f(x, y + height);
    glEnd();
}

void GravityRenderer::renderText(float x, float y, const char* text) {
    // Simple block-style text rendering using filled rectangles
    // This creates clean, readable text that's easy to see
    
    const char* str = text;
    float charWidth = 8.0f;
    float charHeight = 12.0f;
    float currentX = x;
    float blockSize = 1.5f;
    
    glDisable(GL_DEPTH_TEST);
    
    while (*str) {
        char c = *str;
        
        // Draw each character using filled rectangles in a 5x7 grid pattern
        // This creates a simple but readable bitmap font
        
        bool pattern[8][5] = {false}; // 8 rows (includes descenders), 5 columns
        
        switch (c) {
            case 'C':
                // Pattern for C
                pattern[0][1] = pattern[0][2] = pattern[0][3] = true;
                pattern[1][0] = pattern[2][0] = pattern[3][0] = pattern[4][0] = pattern[5][0] = true;
                pattern[6][1] = pattern[6][2] = pattern[6][3] = true;
                break;
                
            case 'a':
                // Pattern for a (lowercase)
                pattern[2][1] = pattern[2][2] = pattern[2][3] = true;
                pattern[3][0] = pattern[3][4] = true;
                pattern[4][1] = pattern[4][2] = pattern[4][3] = pattern[4][4] = true;
                pattern[5][0] = pattern[5][4] = true;
                pattern[6][1] = pattern[6][2] = pattern[6][3] = pattern[6][4] = true;
                break;
                
            case 'm':
                // Pattern for m
                pattern[2][0] = pattern[3][0] = pattern[4][0] = pattern[5][0] = pattern[6][0] = true;
                pattern[2][1] = pattern[2][3] = true;
                pattern[3][2] = pattern[4][2] = pattern[5][2] = pattern[6][2] = true;
                pattern[3][4] = pattern[4][4] = pattern[5][4] = pattern[6][4] = true;
                break;
                
            case 'e':
                // Pattern for e
                pattern[2][1] = pattern[2][2] = pattern[2][3] = true;
                pattern[3][0] = pattern[3][4] = true;
                pattern[4][0] = pattern[4][1] = pattern[4][2] = pattern[4][3] = true;
                pattern[5][0] = true;
                pattern[6][1] = pattern[6][2] = pattern[6][3] = true;
                break;
                
            case 'r':
                // Pattern for r
                pattern[2][0] = pattern[3][0] = pattern[4][0] = pattern[5][0] = pattern[6][0] = true;
                pattern[2][1] = pattern[2][2] = true;
                break;
                
            case 'o':
                // Pattern for o
                pattern[2][1] = pattern[2][2] = pattern[2][3] = true;
                pattern[3][0] = pattern[3][4] = true;
                pattern[4][0] = pattern[4][4] = true;
                pattern[5][0] = pattern[5][4] = true;
                pattern[6][1] = pattern[6][2] = pattern[6][3] = true;
                break;
                
            case 'M':
                // Pattern for M
                pattern[0][0] = pattern[1][0] = pattern[2][0] = pattern[3][0] = pattern[4][0] = pattern[5][0] = pattern[6][0] = true;
                pattern[0][4] = pattern[1][4] = pattern[2][4] = pattern[3][4] = pattern[4][4] = pattern[5][4] = pattern[6][4] = true;
                pattern[1][1] = pattern[2][2] = pattern[1][3] = true;
                break;
                
            case 'd':
                // Pattern for d
                pattern[0][4] = pattern[1][4] = pattern[2][4] = pattern[3][4] = pattern[4][4] = pattern[5][4] = pattern[6][4] = true;
                pattern[2][1] = pattern[2][2] = pattern[2][3] = true;
                pattern[3][0] = pattern[4][0] = pattern[5][0] = true;
                pattern[6][1] = pattern[6][2] = pattern[6][3] = true;
                break;
                
            case 'F':
                // Pattern for F
                pattern[0][0] = pattern[1][0] = pattern[2][0] = pattern[3][0] = pattern[4][0] = pattern[5][0] = pattern[6][0] = true;
                pattern[0][1] = pattern[0][2] = pattern[0][3] = true;
                pattern[3][1] = pattern[3][2] = true;
                break;
                
            case 'l':
                // Pattern for l
                pattern[0][2] = pattern[1][2] = pattern[2][2] = pattern[3][2] = pattern[4][2] = pattern[5][2] = pattern[6][2] = true;
                break;
                
            case 'i':
                // Pattern for i
                pattern[0][2] = true; // dot
                pattern[2][2] = pattern[3][2] = pattern[4][2] = pattern[5][2] = pattern[6][2] = true;
                break;
                
            case 'g':
                // Pattern for g
                pattern[2][1] = pattern[2][2] = pattern[2][3] = true;
                pattern[3][0] = pattern[4][0] = pattern[5][0] = true;
                pattern[4][1] = pattern[4][2] = pattern[4][3] = pattern[4][4] = true;
                pattern[5][4] = pattern[6][4] = true;
                pattern[7][1] = pattern[7][2] = pattern[7][3] = true; // extends below baseline
                break;
                
            case 'h':
                // Pattern for h
                pattern[0][0] = pattern[1][0] = pattern[2][0] = pattern[3][0] = pattern[4][0] = pattern[5][0] = pattern[6][0] = true;
                pattern[3][1] = pattern[3][2] = pattern[3][3] = true;
                pattern[4][4] = pattern[5][4] = pattern[6][4] = true;
                break;
                
            case 't':
                // Pattern for t
                pattern[1][2] = pattern[2][2] = pattern[3][2] = pattern[4][2] = pattern[5][2] = pattern[6][2] = true;
                pattern[3][0] = pattern[3][1] = pattern[3][3] = pattern[3][4] = true;
                break;
                
            case 'G':
                // Pattern for G
                pattern[0][1] = pattern[0][2] = pattern[0][3] = true;
                pattern[1][0] = pattern[2][0] = pattern[3][0] = pattern[4][0] = pattern[5][0] = true;
                pattern[3][2] = pattern[3][3] = pattern[3][4] = true;
                pattern[4][4] = pattern[5][4] = true;
                pattern[6][1] = pattern[6][2] = pattern[6][3] = true;
                break;
                
            case 's':
                // Pattern for s
                pattern[2][1] = pattern[2][2] = pattern[2][3] = true;
                pattern[3][0] = true;
                pattern[4][1] = pattern[4][2] = true;
                pattern[5][3] = true;
                pattern[6][0] = pattern[6][1] = pattern[6][2] = true;
                break;
                
            case 'y':
                // Pattern for y
                pattern[2][0] = pattern[3][0] = true;
                pattern[2][4] = pattern[3][4] = true;
                pattern[4][1] = pattern[5][2] = pattern[6][2] = true;
                pattern[7][1] = pattern[7][0] = true; // extends below baseline
                break;
                
            case 'n':
                // Pattern for n
                pattern[2][0] = pattern[3][0] = pattern[4][0] = pattern[5][0] = pattern[6][0] = true;
                pattern[2][1] = pattern[2][2] = pattern[2][3] = true;
                pattern[3][4] = pattern[4][4] = pattern[5][4] = pattern[6][4] = true;
                break;
                
            case '-':
                // Pattern for hyphen
                pattern[3][1] = pattern[3][2] = pattern[3][3] = true;
                break;
                
            case ' ':
                // Space - no pattern
                break;
                
            default:
                // Unknown character - draw a filled rectangle
                for (int row = 1; row < 6; row++) {
                    for (int col = 1; col < 4; col++) {
                        pattern[row][col] = true;
                    }
                }
                break;
        }
        
        // Render the pattern using filled rectangles
        glBegin(GL_QUADS);
        for (int row = 0; row < 8; row++) {
            for (int col = 0; col < 5; col++) {
                if (pattern[row][col]) {
                    float px = currentX + col * blockSize;
                    float py = y + (7 - row) * blockSize; // Flip Y to render top-to-bottom
                    
                    glVertex2f(px, py);
                    glVertex2f(px + blockSize, py);
                    glVertex2f(px + blockSize, py + blockSize);
                    glVertex2f(px, py + blockSize);
                }
            }
        }
        glEnd();
        
        currentX += charWidth;
        str++;
    }
    
    glEnable(GL_DEPTH_TEST);
}
