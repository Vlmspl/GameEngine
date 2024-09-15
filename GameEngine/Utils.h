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

typedef struct Vertex {
    glm::vec3 pos;
    glm::vec3 col;
};