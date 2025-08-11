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

void UIRenderer::renderPopupMenu(bool showMenu, int mouseX, int mouseY, CameraMode currentMode) {
    if (!showMenu) return;
    
    float menuX = static_cast<float>(mouseX);
    float menuY = static_cast<float>(mouseY);
    
    // Adjust menu position to stay on screen
    if (menuX + MENU_WIDTH > windowWidth) {
        menuX = windowWidth - MENU_WIDTH;
    }
    if (menuY + MENU_HEIGHT > windowHeight) {
        menuY = windowHeight - MENU_HEIGHT;
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
    if (currentMode == CameraMode::FreeFlight) {
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
    if (currentMode == CameraMode::GameStyle) {
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

CameraMode UIRenderer::getSelectedCameraMode(int mouseX, int mouseY) const {
    if (!isMouseOverMenu(mouseX, mouseY)) {
        return CameraMode::FreeFlight; // Default fallback
    }
    
    float itemY = lastMenuY + MENU_PADDING + 20.0f;
    
    // Check FreeFlight option
    if (mouseY >= itemY && mouseY <= itemY + MENU_ITEM_HEIGHT) {
        return CameraMode::FreeFlight;
    }
    
    // Check GameStyle option
    itemY += MENU_ITEM_HEIGHT + 5.0f;
    if (mouseY >= itemY && mouseY <= itemY + MENU_ITEM_HEIGHT) {
        return CameraMode::GameStyle;
    }
    
    return CameraMode::FreeFlight; // Default fallback
}

void UIRenderer::drawCharacter(char c, float x, float y, float scale) {
    // Simple block-style character rendering using OpenGL quads
    glColor3f(1.0f, 1.0f, 1.0f);
    
    // Define simple character patterns as blocks
    bool shouldDraw = false;
    
    // Simple character mapping - just draw blocks for common characters
    switch (c) {
        case 'A': case 'a':
        case 'B': case 'b':
        case 'C': case 'c':
        case 'D': case 'd':
        case 'E': case 'e':
        case 'F': case 'f':
        case 'G': case 'g':
        case 'H': case 'h':
        case 'I': case 'i':
        case 'J': case 'j':
        case 'K': case 'k':
        case 'L': case 'l':
        case 'M': case 'm':
        case 'N': case 'n':
        case 'O': case 'o':
        case 'P': case 'p':
        case 'Q': case 'q':
        case 'R': case 'r':
        case 'S': case 's':
        case 'T': case 't':
        case 'U': case 'u':
        case 'V': case 'v':
        case 'W': case 'w':
        case 'X': case 'x':
        case 'Y': case 'y':
        case 'Z': case 'z':
            shouldDraw = true;
            break;
        case ':':
            // Draw two small dots
            glBegin(GL_QUADS);
            // Top dot
            glVertex2f(x + 3*scale, y + 2*scale);
            glVertex2f(x + 5*scale, y + 2*scale);
            glVertex2f(x + 5*scale, y + 4*scale);
            glVertex2f(x + 3*scale, y + 4*scale);
            // Bottom dot
            glVertex2f(x + 3*scale, y + 7*scale);
            glVertex2f(x + 5*scale, y + 7*scale);
            glVertex2f(x + 5*scale, y + 9*scale);
            glVertex2f(x + 3*scale, y + 9*scale);
            glEnd();
            return;
        case ' ':
            return; // Space - draw nothing
        default:
            shouldDraw = true; // Draw a placeholder block for unknown characters
            break;
    }
    
    if (shouldDraw) {
        // Draw a simple rectangular character
        glBegin(GL_QUADS);
        glVertex2f(x + scale, y + scale);
        glVertex2f(x + (CHAR_WIDTH-1)*scale, y + scale);
        glVertex2f(x + (CHAR_WIDTH-1)*scale, y + (CHAR_HEIGHT-1)*scale);
        glVertex2f(x + scale, y + (CHAR_HEIGHT-1)*scale);
        glEnd();
    }
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
