#pragma once

#include <glm/glm.hpp>
#include "Transform.h"
#include "../Device/Screen.h"

class Camera {
public:
	CameraTransform transform;
	float fov = 70;
	float nearPlane = 0.05f;
	float farPlane = 1000.0f;

	Camera() {
		transform.UpdateCameraVectors();
	}

	Matrix4f GetViewMatrix() const {
		return transform.GetViewMatrix();
	}
	Matrix4f GetViewMatrixNoTranslation() const {
		return transform.GetViewMatrixNoTranslation();
	}

	Matrix4f GetProjectionMatrix() const {
		return glm::perspective(glm::radians(fov), Screen::GetAspectRatio(), nearPlane, farPlane);
	}

};