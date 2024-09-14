#include <glm/glm.hpp>

class Object {
public:
	glm::vec3 Position;
	glm::vec3 Rotation;
	glm::vec3 Scale;

#include <glm/vec3.hpp>

#include <glm/vec3.hpp>

    glm::vec3 Vertex[36] = {
        // Front face (2 triangles)
        glm::vec3(0, 0, 0), // Bottom-left
        glm::vec3(1, 0, 0), // Bottom-right
        glm::vec3(1, 1, 0), // Top-right

        glm::vec3(0, 0, 0), // Bottom-left
        glm::vec3(1, 1, 0), // Top-right
        glm::vec3(0, 1, 0), // Top-left

        // Back face (2 triangles)
        glm::vec3(0, 0, 1), // Bottom-left
        glm::vec3(1, 0, 1), // Bottom-right
        glm::vec3(1, 1, 1), // Top-right

        glm::vec3(0, 0, 1), // Bottom-left
        glm::vec3(1, 1, 1), // Top-right
        glm::vec3(0, 1, 1), // Top-left

        // Top face (2 triangles)
        glm::vec3(0, 1, 0), // Bottom-left
        glm::vec3(1, 1, 0), // Bottom-right
        glm::vec3(1, 1, 1), // Top-right

        glm::vec3(0, 1, 0), // Bottom-left
        glm::vec3(1, 1, 1), // Top-right
        glm::vec3(0, 1, 1), // Top-left

        // Bottom face (2 triangles)
        glm::vec3(0, 0, 0), // Bottom-left
        glm::vec3(1, 0, 0), // Bottom-right
        glm::vec3(1, 0, 1), // Top-right

        glm::vec3(0, 0, 0), // Bottom-left
        glm::vec3(1, 0, 1), // Top-right
        glm::vec3(0, 0, 1), // Top-left

        // Right face (2 triangles)
        glm::vec3(1, 0, 0), // Bottom-left
        glm::vec3(1, 1, 0), // Bottom-right
        glm::vec3(1, 1, 1), // Top-right

        glm::vec3(1, 0, 0), // Bottom-left
        glm::vec3(1, 1, 1), // Top-right
        glm::vec3(1, 0, 1), // Top-left

        // Left face (2 triangles)
        glm::vec3(0, 0, 0), // Bottom-left
        glm::vec3(0, 1, 0), // Bottom-right
        glm::vec3(0, 1, 1), // Top-right

        glm::vec3(0, 0, 0), // Bottom-left
        glm::vec3(0, 1, 1), // Top-right
        glm::vec3(0, 0, 1)  // Top-left
    };



	Object(const glm::vec3& position = glm::vec3(0, 0, 0), const glm::vec3& rotation = glm::vec3(0, 0, 0), const glm::vec3& scale = glm::vec3(1, 1, 1)) {
		Position = position;
		Rotation = rotation;
		Scale = scale;
	}
};