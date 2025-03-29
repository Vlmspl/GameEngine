#pragma once
#include <glfw/glfw3.h>

class Time {
public:
	static float deltaTime;
	static float lastFrameTime;

	static void Update() {
		float currentFrameTime = static_cast<float>(glfwGetTime());
		deltaTime = currentFrameTime - lastFrameTime;
		lastFrameTime = currentFrameTime;
	}
};

float Time::deltaTime = 0.0f;
float Time::lastFrameTime = 0.0f;
