/**
 * @file Camera.h
 * @brief Camera system for 3D navigation
 * @author BlackholeSimulator Team
 * @date 2025-08-11
 */

#pragma once
#include <memory>

/**
 * @interface ICamera
 * @brief Abstract interface for camera implementations
 * 
 * Defines the contract for camera systems that can be applied to OpenGL transforms.
 * Follows the Interface Segregation Principle by providing only camera-specific methods.
 */
class ICamera {
public:
    virtual ~ICamera() = default;
    
    /**
     * @brief Applies camera transformations to the current OpenGL matrix
     */
    virtual void applyTransform() const = 0;
    
    /**
     * @brief Updates camera based on mouse input
     * @param deltaX Mouse movement in X direction
     * @param deltaY Mouse movement in Y direction
     * @param isMousePressed Whether mouse button is pressed
     */
    virtual void updateFromMouse(float deltaX, float deltaY, bool isMousePressed) = 0;
    
    /**
     * @brief Updates camera based on keyboard input
     * @param forward Forward/backward movement
     * @param right Left/right movement
     * @param up Up/down movement
     */
    virtual void updateFromKeyboard(float forward, float right, float up) = 0;
    
    /**
     * @brief Creates a copy of the camera state for mode switching
     * @return Shared pointer to a new camera instance with copied state
     */
    virtual std::shared_ptr<ICamera> clone() const = 0;
    
    /**
     * @brief Synchronizes camera state from another camera
     * @param other Camera to copy state from
     */
    virtual void syncStateFrom(const ICamera& other) = 0;
};

/**
 * @class FreeFlightCamera
 * @brief Free 6-DOF camera implementation
 * 
 * Provides unrestricted movement in 3D space similar to space simulators.
 * Follows the Single Responsibility Principle by handling only free-flight camera logic.
 */
class FreeFlightCamera : public ICamera {
public:
    FreeFlightCamera(float speed = 10.0f, float sensitivity = 0.5f);
    
    void applyTransform() const override;
    void updateFromMouse(float deltaX, float deltaY, bool isMousePressed) override;
    void updateFromKeyboard(float forward, float right, float up) override;
    std::shared_ptr<ICamera> clone() const override;
    void syncStateFrom(const ICamera& other) override;
    
    // Accessors for state synchronization
    float getDistance() const { return distance; }
    float getAngleX() const { return angleX; }
    float getAngleY() const { return angleY; }
    float getPosX() const { return posX; }
    float getPosY() const { return posY; }
    float getPosZ() const { return posZ; }
    
    void setDistance(float d) { distance = d; }
    void setAngleX(float a) { angleX = a; }
    void setAngleY(float a) { angleY = a; }
    void setPosX(float p) { posX = p; }
    void setPosY(float p) { posY = p; }
    void setPosZ(float p) { posZ = p; }

private:
    float distance;
    float angleX, angleY;
    float posX, posY, posZ;
    float speed;
    float mouseSensitivity;
};

/**
 * @class GameStyleCamera
 * @brief Ground-relative FPS-style camera implementation
 * 
 * Provides video game style camera controls with ground-relative movement.
 * Follows the Single Responsibility Principle by handling only game-style camera logic.
 */
class GameStyleCamera : public ICamera {
public:
    GameStyleCamera(float speed = 10.0f, float sensitivity = 0.5f);
    
    void applyTransform() const override;
    void updateFromMouse(float deltaX, float deltaY, bool isMousePressed) override;
    void updateFromKeyboard(float forward, float right, float up) override;
    std::shared_ptr<ICamera> clone() const override;
    void syncStateFrom(const ICamera& other) override;
    
    // Accessors for state synchronization
    float getYaw() const { return yaw; }
    float getPitch() const { return pitch; }
    float getEyeHeight() const { return eyeHeight; }
    float getPosX() const { return posX; }
    float getPosZ() const { return posZ; }
    
    void setYaw(float y) { yaw = y; }
    void setPitch(float p) { pitch = p; }
    void setEyeHeight(float h) { eyeHeight = h; }
    void setPosX(float p) { posX = p; }
    void setPosZ(float p) { posZ = p; }

private:
    float yaw, pitch;
    float posX, posZ;
    float eyeHeight;
    float speed;
    float mouseSensitivity;
};

/**
 * @class CameraController
 * @brief Manages camera instances and mode switching
 * 
 * Follows the Single Responsibility Principle by handling only camera management.
 * Follows the Open/Closed Principle by allowing new camera types through polymorphism.
 */
class CameraController {
public:
    enum class CameraMode {
        FreeFlight,
        GameStyle
    };
    
    CameraController();
    
    /**
     * @brief Applies the current camera's transform
     */
    void applyTransform() const;
    
    /**
     * @brief Updates current camera from mouse input
     */
    void updateFromMouse(float deltaX, float deltaY, bool isMousePressed);
    
    /**
     * @brief Updates current camera from keyboard input
     */
    void updateFromKeyboard(float forward, float right, float up);
    
    /**
     * @brief Switches camera mode while preserving position
     * @param mode Target camera mode
     */
    void switchMode(CameraMode mode);
    
    /**
     * @brief Gets current camera mode
     * @return Current camera mode
     */
    CameraMode getCurrentMode() const { return currentMode; }

private:
    std::shared_ptr<ICamera> freeFlightCamera;
    std::shared_ptr<ICamera> gameStyleCamera;
    std::shared_ptr<ICamera> currentCamera;
    CameraMode currentMode;
    
    /**
     * @brief Synchronizes camera states when switching modes
     */
    void synchronizeCameraStates();
};
