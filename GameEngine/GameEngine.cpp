#define STB_IMAGE_IMPLEMENTATION
#include "Utilities.h"
#include <iostream>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include "Engine/Core.h"
#include "Engine/FileSystem.h"
#include "Engine/Objects.h"
#include "Engine/Device.h"
#include "Engine/Renderer.h"
#include "Scripts/ScriptBehaviour.h"

#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtx/string_cast.hpp>

class CameraController : public ScriptBehaviour {
public:
	CameraController(Camera* camera) : camera(camera) {};
	~CameraController() = default;

	void Run() override {
		Input::Mouse::SetMouseLock(true);
		Event<>::Connection updateConnection = Renderer::BeforeRender.OnEvent([&]() {
			// Get mouse delta for rotation
			float mouseDeltaX, mouseDeltaY;
			Input::Mouse::GetDelta(mouseDeltaX, mouseDeltaY);

			// Camera sensitivity (adjust as necessary)
			const float sensitivity = 0.1f;

			// Apply mouse delta to rotate the camera (invert Y-axis for better control)
			camera->transform.rotation.x += mouseDeltaX * sensitivity; // Yaw (left-right)
			camera->transform.rotation.y -= mouseDeltaY * sensitivity; // Pitch (up-down)

			// Update camera vectors based on the new rotation
			camera->transform.UpdateCameraVectors();

			// Camera movement using keys (W, S, A, D, Q, E)
			float deltaTime = Time::deltaTime;

			// Movement with WASDQE keys
			if (Input::KeyPressed(KeyCode::Key_W)) {
				camera->transform.position += camera->transform.ForwardVector * deltaTime;
			}
			if (Input::KeyPressed(KeyCode::Key_S)) {
				camera->transform.position -= camera->transform.ForwardVector * deltaTime;
			}
			if (Input::KeyPressed(KeyCode::Key_A)) {
				camera->transform.position -= camera->transform.RightVector * deltaTime;
			}
			if (Input::KeyPressed(KeyCode::Key_D)) {
				camera->transform.position += camera->transform.RightVector * deltaTime;
			}
			if (Input::KeyPressed(KeyCode::Key_Q)) {
				camera->transform.position -= camera->transform.UpVector * deltaTime;
			}
			if (Input::KeyPressed(KeyCode::Key_E)) {
				camera->transform.position += camera->transform.UpVector * deltaTime;
			}
		});
	}
private:
	Camera* camera;
};

Camera* camera = nullptr;


GLFWwindow* initWindow(const int width, const int height, const char* title) {
	if (!glfwInit()) {
		std::cerr << "Failed to initialize GLFW!\n";
		return nullptr;
	}

	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);

	GLFWwindow* window = glfwCreateWindow(width, height, title, nullptr, nullptr);
	if (!window) {
		std::cerr << "Failed to open GLFW window.\n";
		glfwTerminate();
		return nullptr;
	}

	glfwMakeContextCurrent(window);

	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
		std::cerr << "Failed to initialize GLAD!" << std::endl;
		glfwDestroyWindow(window);
		glfwTerminate();
		return nullptr;
	}

	glEnable(GL_DEBUG_OUTPUT);
	glDebugMessageCallback([](GLenum source, GLenum type, GLuint id, GLenum severity, GLsizei length, const GLchar* message, const void* userParam) {
	// Simply log the message along with its source, type, and severity for context
	std::cout << "OpenGL Debug [Source: " << source << ", Type: " << type
			  << ", ID: " << id << ", Severity: " << severity << "]: " << message << std::endl;
	}, nullptr);


	return window;
}

int main() {
	GLFWwindow* window = initWindow(800, 600, "3D engine");
	if (!window) {
		return WINDOW_INITIALIZATION_FAIL;
	}

	File* TestFragmentShaderFile = File::find("Assets/Shaders/Test.frag");
	Shader* TestFragmentShader = new Shader(GL_FRAGMENT_SHADER, *TestFragmentShaderFile);

	File* TestVertexShaderFile = File::find("Assets/Shaders/Test.vert");
	Shader* TestVertexShader = new Shader(GL_VERTEX_SHADER, *TestVertexShaderFile);

	ShaderProgram TestProgram;
	TestProgram.AttachShader(*TestVertexShader);
	TestProgram.AttachShader(*TestFragmentShader);
	TestProgram.LinkProgram();

	float vertices[] = {
		// Position         // Texture Coordinates
		-0.5f,  0.5f, -1.0f,  0.0f, 1.0f,  // Top-left
		 0.5f,  0.5f, -1.0f,  1.0f, 1.0f,  // Top-right
		 0.5f, -0.5f, -1.0f,  1.0f, 0.0f,  // Bottom-right
		-0.5f, -0.5f, -1.0f,  0.0f, 0.0f   // Bottom-left
	};

	unsigned int indices[] = {
		0, 1, 2,  // First triangle
		0, 2, 3   // Second triangle
	};


	VertexBuffer vertexBuffer;
	vertexBuffer.SetData(vertices, sizeof(vertices), GL_STATIC_DRAW);

	ElementBuffer elementBuffer;
	elementBuffer.SetData(indices, sizeof(indices), GL_STATIC_DRAW);


	VertexArray vertexArray;
	vertexArray.AddVertexBuffer(vertexBuffer, VertexFormat::PositionUv);
	vertexArray.AddIndexBuffer(elementBuffer);

	File* textureFile = File::find("Assets/Textures/Test.png");
	Texture* texture = new Texture(*textureFile, GL_TEXTURE0);

	Camera camera;
	camera.transform.position = Vector3f(0, 0, -2);

	Input::Initialize(window);

	CameraController cameraController(&camera);
	cameraController.Run();

	glfwSwapInterval(1);

	glEnable(GL_DEPTH_TEST);

	Renderer::Setup(window, &vertexBuffer, &elementBuffer, &vertexArray, texture, &TestProgram);

	while (!glfwWindowShouldClose(window)) {
		Input::Mouse::Update();



		Renderer::Render(&camera);
	}

	glfwDestroyWindow(window);
	glfwTerminate();

	return SUCCESS;
}
