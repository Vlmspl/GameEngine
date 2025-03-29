#pragma once
#include <GLFW/glfw3.h>

class Screen {
public:
	// Static pointers for width and height
	static int width;
	static int height;

	static void Update(GLFWwindow* window) {
		glfwGetFramebufferSize(window, &width, &height);
	}

	// Static function to get the aspect ratio
	static float GetAspectRatio() {
		return static_cast<float>(width) / static_cast<float>(height);
	}
};

int Screen::width = 800;
int Screen::height = 600;