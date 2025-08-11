/**
 * @file UIRenderer.cpp
 * @brief Simplified implementation of UI rendering system
 * @author BlackholeSimulator Team
 * @date 2025-08-11
 */

#include "../include/UIRenderer.h"
#include "../include/Camera.h"
#include <GL/glew.h>
#include <algorithm>

UIRenderer::UIRenderer(int windowWidth, int windowHeight)
    : windowWidth(windowWidth), windowHeight(windowHeight) {}

void UIRenderer::beginUIMode() {
    // Switch to 2D orthographic projection for UI
    glMatrixMode(GL_PROJECTION);
    glPushMatrix();
    glLoadIdentity();
    glOrtho(0, windowWidth, windowHeight, 0, -1, 1);
    
    glMatrixMode(GL_MODELVIEW);
    glPushMatrix();
    glLoadIdentity();
    
    // Disable depth testing for UI overlay
    glDisable(GL_DEPTH_TEST);
    
    // Enable blending for transparent UI elements
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
}

void UIRenderer::endUIMode() {
    // Restore 3D perspective projection
    glDisable(GL_BLEND);
    glEnable(GL_DEPTH_TEST);
    
    glMatrixMode(GL_PROJECTION);
    glPopMatrix();
    
    glMatrixMode(GL_MODELVIEW);
    glPopMatrix();
}

void UIRenderer::renderText(const std::string& text, float x, float y, float scale) {
    float currentX = x;
    
    for (char c : text) {
        if (c == '\n') {
            y += CHAR_HEIGHT * scale;
            currentX = x;
            continue;
        }
        
        drawCharacter(c, currentX, y, scale);
        currentX += CHAR_WIDTH * scale;
    }
}

void UIRenderer::renderPopupMenu(bool showMenu, int mouseX, int mouseY, CameraController::CameraMode currentMode) {
    if (!showMenu) return;
    
    // Use fixed position for the menu (center-left of screen)
    float menuX = 50.0f; // Fixed X position
    float menuY = 150.0f; // Fixed Y position
    
    // Ensure menu stays on screen
    if (menuX + MENU_WIDTH > windowWidth) {
        menuX = windowWidth - MENU_WIDTH - 10.0f;
    }
    if (menuY + MENU_HEIGHT > windowHeight) {
        menuY = windowHeight - MENU_HEIGHT - 10.0f;
    }
    
    lastMenuX = menuX;
    lastMenuY = menuY;
    
    // Render menu background
    renderMenuBackground(menuX, menuY, MENU_WIDTH, MENU_HEIGHT);
    
    // Menu title
    glColor3f(1.0f, 1.0f, 1.0f);
    renderText("Camera Mode:", menuX + MENU_PADDING, menuY + MENU_PADDING);
    
    // Menu items
    float itemY = menuY + MENU_PADDING + 20.0f;
    
    // FreeFlight Camera option
    bool freeFlightHighlighted = (mouseX >= menuX && mouseX <= menuX + MENU_WIDTH &&
                                 mouseY >= itemY && mouseY <= itemY + MENU_ITEM_HEIGHT);
    if (currentMode == CameraController::CameraMode::FreeFlight) {
        glColor3f(0.0f, 1.0f, 0.0f); // Green for active
    } else if (freeFlightHighlighted) {
        glColor3f(1.0f, 1.0f, 0.0f); // Yellow for hover
    } else {
        glColor3f(0.8f, 0.8f, 0.8f); // Gray for inactive
    }
    renderText("FreeFlight Camera", menuX + MENU_PADDING, itemY);
    
    // GameStyle Camera option
    itemY += MENU_ITEM_HEIGHT + 5.0f;
    bool gameStyleHighlighted = (mouseX >= menuX && mouseX <= menuX + MENU_WIDTH &&
                                mouseY >= itemY && mouseY <= itemY + MENU_ITEM_HEIGHT);
    if (currentMode == CameraController::CameraMode::GameStyle) {
        glColor3f(0.0f, 1.0f, 0.0f); // Green for active
    } else if (gameStyleHighlighted) {
        glColor3f(1.0f, 1.0f, 0.0f); // Yellow for hover
    } else {
        glColor3f(0.8f, 0.8f, 0.8f); // Gray for inactive
    }
    renderText("GameStyle Camera", menuX + MENU_PADDING, itemY);
    
    // Instructions
    itemY += MENU_ITEM_HEIGHT + 10.0f;
    glColor3f(0.7f, 0.7f, 0.7f);
    renderText("Click to select", menuX + MENU_PADDING, itemY);
}

void UIRenderer::setWindowSize(int width, int height) {
    windowWidth = width;
    windowHeight = height;
}

void UIRenderer::getWindowSize(int& width, int& height) const {
    width = windowWidth;
    height = windowHeight;
}

bool UIRenderer::isMouseOverMenu(int mouseX, int mouseY) const {
    return (mouseX >= lastMenuX && mouseX <= lastMenuX + MENU_WIDTH &&
            mouseY >= lastMenuY && mouseY <= lastMenuY + MENU_HEIGHT);
}

CameraController::CameraMode UIRenderer::getSelectedCameraMode(int mouseX, int mouseY) const {
    if (!isMouseOverMenu(mouseX, mouseY)) {
        return CameraController::CameraMode::FreeFlight; // Default fallback
    }
    
    float itemY = lastMenuY + MENU_PADDING + 20.0f;
    
    // Check FreeFlight option
    if (mouseY >= itemY && mouseY <= itemY + MENU_ITEM_HEIGHT) {
        return CameraController::CameraMode::FreeFlight;
    }
    
    // Check GameStyle option
    itemY += MENU_ITEM_HEIGHT + 5.0f;
    if (mouseY >= itemY && mouseY <= itemY + MENU_ITEM_HEIGHT) {
        return CameraController::CameraMode::GameStyle;
    }
    
    return CameraController::CameraMode::FreeFlight; // Default fallback
}

void UIRenderer::drawCharacter(char c, float x, float y, float scale) {
    // Simple bitmap font rendering with actual character shapes
    glColor3f(1.0f, 1.0f, 1.0f);
    
    // Define character rendering using line segments for readability
    glBegin(GL_LINES);
    
    switch (c) {
        case 'A': case 'a':
            // Draw letter A
            glVertex2f(x + 1*scale, y + 10*scale); glVertex2f(x + 4*scale, y + 1*scale);  // Left line
            glVertex2f(x + 4*scale, y + 1*scale); glVertex2f(x + 7*scale, y + 10*scale); // Right line
            glVertex2f(x + 2*scale, y + 6*scale); glVertex2f(x + 6*scale, y + 6*scale);  // Cross bar
            break;
        case 'B': case 'b':
            // Draw letter B
            glVertex2f(x + 1*scale, y + 1*scale); glVertex2f(x + 1*scale, y + 10*scale); // Left vertical
            glVertex2f(x + 1*scale, y + 1*scale); glVertex2f(x + 5*scale, y + 1*scale);  // Top horizontal
            glVertex2f(x + 1*scale, y + 5.5f*scale); glVertex2f(x + 5*scale, y + 5.5f*scale); // Middle horizontal
            glVertex2f(x + 1*scale, y + 10*scale); glVertex2f(x + 5*scale, y + 10*scale); // Bottom horizontal
            glVertex2f(x + 5*scale, y + 1*scale); glVertex2f(x + 5*scale, y + 5.5f*scale); // Top right vertical
            glVertex2f(x + 5*scale, y + 5.5f*scale); glVertex2f(x + 5*scale, y + 10*scale); // Bottom right vertical
            break;
        case 'C': case 'c':
            // Draw letter C
            glVertex2f(x + 6*scale, y + 2*scale); glVertex2f(x + 2*scale, y + 2*scale);  // Top
            glVertex2f(x + 2*scale, y + 2*scale); glVertex2f(x + 2*scale, y + 9*scale); // Left
            glVertex2f(x + 2*scale, y + 9*scale); glVertex2f(x + 6*scale, y + 9*scale); // Bottom
            break;
        case 'D': case 'd':
            // Draw letter D
            glVertex2f(x + 1*scale, y + 1*scale); glVertex2f(x + 1*scale, y + 10*scale); // Left vertical
            glVertex2f(x + 1*scale, y + 1*scale); glVertex2f(x + 5*scale, y + 1*scale);  // Top
            glVertex2f(x + 1*scale, y + 10*scale); glVertex2f(x + 5*scale, y + 10*scale); // Bottom
            glVertex2f(x + 5*scale, y + 1*scale); glVertex2f(x + 6*scale, y + 3*scale);  // Top right curve
            glVertex2f(x + 6*scale, y + 3*scale); glVertex2f(x + 6*scale, y + 8*scale);  // Right vertical
            glVertex2f(x + 6*scale, y + 8*scale); glVertex2f(x + 5*scale, y + 10*scale); // Bottom right curve
            break;
        case 'E': case 'e':
            // Draw letter E
            glVertex2f(x + 1*scale, y + 1*scale); glVertex2f(x + 1*scale, y + 10*scale); // Left vertical
            glVertex2f(x + 1*scale, y + 1*scale); glVertex2f(x + 6*scale, y + 1*scale);  // Top horizontal
            glVertex2f(x + 1*scale, y + 5.5f*scale); glVertex2f(x + 5*scale, y + 5.5f*scale); // Middle horizontal
            glVertex2f(x + 1*scale, y + 10*scale); glVertex2f(x + 6*scale, y + 10*scale); // Bottom horizontal
            break;
        case 'F': case 'f':
            // Draw letter F
            glVertex2f(x + 1*scale, y + 1*scale); glVertex2f(x + 1*scale, y + 10*scale); // Left vertical
            glVertex2f(x + 1*scale, y + 1*scale); glVertex2f(x + 6*scale, y + 1*scale);  // Top horizontal
            glVertex2f(x + 1*scale, y + 5.5f*scale); glVertex2f(x + 5*scale, y + 5.5f*scale); // Middle horizontal
            break;
        case 'G': case 'g':
            // Draw letter G
            glVertex2f(x + 6*scale, y + 2*scale); glVertex2f(x + 2*scale, y + 2*scale);  // Top
            glVertex2f(x + 2*scale, y + 2*scale); glVertex2f(x + 2*scale, y + 9*scale); // Left
            glVertex2f(x + 2*scale, y + 9*scale); glVertex2f(x + 6*scale, y + 9*scale); // Bottom
            glVertex2f(x + 6*scale, y + 9*scale); glVertex2f(x + 6*scale, y + 6*scale); // Right vertical
            glVertex2f(x + 6*scale, y + 6*scale); glVertex2f(x + 4*scale, y + 6*scale); // Inner horizontal
            break;
        case 'H': case 'h':
            // Draw letter H
            glVertex2f(x + 1*scale, y + 1*scale); glVertex2f(x + 1*scale, y + 10*scale); // Left vertical
            glVertex2f(x + 6*scale, y + 1*scale); glVertex2f(x + 6*scale, y + 10*scale); // Right vertical
            glVertex2f(x + 1*scale, y + 5.5f*scale); glVertex2f(x + 6*scale, y + 5.5f*scale); // Cross bar
            break;
        case 'I': case 'i':
            // Draw letter I
            glVertex2f(x + 2*scale, y + 1*scale); glVertex2f(x + 5*scale, y + 1*scale);  // Top horizontal
            glVertex2f(x + 3.5f*scale, y + 1*scale); glVertex2f(x + 3.5f*scale, y + 10*scale); // Vertical
            glVertex2f(x + 2*scale, y + 10*scale); glVertex2f(x + 5*scale, y + 10*scale); // Bottom horizontal
            break;
        case 'K': case 'k':
            // Draw letter K
            glVertex2f(x + 1*scale, y + 1*scale); glVertex2f(x + 1*scale, y + 10*scale); // Left vertical
            glVertex2f(x + 1*scale, y + 5.5f*scale); glVertex2f(x + 6*scale, y + 1*scale); // Upper diagonal
            glVertex2f(x + 1*scale, y + 5.5f*scale); glVertex2f(x + 6*scale, y + 10*scale); // Lower diagonal
            break;
        case 'L': case 'l':
            // Draw letter L
            glVertex2f(x + 1*scale, y + 1*scale); glVertex2f(x + 1*scale, y + 10*scale); // Vertical
            glVertex2f(x + 1*scale, y + 10*scale); glVertex2f(x + 6*scale, y + 10*scale); // Bottom horizontal
            break;
        case 'M': case 'm':
            // Draw letter M
            glVertex2f(x + 1*scale, y + 10*scale); glVertex2f(x + 1*scale, y + 1*scale); // Left vertical
            glVertex2f(x + 1*scale, y + 1*scale); glVertex2f(x + 3.5f*scale, y + 6*scale); // Left diagonal
            glVertex2f(x + 3.5f*scale, y + 6*scale); glVertex2f(x + 6*scale, y + 1*scale); // Right diagonal
            glVertex2f(x + 6*scale, y + 1*scale); glVertex2f(x + 6*scale, y + 10*scale); // Right vertical
            break;
        case 'N': case 'n':
            // Draw letter N
            glVertex2f(x + 1*scale, y + 10*scale); glVertex2f(x + 1*scale, y + 1*scale); // Left vertical
            glVertex2f(x + 1*scale, y + 1*scale); glVertex2f(x + 6*scale, y + 10*scale); // Diagonal
            glVertex2f(x + 6*scale, y + 10*scale); glVertex2f(x + 6*scale, y + 1*scale); // Right vertical
            break;
        case 'O': case 'o':
            // Draw letter O
            glVertex2f(x + 2*scale, y + 2*scale); glVertex2f(x + 5*scale, y + 2*scale);  // Top
            glVertex2f(x + 5*scale, y + 2*scale); glVertex2f(x + 5*scale, y + 9*scale); // Right
            glVertex2f(x + 5*scale, y + 9*scale); glVertex2f(x + 2*scale, y + 9*scale); // Bottom
            glVertex2f(x + 2*scale, y + 9*scale); glVertex2f(x + 2*scale, y + 2*scale); // Left
            break;
        case 'P': case 'p':
            // Draw letter P
            glVertex2f(x + 1*scale, y + 1*scale); glVertex2f(x + 1*scale, y + 10*scale); // Left vertical
            glVertex2f(x + 1*scale, y + 1*scale); glVertex2f(x + 5*scale, y + 1*scale);  // Top horizontal
            glVertex2f(x + 5*scale, y + 1*scale); glVertex2f(x + 5*scale, y + 5.5f*scale); // Top right vertical
            glVertex2f(x + 5*scale, y + 5.5f*scale); glVertex2f(x + 1*scale, y + 5.5f*scale); // Middle horizontal
            break;
        case 'R': case 'r':
            // Draw letter R
            glVertex2f(x + 1*scale, y + 1*scale); glVertex2f(x + 1*scale, y + 10*scale); // Left vertical
            glVertex2f(x + 1*scale, y + 1*scale); glVertex2f(x + 5*scale, y + 1*scale);  // Top horizontal
            glVertex2f(x + 5*scale, y + 1*scale); glVertex2f(x + 5*scale, y + 5.5f*scale); // Top right vertical
            glVertex2f(x + 5*scale, y + 5.5f*scale); glVertex2f(x + 1*scale, y + 5.5f*scale); // Middle horizontal
            glVertex2f(x + 3*scale, y + 5.5f*scale); glVertex2f(x + 6*scale, y + 10*scale); // Diagonal leg
            break;
        case 'S': case 's':
            // Draw letter S
            glVertex2f(x + 6*scale, y + 2*scale); glVertex2f(x + 2*scale, y + 2*scale);  // Top
            glVertex2f(x + 2*scale, y + 2*scale); glVertex2f(x + 2*scale, y + 5.5f*scale); // Upper left
            glVertex2f(x + 2*scale, y + 5.5f*scale); glVertex2f(x + 5*scale, y + 5.5f*scale); // Middle
            glVertex2f(x + 5*scale, y + 5.5f*scale); glVertex2f(x + 5*scale, y + 9*scale); // Lower right
            glVertex2f(x + 5*scale, y + 9*scale); glVertex2f(x + 1*scale, y + 9*scale); // Bottom
            break;
        case 'T': case 't':
            // Draw letter T
            glVertex2f(x + 1*scale, y + 1*scale); glVertex2f(x + 6*scale, y + 1*scale);  // Top horizontal
            glVertex2f(x + 3.5f*scale, y + 1*scale); glVertex2f(x + 3.5f*scale, y + 10*scale); // Vertical
            break;
        case 'U': case 'u':
            // Draw letter U
            glVertex2f(x + 1*scale, y + 1*scale); glVertex2f(x + 1*scale, y + 9*scale); // Left vertical
            glVertex2f(x + 1*scale, y + 9*scale); glVertex2f(x + 6*scale, y + 9*scale); // Bottom
            glVertex2f(x + 6*scale, y + 9*scale); glVertex2f(x + 6*scale, y + 1*scale); // Right vertical
            break;
        case 'V': case 'v':
            // Draw letter V
            glVertex2f(x + 1*scale, y + 1*scale); glVertex2f(x + 3.5f*scale, y + 10*scale); // Left diagonal
            glVertex2f(x + 3.5f*scale, y + 10*scale); glVertex2f(x + 6*scale, y + 1*scale); // Right diagonal
            break;
        case 'W': case 'w':
            // Draw letter W
            glVertex2f(x + 1*scale, y + 1*scale); glVertex2f(x + 2*scale, y + 10*scale); // Left diagonal
            glVertex2f(x + 2*scale, y + 10*scale); glVertex2f(x + 3.5f*scale, y + 6*scale); // Left inner
            glVertex2f(x + 3.5f*scale, y + 6*scale); glVertex2f(x + 5*scale, y + 10*scale); // Right inner
            glVertex2f(x + 5*scale, y + 10*scale); glVertex2f(x + 6*scale, y + 1*scale); // Right diagonal
            break;
        case 'X': case 'x':
            // Draw letter X
            glVertex2f(x + 1*scale, y + 1*scale); glVertex2f(x + 6*scale, y + 10*scale); // Diagonal 1
            glVertex2f(x + 6*scale, y + 1*scale); glVertex2f(x + 1*scale, y + 10*scale); // Diagonal 2
            break;
        case 'Y': case 'y':
            // Draw letter Y
            glVertex2f(x + 1*scale, y + 1*scale); glVertex2f(x + 3.5f*scale, y + 5.5f*scale); // Left diagonal
            glVertex2f(x + 6*scale, y + 1*scale); glVertex2f(x + 3.5f*scale, y + 5.5f*scale); // Right diagonal
            glVertex2f(x + 3.5f*scale, y + 5.5f*scale); glVertex2f(x + 3.5f*scale, y + 10*scale); // Vertical
            break;
        case 'Z': case 'z':
            // Draw letter Z
            glVertex2f(x + 1*scale, y + 1*scale); glVertex2f(x + 6*scale, y + 1*scale);  // Top
            glVertex2f(x + 6*scale, y + 1*scale); glVertex2f(x + 1*scale, y + 10*scale); // Diagonal
            glVertex2f(x + 1*scale, y + 10*scale); glVertex2f(x + 6*scale, y + 10*scale); // Bottom
            break;
        case ':':
            // Draw colon
            glVertex2f(x + 3*scale, y + 3*scale); glVertex2f(x + 4*scale, y + 3*scale);  // Top dot
            glVertex2f(x + 3*scale, y + 8*scale); glVertex2f(x + 4*scale, y + 8*scale);  // Bottom dot
            break;
        case ' ':
            // Space - draw nothing
            break;
        default:
            // Draw a simple rectangle for unknown characters
            glVertex2f(x + 1*scale, y + 2*scale); glVertex2f(x + 6*scale, y + 2*scale);  // Top
            glVertex2f(x + 6*scale, y + 2*scale); glVertex2f(x + 6*scale, y + 9*scale); // Right
            glVertex2f(x + 6*scale, y + 9*scale); glVertex2f(x + 1*scale, y + 9*scale); // Bottom
            glVertex2f(x + 1*scale, y + 9*scale); glVertex2f(x + 1*scale, y + 2*scale); // Left
            break;
    }
    
    glEnd();
}

void UIRenderer::getBitmapFontData(char c, int& width, int& height, unsigned char*& data) {
    // Simplified implementation - not used in the simplified version
    width = CHAR_WIDTH;
    height = CHAR_HEIGHT;
    data = nullptr;
}

void UIRenderer::renderMenuBackground(float x, float y, float width, float height) {
    // Semi-transparent dark background
    glColor4f(0.0f, 0.0f, 0.0f, 0.8f);
    glBegin(GL_QUADS);
    glVertex2f(x, y);
    glVertex2f(x + width, y);
    glVertex2f(x + width, y + height);
    glVertex2f(x, y + height);
    glEnd();
    
    // Border
    glColor3f(0.5f, 0.5f, 0.5f);
    glBegin(GL_LINE_LOOP);
    glVertex2f(x, y);
    glVertex2f(x + width, y);
    glVertex2f(x + width, y + height);
    glVertex2f(x, y + height);
    glEnd();
}

void UIRenderer::renderMenuItem(const std::string& text, float x, float y, bool isHighlighted) {
    if (isHighlighted) {
        // Highlight background
        glColor4f(0.3f, 0.3f, 0.3f, 0.5f);
        glBegin(GL_QUADS);
        glVertex2f(x - 5, y - 2);
        glVertex2f(x + text.length() * CHAR_WIDTH + 5, y - 2);
        glVertex2f(x + text.length() * CHAR_WIDTH + 5, y + CHAR_HEIGHT + 2);
        glVertex2f(x - 5, y + CHAR_HEIGHT + 2);
        glEnd();
    }
    
    renderText(text, x, y);
}
