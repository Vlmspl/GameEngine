#pragma once

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/constants.hpp>


class Camera {
public:
    float FOV = 90;

    Camera(glm::vec3 position = glm::vec3(0.0f),
        glm::vec3 up = glm::vec3(0.0f, 1.0f, 0.0f),
        glm::vec3 rotation = glm::vec3(0.0f))  // Rotation in radians
        : Position(position), UpVector(up), Rotation(rotation), MoveSpeed(2.5f) {
        Update();
    }

    // Update the camera state
    void Update() {
        RecalculateVectors();
        // Other updates if needed
    }

    // Set the camera position based on speed and delta time
    void setMoveSpeed(glm::vec3 speed) {
        Position += ((ForwardVector * -speed.z) + (RightVector * speed.x) + (UpVector * speed.y))* deltaTime;
    }

    // Set movement speed
    void setMovementSpeed(float speed) {
        MoveSpeed = speed;
    }
    

    // Accessor for view matrix
    glm::mat4 GetViewMatrix() const {
        return glm::lookAt(Position, Position + ForwardVector, UpVector);
    }

    // Set position and rotation
    void SetPosition(const glm::vec3& position) {
        Position = position;
    }

    glm::vec3 GetPosition() {
        return Position;
    }

    void SetRotation(const glm::vec3& rotation) {
        Rotation = rotation;
        RecalculateVectors();
    }

    glm::vec3 GetRotation() {
        return Rotation;
    }
    
    void setDeltaTime(float dt) {
        deltaTime = dt;
    }

private:
    void RecalculateVectors() {
        // Calculate yaw and pitch from the current rotation (stored as vec3)
        float yaw = glm::radians(Rotation.x);  // Left-right (yaw)
        float pitch = glm::radians(Rotation.y);  // Up-down (pitch)

        // Calculate new Forward Vector using yaw and pitch (spherical coordinates to Cartesian)
        glm::vec3 front;
        front.x = cos(pitch) * cos(yaw);
        front.y = sin(pitch);
        front.z = cos(pitch) * sin(yaw);
        ForwardVector = glm::normalize(front);

        // Recalculate the Right Vector (local right, perpendicular to Forward and Up)
        RightVector = glm::normalize(glm::cross(ForwardVector, glm::vec3(0.0f, 1.0f, 0.0f)));

        // Recalculate the UpVector (local up, perpendicular to Right and Forward)
        UpVector = glm::normalize(glm::cross(RightVector, ForwardVector));
    }


    glm::vec3 Position;
    glm::vec3 UpVector = glm::vec3(0, 1, 0);
    glm::vec3 ForwardVector = glm::vec3(0, 0, -1);
    glm::vec3 RightVector = glm::vec3(1, 0, 0);
    glm::vec3 Rotation;

    float MoveSpeed;
    float deltaTime; // Make sure this is updated every frame
};
