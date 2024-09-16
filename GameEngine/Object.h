#include <glm/glm.hpp>
#include <array>

class Object {
public:
    // Constructor
    Object(glm::vec3 pos, glm::vec3 rot, glm::vec3 scale, const std::string& path)
        : Position(pos), Rotation(rot), Scale(scale) {
        load(path);
    }

    // Render method
    void Render() {
        // Implement rendering logic here
    }

private:
    // Transformations
    glm::vec3 Position;
    glm::vec3 Rotation;
    glm::vec3 Scale;

    // Vertices and indices
    std::vector<Vertex> vertices;
    std::vector<int> indices;

    // Method to load object data
    void load(const std::string& path) {
        // Use a function to parse the OBJ file
        // For example:
        parseOBJ(path, vertices, indices);
    }
};