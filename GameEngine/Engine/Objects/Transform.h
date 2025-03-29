#pragma once

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/constants.hpp>
#include <cmath>

class Transform {
public:
	Vector3f position;
	Vector3f rotation; // In Euler angles (pitch, yaw, roll)
	Vector3f scale;

	Transform(Vector3f position, Vector3f rotation, Vector3f scale)
		: position(position), rotation(rotation), scale(scale) {}
	Transform() {
		position = Vector3f();
		rotation = Vector3f();
		scale = Vector3f();
	}

	// Function to return the model matrix (Matrix4d)
	Matrix4d GetModelMatrix() const {
		// Create the transformation matrix (identity matrix)
		Matrix4d transform = Matrix4d(1.0f);

		// Apply scaling
		transform = glm::scale(transform, scale);

		// Apply rotation (Euler angles in radians)
		transform = glm::rotate(transform, glm::radians(rotation.x), Vector3f(1, 0, 0)); // Rotate around X axis (Pitch)
		transform = glm::rotate(transform, glm::radians(rotation.y), Vector3f(0, 1, 0)); // Rotate around Y axis (Yaw)
		transform = glm::rotate(transform, glm::radians(rotation.z), Vector3f(0, 0, 1)); // Rotate around Z axis (Roll)

		// Apply translation
		transform = glm::translate(transform, position);

		// Cast to Matrix4d (for double precision)
		Matrix4d transform_d = Matrix4d(transform);

		return transform_d;
	}
};


	class CameraTransform {
	public:
		// Camera position and rotation
		Vector3f position;
		Vector3f rotation = Vector3f(0.0f, 0.0f, 0.0f); // (pitch, yaw, roll)

		// Initialize vectors immediately
		Vector3f ForwardVector = Vector3f(0.0f, 0.0f, -1.0f);
		Vector3f RightVector = Vector3f(1.0f, 0.0f, 0.0f);
		Vector3f UpVector = Vector3f(0.0f, 1.0f, 0.0f);

		CameraTransform(Vector3f startPos = Vector3f(0.0f, 0.0f, 0.0f))
			: position(startPos) {
			UpdateCameraVectors();
		}

		void UpdateCameraVectors() {
			float yaw = glm::radians(rotation.x);
			float pitch = glm::radians(rotation.y);


			Vector3f front;
			front.x = cos(pitch) * cos(yaw);
			front.y = sin(pitch);
			front.z = cos(pitch) * sin(yaw);
			ForwardVector = glm::normalize(front);

			RightVector = glm::normalize(glm::cross(ForwardVector, glm::vec3(0.0f, 1.0f, 0.0f)));

			UpVector = glm::normalize(glm::cross(RightVector, ForwardVector));
		}


		// Get the camera's transformation matrix
		glm::mat4 GetViewMatrix() const {
			return glm::lookAt(position, position + ForwardVector, UpVector);
		}

		Matrix4f GetViewMatrixNoTranslation() const {
			return glm::lookAt(Vector3f(0.0f), ForwardVector, UpVector);
		}
	};

