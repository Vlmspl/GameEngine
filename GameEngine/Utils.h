#include <fstream>
#define STB_IMAGE_IMPLEMENTATION
#include <stb/stb_image.h>
#include <vector>

std::string ReadShaderCode(const std::string& path) {
    std::ifstream file(path, std::ios::in | std::ios::binary);

    if (!file.is_open()) {
        throw std::exception("Issue while reading shader file");
        return "";
    }

    // Read the entire file content into a string
    std::string content((std::istreambuf_iterator<char>(file)), std::istreambuf_iterator<char>());

    file.close();

    return content;
}

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

    // Scale the movement vector by the move speed
    return movement * moveSpeed;
}


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


typedef struct Vertex {
    glm::vec3 pos;
    glm::vec2 uv;
};

void cursor_position_callback(GLFWwindow* window, double xpos, double ypos) {
    // Store current cursor position
    static double lastX = 0.0, lastY = 0.0;
    double xoffset = xpos - lastX;
    double yoffset = lastY - ypos; // Note: reversed Y offset

    // Update last cursor position
    lastX = xpos;
    lastY = ypos;

    // Handle input
    // (You can use xoffset and yoffset to adjust camera orientation here)
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

    // Upload the texture data
    glTexImage2D(GL_TEXTURE_2D, 0, internalFormat, width, height, 0, dataFormat, GL_UNSIGNED_BYTE, bytes);
    glGenerateMipmap(GL_TEXTURE_2D);

    // Free image memory
    stbi_image_free(bytes);

    // Unbind the texture
    glBindTexture(GL_TEXTURE_2D, 0);

    std::cout << "Loaded texture ID: " << texture << std::endl;

    return texture;
}

void checkShaderCompileErrors(GLuint shader, const std::string& type) {
    GLint success;
    GLchar infoLog[1024];
    glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
    if (!success) {
        glGetShaderInfoLog(shader, 1024, NULL, infoLog);
        std::cout << "ERROR::SHADER_COMPILATION_ERROR of type: " << type << "\n" << infoLog << "\n";
    }
}

void parseOBJ(const std::string& path, std::vector<Vertex>& vertices, std::vector<int>& indices) {
    std::ifstream file(path);
    if (!file.is_open()) {
        std::cerr << "Error opening file: " << path << std::endl;
        return;
    }

    std::vector<glm::vec3> tempPositions;
    std::vector<glm::vec2> tempUVs;

    std::string line;
    while (std::getline(file, line)) {
        std::istringstream s(line);
        std::string prefix;
        s >> prefix;

        if (prefix == "v") {
            glm::vec3 pos;
            s >> pos.x >> pos.y >> pos.z;
            tempPositions.push_back(pos);
        }
        else if (prefix == "vt") {
            glm::vec2 uv;
            s >> uv.x >> uv.y;
            tempUVs.push_back(uv);
        }
        else if (prefix == "f") {
            int posIndex, uvIndex;
            char slash;
            while (s >> posIndex >> slash >> uvIndex) {
                Vertex vertex;
                vertex.pos = tempPositions[posIndex - 1];
                vertex.uv = tempUVs[uvIndex - 1];
                vertices.push_back(vertex);
                indices.push_back(vertices.size() - 1);
            }
        }
    }

    file.close();
}

class Shader {
public:
    Shader(std::string Path) {}
};