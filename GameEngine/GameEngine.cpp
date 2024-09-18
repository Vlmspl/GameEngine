#include <windows.h>
#include <glad/glad.h>
#include "GLFW/glfw3.h"
#include <iostream>
#include <filesystem>
#include <string>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "Utils.h"
#include "Camera.h"
#include "Object.h"


int main() {
	// Initialize GLFW
	if (!glfwInit()) {
		std::cerr << "Failed to initialize GLFW" << std::endl;
		return -1;
	}

	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_DEPTH_BITS, 24); // Request a 24-bit depth buffer

	// Create a windowed mode window and its OpenGL context
	GLFWwindow* window = glfwCreateWindow(800, 600, "OpenGL Window", NULL, NULL);
	if (!window) {
		glfwTerminate();
		std::cerr << "Failed to create GLFW window" << std::endl;
		return -1;
	}
	

	// Make the window's context current
	glfwMakeContextCurrent(window);
	gladLoadGL();


	Shader VertexShader("Assets/Shaders/TestShader.vert", GL_VERTEX_SHADER);
	Shader FragmentShader("Assets/Shaders/TestShader.frag", GL_FRAGMENT_SHADER);

	ShaderProgram program;
	program.AttachShader(VertexShader);
	program.AttachShader(FragmentShader);
	program.Link();

	const float nearPlane = 0.1f; // Near clipping plane
	const float farPlane = 1000; // Far clipping plane

	glEnable(GL_CULL_FACE);
	glCullFace(GL_BACK); // Cull back faces
	glFrontFace(GL_CCW); // Define counter-clockwise as front

	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LESS);
	glDepthMask(GL_TRUE); // Ensure depth writes are enabled (default)

	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
	glfwSetInputMode(window, GLFW_RAW_MOUSE_MOTION, GLFW_TRUE);

	float deltaTime;
	int width, height;
	float ratio;

	Camera camera = Camera();

	float lastFrame = 0;
	GLuint albedo = loadTexture("Assets/Textures/Albedo.png", GL_TEXTURE0, GL_LINEAR, GL_REPEAT);
	GLuint normal = loadTexture("Assets/Textures/Normal.png", GL_TEXTURE1, GL_LINEAR, GL_REPEAT);
	GLuint roughness = loadTexture("Assets/Textures/Roughness.png", GL_TEXTURE2, GL_LINEAR, GL_REPEAT);

	
	auto Update = [&camera, &ratio, &albedo, &normal, &roughness, &nearPlane, &farPlane](Object& object) {
		glm::mat4 Perspective = glm::perspective(camera.FOV, ratio, nearPlane, farPlane);
		glm::mat4 View = camera.GetViewMatrix();
		glm::mat4 Model = object.getModelMatrix();

		glm::vec3 CameraPos = camera.GetPosition();

		GLint uperspective_location = object.RetrieveUniformAddress("uProjection");
		GLint uview_location = object.RetrieveUniformAddress("uView");
		GLint umodel_location = object.RetrieveUniformAddress("uModel");
		GLint ualbedo_location = object.RetrieveUniformAddress("uAlbedo");
		GLint unormal_location = object.RetrieveUniformAddress("uNormal");
		GLint uroughness_location = object.RetrieveUniformAddress("uRoughness");

		GLint uviewPos_location = object.RetrieveUniformAddress("viewPos");

		glUniformMatrix4fv(uperspective_location, 1, GL_FALSE, glm::value_ptr(Perspective));
		glUniformMatrix4fv(uview_location, 1, GL_FALSE, glm::value_ptr(View));
		glUniformMatrix4fv(umodel_location, 1, GL_FALSE, glm::value_ptr(Model));

		glUniform3fv(uviewPos_location, 1, &CameraPos[0]);

		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, albedo);
		glUniform1i(ualbedo_location, 0);

		glActiveTexture(GL_TEXTURE1);
		glBindTexture(GL_TEXTURE_2D, normal);
		glUniform1i(unormal_location, 1);

		glActiveTexture(GL_TEXTURE2);
		glBindTexture(GL_TEXTURE_2D, roughness);
		glUniform1i(uroughness_location, 2);
	};

	Object object(glm::vec3(0, 0, 0), glm::vec3(0, 0, 0), glm::vec3(1, 1, 1), "Assets/Models/Monkey.model", program);
	object.BindUpdater(Update);

	while (!glfwWindowShouldClose(window)) {
		float currentFrame = glfwGetTime();
		deltaTime = currentFrame - lastFrame;
		lastFrame = currentFrame;

		// Poll for and process events
		glfwPollEvents();

		
		glfwGetFramebufferSize(window, &width, &height);
		ratio = width / (float)height;

		glViewport(0, 0, width, height);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		glClearColor(0, 0.5, 0.5, 1.0);
		

		//Update Stuff:
		glm::vec3 Velocity = GetKeyboardMovement(window, 1);

		glm::vec3 CurrentRotation = camera.GetRotation();
		glm::vec3 NewRotation = CalculateNewRotation(window, CurrentRotation, 0.25);

		camera.setDeltaTime(deltaTime);

		camera.setMoveSpeed(Velocity);
		camera.SetRotation(NewRotation);

		cursor_position_callback(window, (double)width / 2, (double)height / 2);

		object.Render();
		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);


		
		// Swap front and back buffers
		glfwSwapBuffers(window); //unhandled exception: read access violation

	}
	// Cleanup
	glfwDestroyWindow(window);
	glfwTerminate();

	return 0;
}
