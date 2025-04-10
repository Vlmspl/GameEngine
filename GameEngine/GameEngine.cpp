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
	glEnable(GL_DEBUG_OUTPUT_SYNCHRONOUS);
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

	File* textureFile = File::find("Assets/Textures/Test.jpg");
	Texture* texture = new Texture(*textureFile, GL_TEXTURE0);

	Camera camera;
	camera.transform.position = Vector3f(-5, 0, 0);

	UniformValue perspectiveMatrix("projectionMatrix",
		[&camera](Object* obj, ShaderProgram* shader_program) {
			return camera.GetProjectionMatrix();
		});
	UniformValue viewMatrix("viewMatrix",
		[&camera](Object* obj, ShaderProgram* shader_program) {
			return camera.GetViewMatrix();
		});
	UniformValue modelMatrix("modelMatrix",
		[](Object* obj, ShaderProgram* shader_program) {
			return obj->transform.GetModelMatrix();
		});
	UniformValue sampleTexture("Texture", [texture](Object* obj, ShaderProgram* shader_program) {
		texture->bind();
		return (GLuint) 0;
	});
	Material material(&TestProgram, perspectiveMatrix, viewMatrix, modelMatrix, sampleTexture);


	Scene scene;

	File* meshFile = File::find("Assets/Models/Test.obj");
	Mesh mesh = MeshLoader::LoadMesh(meshFile, MeshFormat::OBJ, VertexFormat::PositionUvNormal);

	Object* object = Instance::Create<Object>(scene, "TestObject", &material);
	object->SetMesh(&mesh);

	Object* object1 = Instance::Create<Object>(scene, "TestObject1", &material);
	object1->transform.position = Vector3f(3.0f, 0, 0);
	object1->SetMesh(&mesh);

	Object* object2 = Instance::Create<Object>(scene, "TestObject2", &material);
	object2->transform.position = Vector3f(6.0f, 0, 0);
	object2->SetMesh(&mesh);






	Input::Initialize(window);

	CameraController cameraController(&camera);
	cameraController.Run();

	glfwSwapInterval(0);

	glEnable(GL_DEPTH_TEST);
	glEnable(GL_CULL_FACE);
	glCullFace(GL_BACK);
	glFrontFace(GL_CCW);

	Renderer::Setup(window);

	double lastTime = glfwGetTime();  // Get the current time at the start
	
	while (!glfwWindowShouldClose(window)) {
		Input::Mouse::Update();
		Renderer::Render(&scene, &camera);

		double currentTime = glfwGetTime();  // Get the current time during each frame
		if (currentTime - lastTime >= 1.0) {  // If 1 second has passed
			glfwSetWindowTitle(window, ("3D Engine, FPS: " + std::to_string(1.0 / Time::deltaTime)).c_str());
			lastTime = currentTime;  // Reset the timer
		}
	}

	glfwDestroyWindow(window);
	glfwTerminate();

	return SUCCESS;
}
