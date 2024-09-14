#pragma once

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/constants.hpp>

class Camera {
public:
    Camera(glm::vec3 position = glm::vec3(0.0f),
        glm::vec3 up = glm::vec3(0.0f, 1.0f, 0.0f),
        glm::vec3 rotation = glm::vec3(0.0f))  // Rotation in radians
        : Position(position), UpVector(up), Rotation(rotation) {
        RecalculateVectors();
    }

    glm::mat4 GetViewMatrix() const {
        return glm::lookAt(Position, Position + ForwardVector, UpVector);
    }

    void Update() {
        RecalculateVectors();
        //TODO: Add more stuff to Update if needed
    }

    glm::vec3 Position = glm::vec3(0.0f, 0.0f, 0.0f);
    glm::vec3 UpVector = glm::vec3(0.0f, 1.0f, 0.0f);
    glm::vec3 ForwardVector = glm::vec3(0.0f, 0.0f, -1.0f);
    glm::vec3 RightVector = glm::vec3(1.0f, 0.0f, 0.0f);

    glm::vec3 Rotation;

    void SetPosition(const glm::vec3& position) {
        Position = position;
    }

    void SetRotation(const glm::vec3& rotation) {
        Rotation = rotation;
        RecalculateVectors();
    }

private:
    void RecalculateVectors() {
        float roll = Rotation.z;
        float pitch = Rotation.y;
        float yaw = Rotation.x;

        glm::vec3 front;
        front.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
        front.y = sin(glm::radians(pitch));
        front.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));
        ForwardVector = glm::normalize(front);

        RightVector = glm::normalize(glm::cross(ForwardVector, UpVector));
        UpVector = glm::normalize(glm::cross(RightVector, ForwardVector));

        float rollMatrix[3][3] = {
            {cos(roll), -sin(roll), 0.0f},
            {sin(roll), cos(roll), 0.0f},
            {0.0f, 0.0f, 1.0f}
        };

        glm::mat3 rollMat = glm::make_mat3(rollMatrix);
        UpVector = rollMat * UpVector;
        RightVector = rollMat * RightVector;
        ForwardVector = rollMat * ForwardVector;
    }
};
