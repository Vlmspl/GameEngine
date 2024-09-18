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

	static const Vertex vertices[] = {
		{ {2.0f, 0.0f, 0.0f}, {0.0f, 0.0f}, {0.0f, 0.0f, 1.0f} },  // Bottom-left
		{ {3.0f, 0.0f, 0.0f}, {1.0f, 0.0f}, {0.0f, 0.0f, 1.0f} },  // Bottom-right
		{ {3.0f, 1.0f, 0.0f}, {1.0f, 1.0f}, {0.0f, 0.0f, 1.0f} },  // Top-right
		{ {2.0f, 1.0f, 0.0f}, {0.0f, 1.0f}, {0.0f, 0.0f, 1.0f} }   // Top-left
	};

	GLuint indices[] = {
		0, 1, 2,  // First triangle: bottom-left, bottom-right, top-right
		0, 2, 3   // Second triangle: bottom-left, top-right, top-left
	};

	VBO VertexBufferObject(vertices, sizeof(vertices));


	Shader VertexShader("Assets/Shaders/TestShader.vert", GL_VERTEX_SHADER);
	Shader FragmentShader("Assets/Shaders/TestShader.frag", GL_FRAGMENT_SHADER);

	ShaderProgram program;
	program.AttachShader(VertexShader);
	program.AttachShader(FragmentShader);
	program.Link();
	program.use();

	GLint uprojection_location = program.GetUniformLocation("u_projection");
	GLint uview_location = program.GetUniformLocation("u_view");
	GLint umodel_location = program.GetUniformLocation("u_model");
	GLint utexture_location = program.GetUniformLocation("u_texture");
	GLint uViewPos_location = program.GetUniformLocation("u_viewPos");

	//Material Struct
	Material material;
	material.ambient = glm::vec3(0.2f, 0.2f, 0.2f);  // Low intensity, grayish base color
	material.diffuse = glm::vec3(0.8f, 0.8f, 0.8f);  // White/Gray diffuse color
	material.specular = glm::vec3(1.0f, 1.0f, 1.0f); // Full white specular color
	material.shininess = 32.0f;  // Typical value for smooth surfaces

	GLint materialAmbient_location = program.GetUniformLocation("Material.ambient");
	GLint materialDiffuse_location = program.GetUniformLocation("Material.diffuse");
	GLint materialSpecular_location = program.GetUniformLocation("Material.specular");
	GLint materialShininess_location = program.GetUniformLocation("Material.shininess");
	//-----------
	
	//Light Struct
	Light light;
	light.position = glm::vec3(10.0f, 10.0f, 10.0f); // Light source position (e.g., above and to the side)
	light.ambient = glm::vec3(0.2f, 0.2f, 0.2f);     // Low intensity, grayish light
	light.diffuse = glm::vec3(1.0f, 1.0f, 1.0f);     // Full intensity white light
	light.specular = glm::vec3(1.0f, 1.0f, 1.0f);    // Full intensity white specular light


	GLint lightPosition_location = program.GetUniformLocation("Light[0].position");
	GLint lightAmbient_location = program.GetUniformLocation("Light[0].ambient");
	GLint lightDiffuse_location = program.GetUniformLocation("Light[0].diffuse");
	GLint lightSpecular_location = program.GetUniformLocation("Light[0].specular");
	//-----------

	GLint pos_location = program.GetAttributeLocation("vPos");
	GLint uv_location = program.GetAttributeLocation("vUv");
	GLint normal_location = program.GetAttributeLocation("vNormal");

	VAO VertexArrayObject;
	VertexArrayObject.Bind();
	VertexArrayObject.EnableVertexAttributeArray(pos_location);
	VertexArrayObject.VertexAttrributePointer(pos_location, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, pos));

	VertexArrayObject.EnableVertexAttributeArray(uv_location);
	VertexArrayObject.VertexAttrributePointer(uv_location, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, uv));

	VertexArrayObject.EnableVertexAttributeArray(normal_location);
	VertexArrayObject.VertexAttrributePointer(normal_location, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, normal));


	EBO ElementBufferObject(indices, sizeof(indices));



	const float nearPlane = 0.1f; // Near clipping plane
	const float farPlane = 10000.0f; // Far clipping plane

	// Main loop

	//glEnable(GL_CULL_FACE);
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
	GLuint texture = loadTexture("Assets/Textures/texture.png", GL_TEXTURE0, GL_NEAREST, GL_REPEAT);

	
	auto Update = [&camera, &ratio, &texture, &nearPlane, &farPlane](Object& object) {
		glm::mat4 Perspective = glm::perspective(camera.FOV, ratio, nearPlane, farPlane);
		glm::mat4 View = camera.GetViewMatrix();

		GLint uperspective_location = object.RetrieveUniformAddress("u_projection");
		GLint uview_location = object.RetrieveUniformAddress("u_view");
		GLint utexture_location = object.RetrieveUniformAddress("u_texture");

		glUniformMatrix4fv(uperspective_location, 1, GL_FALSE, glm::value_ptr(Perspective));
		glUniformMatrix4fv(uview_location, 1, GL_FALSE, glm::value_ptr(View));

		glBindTexture(GL_TEXTURE_2D, texture);
		glUniform1i(utexture_location, 0);
	};

	Object object(glm::vec3(0, 0, 0), glm::vec3(0, 0, 0), glm::vec3(1, 1, 1), "Assets/Models/Monkey.model", program);
	object.BindUpdater(Update);

	while (!glfwWindowShouldClose(window)) {
		float currentFrame = glfwGetTime();
		deltaTime = currentFrame - lastFrame;
		lastFrame = currentFrame;

		std::cout << "\nFPS: " << 1.0f / deltaTime;

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

		//program.use();

		glm::mat4 Perspective = glm::perspective(camera.FOV, ratio, nearPlane, farPlane);
		glUniformMatrix4fv(uprojection_location, 1, GL_FALSE, glm::value_ptr(Perspective));

		glm::mat4 View = camera.GetViewMatrix();
		glUniformMatrix4fv(uview_location, 1, GL_FALSE, glm::value_ptr(View));
		
		glBindTexture(GL_TEXTURE_2D, texture);
		glUniform1i(utexture_location, 0);

		//PhongLightning Uniforms:
		//Material:
		glUniform3fv(materialAmbient_location, 1, &material.ambient[0]);
		glUniform3fv(materialDiffuse_location, 1, &material.diffuse[0]);
		glUniform3fv(materialSpecular_location, 1, &material.specular[0]);
		glUniform1f(materialShininess_location, material.shininess);
		
		//Light:
		glUniform3fv(lightPosition_location, 1, &light.position[0]);
		glUniform3fv(lightAmbient_location, 1, &light.ambient[0]);
		glUniform3fv(lightDiffuse_location, 1, &light.diffuse[0]);
		glUniform3fv(lightSpecular_location, 1, &light.specular[0]);

		//Other:
		glm::vec3 cameraPos = camera.GetPosition();
		glUniform3fv(uViewPos_location, 1, &cameraPos[0]);
		//----------

		

		VertexArrayObject.Bind();
		VertexBufferObject.Bind();
		ElementBufferObject.Bind();
		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);


		
		// Swap front and back buffers
		glfwSwapBuffers(window); //unhandled exception: read access violation

	}
	// Cleanup
	glfwDestroyWindow(window);
	glfwTerminate();

	return 0;
}
