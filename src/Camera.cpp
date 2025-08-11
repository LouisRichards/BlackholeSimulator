/**
 * @file Camera.cpp
 * @brief Implementation of camera system classes
 * @author BlackholeSimulator Team
 * @date 2025-08-11
 */

#include "../include/Camera.h"
#include <GL/glew.h>
#include <cmath>
#include <algorithm>

#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif

// FreeFlightCamera Implementation
FreeFlightCamera::FreeFlightCamera(float speed, float sensitivity)
    : distance(800.0f), angleX(30.0f), angleY(-15.0f),
      posX(0.0f), posY(0.0f), posZ(0.0f),
      speed(speed), mouseSensitivity(sensitivity) {}

void FreeFlightCamera::applyTransform() const {
    glTranslatef(0.0f, 0.0f, -distance);
    glRotatef(angleX, 1.0f, 0.0f, 0.0f);
    glRotatef(angleY, 0.0f, 1.0f, 0.0f);
    glTranslatef(posX, posY, posZ);
}

void FreeFlightCamera::updateFromMouse(float deltaX, float deltaY, bool isMousePressed) {
    if (isMousePressed) {
        angleY += deltaX * mouseSensitivity;
        angleX += deltaY * mouseSensitivity;
        
        // Clamp vertical rotation
        angleX = std::max(-89.0f, std::min(89.0f, angleX));
        
        // Wrap horizontal rotation
        if (angleY > 360.0f) angleY -= 360.0f;
        if (angleY < -360.0f) angleY += 360.0f;
    }
}

void FreeFlightCamera::updateFromKeyboard(float forward, float right, float up) {
    float radY = angleY * M_PI / 180.0f;
    float radX = angleX * M_PI / 180.0f;
    
    // Forward/backward movement
    posX += forward * sin(radY) * cos(radX) * speed;
    posY += forward * sin(radX) * speed;
    posZ += forward * cos(radY) * cos(radX) * speed;
    
    // Left/right strafe movement
    posX += right * cos(radY) * speed;
    posZ -= right * sin(radY) * speed;
    
    // Up/down movement
    posY += up * speed;
    
    // Zoom control
    if (forward != 0.0f) {
        distance -= forward * speed * 5.0f;
        distance = std::max(50.0f, std::min(2000.0f, distance));
    }
}

std::shared_ptr<ICamera> FreeFlightCamera::clone() const {
    auto camera = std::make_shared<FreeFlightCamera>(speed, mouseSensitivity);
    camera->distance = distance;
    camera->angleX = angleX;
    camera->angleY = angleY;
    camera->posX = posX;
    camera->posY = posY;
    camera->posZ = posZ;
    return camera;
}

void FreeFlightCamera::syncStateFrom(const ICamera& other) {
    // Try to cast to both camera types and sync appropriately
    if (const auto* freeCamera = dynamic_cast<const FreeFlightCamera*>(&other)) {
        distance = freeCamera->distance;
        angleX = freeCamera->angleX;
        angleY = freeCamera->angleY;
        posX = freeCamera->posX;
        posY = freeCamera->posY;
        posZ = freeCamera->posZ;
    } else if (const auto* gameCamera = dynamic_cast<const GameStyleCamera*>(&other)) {
        angleY = gameCamera->getYaw();
        angleX = gameCamera->getPitch();
        posX = gameCamera->getPosX();
        posZ = gameCamera->getPosZ();
        posY = -(gameCamera->getEyeHeight() - 100.0f);
    }
}

// GameStyleCamera Implementation
GameStyleCamera::GameStyleCamera(float speed, float sensitivity)
    : yaw(0.0f), pitch(30.0f), posX(0.0f), posZ(0.0f),
      eyeHeight(100.0f), speed(speed), mouseSensitivity(sensitivity) {}

void GameStyleCamera::applyTransform() const {
    glRotatef(-pitch, 1.0f, 0.0f, 0.0f);
    glRotatef(-yaw, 0.0f, 1.0f, 0.0f);
    glTranslatef(-posX, -(eyeHeight), -posZ);
}

void GameStyleCamera::updateFromMouse(float deltaX, float deltaY, bool isMousePressed) {
    if (isMousePressed) {
        yaw += deltaX * mouseSensitivity;
        pitch += deltaY * mouseSensitivity;
        
        // Clamp pitch
        pitch = std::max(-89.0f, std::min(89.0f, pitch));
        
        // Wrap yaw
        if (yaw > 360.0f) yaw -= 360.0f;
        if (yaw < -360.0f) yaw += 360.0f;
    }
}

void GameStyleCamera::updateFromKeyboard(float forward, float right, float up) {
    float radYaw = yaw * M_PI / 180.0f;
    
    // Forward/backward relative to look direction
    posX += forward * sin(radYaw) * speed;
    posZ += forward * cos(radYaw) * speed;
    
    // Left/right strafe movement
    posX += right * cos(radYaw) * speed;
    posZ -= right * sin(radYaw) * speed;
    
    // Adjust eye height
    eyeHeight += up * speed;
    eyeHeight = std::max(10.0f, std::min(500.0f, eyeHeight));
}

std::shared_ptr<ICamera> GameStyleCamera::clone() const {
    auto camera = std::make_shared<GameStyleCamera>(speed, mouseSensitivity);
    camera->yaw = yaw;
    camera->pitch = pitch;
    camera->posX = posX;
    camera->posZ = posZ;
    camera->eyeHeight = eyeHeight;
    return camera;
}

void GameStyleCamera::syncStateFrom(const ICamera& other) {
    if (const auto* gameCamera = dynamic_cast<const GameStyleCamera*>(&other)) {
        yaw = gameCamera->yaw;
        pitch = gameCamera->pitch;
        posX = gameCamera->posX;
        posZ = gameCamera->posZ;
        eyeHeight = gameCamera->eyeHeight;
    } else if (const auto* freeCamera = dynamic_cast<const FreeFlightCamera*>(&other)) {
        yaw = freeCamera->getAngleY();
        pitch = freeCamera->getAngleX();
        posX = freeCamera->getPosX();
        posZ = freeCamera->getPosZ();
        eyeHeight = std::max(10.0f, std::min(500.0f, -freeCamera->getPosY() + 100.0f));
    }
}

// CameraController Implementation
CameraController::CameraController()
    : currentMode(CameraMode::FreeFlight) {
    freeFlightCamera = std::make_shared<FreeFlightCamera>();
    gameStyleCamera = std::make_shared<GameStyleCamera>();
    currentCamera = freeFlightCamera;
}

void CameraController::applyTransform() const {
    currentCamera->applyTransform();
}

void CameraController::updateFromMouse(float deltaX, float deltaY, bool isMousePressed) {
    currentCamera->updateFromMouse(deltaX, deltaY, isMousePressed);
}

void CameraController::updateFromKeyboard(float forward, float right, float up) {
    currentCamera->updateFromKeyboard(forward, right, up);
}

void CameraController::switchMode(CameraMode mode) {
    if (mode == currentMode) return;
    
    synchronizeCameraStates();
    
    currentMode = mode;
    currentCamera = (mode == CameraMode::FreeFlight) ? freeFlightCamera : gameStyleCamera;
}

void CameraController::synchronizeCameraStates() {
    if (currentMode == CameraMode::FreeFlight) {
        gameStyleCamera->syncStateFrom(*freeFlightCamera);
    } else {
        freeFlightCamera->syncStateFrom(*gameStyleCamera);
    }
}
