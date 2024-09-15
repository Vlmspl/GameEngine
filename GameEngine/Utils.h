#include <fstream>

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
    glm::vec3 col;
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