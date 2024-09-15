#include <fstream>
#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>

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

// Function to load a texture
GLuint LoadTextureFromFile(const char* filePath) {
    GLuint textureID;
    glGenTextures(1, &textureID);
    glBindTexture(GL_TEXTURE_2D, textureID);

    int width, height, channels;
    // Load image using stb_image
    unsigned char* data = stbi_load(filePath, &width, &height, &channels, 4);  // 4 for RGBA
    if (data == nullptr) {
        std::cerr << "Failed to load image: " << filePath << std::endl;
        return 0;
    }

    // Upload texture to OpenGL
    glTexImage2D(GL_TEXTURE_2D,
        0,
        GL_RGBA,        // Internal format
        width,
        height,
        0,
        GL_RGBA,        // Format
        GL_UNSIGNED_BYTE,  // Type
        data);

    // Set texture parameters
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    // Free the image memory
    stbi_image_free(data);

    return textureID;
}