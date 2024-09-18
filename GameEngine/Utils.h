#pragma once
#include <fstream>
#define STB_IMAGE_IMPLEMENTATION
#include <stb/stb_image.h>
#include <vector>
#include <unordered_map>
#include <functional>

//Read Shader Code from Path
std::string ReadShaderCode(const std::string& path) {
	std::ifstream file(path, std::ios::in | std::ios::binary);

	if (!file.is_open()) {
		throw std::exception("Issue while reading shader file");
		return "";
	}

	std::string content((std::istreambuf_iterator<char>(file)), std::istreambuf_iterator<char>());

	file.close();

	return content;
}

//Get Keyboard Movement for Camera Controls
glm::vec3 GetKeyboardMovement(GLFWwindow* window, float moveSpeed) {
	glm::vec3 movement(0.0f);

	// Check for key presses and update the movement vector
	if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
		movement += glm::vec3(0.0f, 0.0f, -1.0f);  // Forward
	if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
		movement += glm::vec3(0.0f, 0.0f, 1.0f);   // Backward
	if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
		movement += glm::vec3(-1.0f, 0.0f, 0.0f);  // Left
	if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
		movement += glm::vec3(1.0f, 0.0f, 0.0f);   // Right


	return movement * moveSpeed;
}

//Calculate new rotation of Camera
glm::vec3 CalculateNewRotation(GLFWwindow* window, glm::vec3 oldRotation, float sensitivity) {
	double xpos, ypos;
	glfwGetCursorPos(window, &xpos, &ypos);

	static double lastX = xpos;
	static double lastY = ypos;

	double xOffset = xpos - lastX;
	double yOffset = lastY - ypos;  // Inverted for pitch (y-axis)

	lastX = xpos;
	lastY = ypos;

	xOffset *= sensitivity;
	yOffset *= sensitivity;

	oldRotation.x += (float)xOffset;  // Update yaw (left-right)
	oldRotation.y += (float)yOffset;  // Update pitch (up-down)

	// Clamp pitch to prevent camera flip
	if (oldRotation.y > 89.0f) {
		oldRotation.y = 89.0f;
	}
	if (oldRotation.y < -89.0f) {
		oldRotation.y = -89.0f;
	}

	return oldRotation;
}

//Vertex Structure
struct Vertex {
	glm::vec3 pos;    // Position
	glm::vec2 uv;     // UV Coordinates
	glm::vec3 normal; // Normal vector

	// Operator to compare vertices based on position, UV, and normal
	bool operator==(const Vertex& other) const {
		return pos == other.pos && uv == other.uv && normal == other.normal;
	}
};

//Cursor locking
void cursor_position_callback(GLFWwindow* window, double xpos, double ypos) {
	// Store current cursor position
	static double lastX = 0.0, lastY = 0.0;
	double xoffset = xpos - lastX;
	double yoffset = lastY - ypos;

	// Update last cursor position
	lastX = xpos;
	lastY = ypos;

	// Handle input
}


// Function to load image data and create OpenGL texture
GLuint loadTexture(const char* filename, GLenum ActiveTexture, GLint FilterType, GLint RepetitionType) {
	int width, height, numColorCh;
	// Load the image
	unsigned char* bytes = stbi_load(filename, &width, &height, &numColorCh, 0);

	// Check if the image was loaded successfully
	if (bytes) {
		std::cout << "Texture data loaded at address: " << static_cast<void*>(bytes) << "\n";
	}
	else {
		std::cout << "Failed to load texture data\n";
	}


	// Generate and bind the texture
	GLuint texture;
	glGenTextures(1, &texture);
	glActiveTexture(ActiveTexture);
	glBindTexture(GL_TEXTURE_2D, texture);

	// Set texture parameters
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, FilterType);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, FilterType);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, RepetitionType);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, RepetitionType);

	// Determine the internal format based on the number of color channels
	GLenum internalFormat, dataFormat;
	switch (numColorCh) {
	case 1:
		internalFormat = GL_RED;
		dataFormat = GL_RED;
		break;
	case 3:
		internalFormat = GL_RGB;
		dataFormat = GL_RGB;
		break;
	case 4:
		internalFormat = GL_RGBA;
		dataFormat = GL_RGBA;
		break;
	default:
		std::cerr << "Unsupported number of color channels: " << numColorCh << std::endl;
		stbi_image_free(bytes);
		return 0;
	}


	glTexImage2D(GL_TEXTURE_2D, 0, internalFormat, width, height, 0, dataFormat, GL_UNSIGNED_BYTE, bytes);
	glGenerateMipmap(GL_TEXTURE_2D);

	stbi_image_free(bytes);

	glBindTexture(GL_TEXTURE_2D, 0);

	std::cout << "Loaded texture ID: " << texture << std::endl;

	return texture;
}

//Checking Shader Compilation Error
void checkShaderCompileErrors(GLuint shader, GLenum type) {
	GLint success;
	GLchar infoLog[1024];
	glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
	if (!success) {
		glGetShaderInfoLog(shader, 1024, NULL, infoLog);
		std::cout << "ERROR::SHADER_COMPILATION_ERROR of type: " << type << "\n" << infoLog << "\n";
	}
}

// OBJ Parser
void parseOBJ(const std::string& path, Vertex*& vertices, size_t& vertexCount, GLuint*& indices, size_t& indexCount) {
	std::ifstream file(path);
	if (!file.is_open()) {
		std::cerr << "Error opening file: " << path << std::endl;
		return;
	}

	// Temporary storage for positions, UVs, normals, and final vertex data
	std::vector<glm::vec3> tempPositions;
	std::vector<glm::vec2> tempUVs;
	std::vector<glm::vec3> tempNormals;
	std::vector<Vertex> tempVertices;
	std::vector<GLuint> tempIndices;

	std::string line;
	while (std::getline(file, line)) {
		std::istringstream s(line);
		std::string prefix;
		s >> prefix;

		// Parse vertex positions
		if (prefix == "v") {
			glm::vec3 pos;
			s >> pos.x >> pos.y >> pos.z;
			tempPositions.push_back(pos);
		}
		// Parse UV coordinates
		else if (prefix == "vt") {
			glm::vec2 uv;
			s >> uv.x >> uv.y;
			tempUVs.push_back(uv);
		}
		// Parse normals
		else if (prefix == "vn") {
			glm::vec3 normal;
			s >> normal.x >> normal.y >> normal.z;
			tempNormals.push_back(normal);
		}
		// Parse faces (handles position, UV, and normal indices)
		else if (prefix == "f") {
			std::string vertexData;
			std::vector<GLuint> faceIndices;
			while (s >> vertexData) {
				std::replace(vertexData.begin(), vertexData.end(), '/', ' '); // Replace '/' with space
				std::istringstream vertexStream(vertexData);
				GLuint posIndex, uvIndex, normIndex;
				vertexStream >> posIndex >> uvIndex >> normIndex;

				// Handle 1-based indexing
				if (posIndex > 0) posIndex--;
				if (uvIndex > 0) uvIndex--;
				if (normIndex > 0) normIndex--;

				// Check for valid indices and create the vertex
				if (posIndex < tempPositions.size() && uvIndex < tempUVs.size() && normIndex < tempNormals.size()) {
					Vertex vertex;
					vertex.pos = tempPositions[posIndex];
					vertex.uv = tempUVs[uvIndex];
					vertex.normal = tempNormals[normIndex];

					// Avoid duplicates by checking if the vertex already exists
					auto it = std::find(tempVertices.begin(), tempVertices.end(), vertex);
					if (it == tempVertices.end()) {
						tempVertices.push_back(vertex);
						faceIndices.push_back(tempVertices.size() - 1);
					}
					else {
						faceIndices.push_back(std::distance(tempVertices.begin(), it));
					}
				}
			}

			// Assuming faces are triangles or quads; handle accordingly
			if (faceIndices.size() >= 3) {
				for (size_t i = 1; i < faceIndices.size() - 1; ++i) {
					tempIndices.push_back(faceIndices[0]);
					tempIndices.push_back(faceIndices[i]);
					tempIndices.push_back(faceIndices[i + 1]);
				}
			}
		}
	}

	file.close();

	// Allocate and copy data
	vertexCount = tempVertices.size();
	indexCount = tempIndices.size();
	vertices = new Vertex[vertexCount];
	indices = new GLuint[indexCount];
	std::copy(tempVertices.begin(), tempVertices.end(), vertices);
	std::copy(tempIndices.begin(), tempIndices.end(), indices);
}



class Shader {
public:
	GLuint shader;

	Shader(const std::string& Path, GLenum Type) {
		std::string CodeStr = ReadShaderCode(Path);
		shader = glCreateShader(Type);
		const char* Code = CodeStr.c_str();
		glShaderSource(shader, 1, &Code, NULL);
		glCompileShader(shader);
		checkShaderCompileErrors(shader, Type);
	}
};


class ShaderProgram {
public:
	GLuint program;

	ShaderProgram() {
		program = glCreateProgram();
	}

	void AttachShader(const Shader& shader) {
		glAttachShader(program, shader.shader);
	}

	void Link() {
		glLinkProgram(program);
		checkLinkingErrors();
	}

	void use() {
		glUseProgram(program);
	}

	GLint GetUniformLocation(const std::string& name) {
		GLint location = glGetUniformLocation(program, name.c_str());
		if (location == -1) {
			std::cerr << "Warning: Uniform '" << name << "' not found!" << std::endl;
		}
		return location;
	}

	GLint GetAttributeLocation(const std::string& name) {
		GLint location = glGetAttribLocation(program, name.c_str());
		if (location == -1) {
			std::cerr << "Warning: Attribute '" << name << "' not found!" << std::endl;
		}
		return location;
	}

	GLint GetUniformBlockIndex(const std::string& name) {
		GLint index = glGetUniformBlockIndex(program, name.c_str());
		return index;
	}

private:
	void checkLinkingErrors() {
		GLint success;
		GLchar infoLog[1024];
		glGetProgramiv(program, GL_LINK_STATUS, &success);
		if (!success) {
			glGetProgramInfoLog(program, 1024, nullptr, infoLog);
			std::cerr << "ERROR::PROGRAM_LINKING_ERROR\n" << infoLog << std::endl;
		}
	}
};

class Buffer {
public:
	// Constructor with specified buffer type
	Buffer(GLenum bufferType) : bufferID(0), type(bufferType) {
		// Buffer ID will be managed by derived classes
	}

	virtual ~Buffer() {
		if (bufferID != 0) {
			glDeleteBuffers(1, &bufferID);
		}
	}

	// Bind the buffer
	void Bind() const {
		glBindBuffer(type, bufferID);
	}

	// Unbind the buffer
	void Unbind() const {
		glBindBuffer(type, 0);
	}

protected:
	GLuint bufferID;  // Buffer ID
	GLenum type;      // Buffer type (e.g., GL_ARRAY_BUFFER)
};

// Vertex Buffer Object (VBO) class
class VBO : public Buffer {
public:
	// Default constructor (creates an empty buffer)
	VBO() : Buffer(GL_ARRAY_BUFFER) {}

	// Constructor with data initialization
	VBO(const Vertex* data, GLsizeiptr size) : Buffer(GL_ARRAY_BUFFER) {
		Initialize(); // Ensure the buffer is created
		LoadData(data, size); // Load data into the buffer
	}

	// Method to initialize the buffer (generate ID)
	void Initialize() {
		if (bufferID == 0) {
			glGenBuffers(1, &bufferID);
		}
	}

	// Method to load data into the buffer
	void LoadData(const Vertex* data, GLsizeiptr size) {
		Bind();
		glBufferData(GL_ARRAY_BUFFER, size, data, GL_STATIC_DRAW);
	}
};

// Element Buffer Object (EBO) class
class EBO : public Buffer {
public:
	// Default constructor (creates an empty buffer)
	EBO() : Buffer(GL_ELEMENT_ARRAY_BUFFER) {}

	// Constructor with data initialization
	EBO(const GLuint* data, GLsizeiptr size) : Buffer(GL_ELEMENT_ARRAY_BUFFER) {
		Initialize(); // Ensure the buffer is created
		LoadData(data, size); // Load data into the buffer
	}

	// Method to initialize the buffer (generate ID)
	void Initialize() {
		if (bufferID == 0) {
			glGenBuffers(1, &bufferID);
		}
	}

	// Method to load data into the buffer
	void LoadData(const GLuint* data, GLsizeiptr size) {
		Bind();
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, size, data, GL_STATIC_DRAW);
	}
};


class VAO {
public:
	VAO() {
		glGenVertexArrays(1, &vaoID);
	}

	~VAO() {
		glDeleteVertexArrays(1, &vaoID);
	}

	void Bind() const {
		glBindVertexArray(vaoID);
	}

	void Unbind() const {
		glBindVertexArray(0);
	}
	
	void EnableVertexAttributeArray(GLuint index) {
		glEnableVertexAttribArray(index);
	}
	
	void VertexAttrributePointer(GLuint index, GLint size, GLenum type, GLboolean normalized, GLsizei stride, const void* pointer) {
		glVertexAttribPointer(index, size, type, normalized, stride, pointer);
	}

	GLuint getVao() {
		return vaoID;
	}
private:
	GLuint vaoID;
};

struct Material {
	glm::vec3 ambient;
	glm::vec3 diffuse;
	glm::vec3 specular;
	float shininess;
};

struct Light {
	glm::vec3 position;
	glm::vec3 ambient;
	glm::vec3 diffuse;
	glm::vec3 specular;
};

class UBO : public Buffer {
public:
	// Constructor that sets buffer type to GL_UNIFORM_BUFFER
	UBO() : Buffer(GL_UNIFORM_BUFFER), bindingPoint(0), size(0) {
		glGenBuffers(1, &bufferID); // Generate the UBO ID
	}

	// Initialize the UBO with the size and optionally the initial data
	void init(GLuint bindingPoint, GLsizeiptr size, void* data = nullptr) {
		this->bindingPoint = bindingPoint;
		this->size = size;

		Bind();  // Bind the UBO
		glBufferData(type, size, data, GL_STATIC_DRAW); // Allocate memory and optionally set data
		glBindBufferBase(type, bindingPoint, bufferID); // Bind to the uniform block binding point
		Unbind(); // Unbind the buffer
	}

	// Update UBO data at a specific offset
	void update(GLintptr offset, GLsizeiptr size, const void* data) {
		Bind();  // Bind the UBO
		glBufferSubData(type, offset, size, data); // Update buffer data
		Unbind(); // Unbind the buffer
	}

	// Bind the UBO to a uniform block in the shader program
	void bind(ShaderProgram program, GLuint blockIndex) {
		glUniformBlockBinding(program.program, blockIndex, bindingPoint); // Bind the block to the binding point
	}

private:
	GLuint bindingPoint;  // The binding point for the UBO
	GLsizeiptr size;      // Size of the UBO data
};