/**
 * @file UIRenderer.h
 * @brief User Interface rendering system for BlackholeSimulator
 * @author BlackholeSimulator Team
 * @date 2025-08-11
 */

#pragma once

#include <string>
#include <vector>
#include <functional>
#include "Camera.h"

/**
 * @brief Handles UI rendering and interaction in 2D overlay
 * Following Single Responsibility Principle: focused only on UI rendering
 */
class UIRenderer {
public:
    UIRenderer(int windowWidth, int windowHeight);
    ~UIRenderer() = default;

    // Core UI rendering operations
    void beginUIMode();
    void endUIMode();
    void renderText(const std::string& text, float x, float y, float scale = 1.0f);
    void renderPopupMenu(bool showMenu, int mouseX, int mouseY, CameraController::CameraMode currentMode);
    
    // Window management
    void setWindowSize(int width, int height);
    void getWindowSize(int& width, int& height) const;
    
    // Interaction handling
    bool isMouseOverMenu(int mouseX, int mouseY) const;
    CameraController::CameraMode getSelectedCameraMode(int mouseX, int mouseY) const;

private:
    // Text rendering internals
    void drawCharacter(char c, float x, float y, float scale = 1.0f);
    void getBitmapFontData(char c, int& width, int& height, unsigned char*& data);
    
    // Menu rendering
    void renderMenuBackground(float x, float y, float width, float height);
    void renderMenuItem(const std::string& text, float x, float y, bool isHighlighted);
    
    // State management
    int windowWidth, windowHeight;
    
    // Menu configuration
    static constexpr float MENU_WIDTH = 200.0f;
    static constexpr float MENU_HEIGHT = 120.0f;
    static constexpr float MENU_ITEM_HEIGHT = 25.0f;
    static constexpr float MENU_PADDING = 10.0f;
    
    // Character dimensions for bitmap font
    static constexpr int CHAR_WIDTH = 8;
    static constexpr int CHAR_HEIGHT = 12;
    
    // Menu position and state
    mutable float lastMenuX = 0.0f;
    mutable float lastMenuY = 0.0f;
};
