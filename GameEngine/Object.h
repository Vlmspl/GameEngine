#include <vector>
#include <string>
#include <glm/glm.hpp>
#include <functional>

class Object {
public:
    // Constructor
    Object(glm::vec3 pos, glm::vec3 rot, glm::vec3 scale, const std::string& path, ShaderProgram program)
        : Position(pos), Rotation(rot), Scale(scale), Program(program), vertices(nullptr), indices(nullptr) {
        load(path);
        VertexBufferObject.Initialize();
        VertexBufferObject.LoadData(vertices, vertexCount * sizeof(Vertex));

        ElementBufferObject.Initialize();
        ElementBufferObject.LoadData(indices, indexCount * sizeof(GLuint));

        program.use();
        GLint vPos_location = program.GetAttributeLocation("vPos");
        GLint vUv_location = program.GetAttributeLocation("vUv");

        VertexArrayObject.Bind();
        VertexArrayObject.EnableVertexAttributeArray(vPos_location);
        VertexArrayObject.VertexAttrributePointer(vPos_location, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, pos));

        VertexArrayObject.EnableVertexAttributeArray(vUv_location);
        VertexArrayObject.VertexAttrributePointer(vUv_location, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, uv));
    }

    // Destructor
    ~Object() {
        delete[] vertices;
        delete[] indices;
    }

    // Render method
    void Render() {
        Program.use();
        UpdateUniforms();

        VertexArrayObject.Bind();
        VertexBufferObject.Bind();
        ElementBufferObject.Bind();

        glDrawElements(GL_TRIANGLES, static_cast<GLsizei>(indexCount), GL_UNSIGNED_INT, nullptr);

        std::cout << "\nRendered?";
    }

    // Set a lambda function to update uniform data
    void BindUpdater(std::function<void(Object& object)> updater) {
        uniformUpdater = updater;
    }

    // Call the uniform updater if it exists
    void UpdateUniforms() {
        if (uniformUpdater) {
            uniformUpdater(*this);
        }
    }

    // Retrieve uniform location (and cache it in a hash table)
    GLint RetrieveUniformAddress(const std::string& uniformName) {
        // Check if the uniform is already in the cache
        if (uniformLocations.find(uniformName) == uniformLocations.end()) {
            // Uniform not found in cache, retrieve it using glGetUniformLocation and cache it
            GLint location = glGetUniformLocation(Program.program, uniformName.c_str());
            uniformLocations[uniformName] = location;
        }
        // Return the cached uniform location
        return uniformLocations[uniformName];
    }

private:
    ShaderProgram Program;

    // Transformations
    glm::vec3 Position;
    glm::vec3 Rotation;
    glm::vec3 Scale;

    // Vertices and indices
    Vertex* vertices;
    size_t vertexCount;
    GLuint* indices;
    size_t indexCount;

    VBO VertexBufferObject;
    VAO VertexArrayObject;
    EBO ElementBufferObject;

    std::function<void(Object& object)> uniformUpdater;  // Lambda function to update uniforms

    // Hash table to store uniform locations
    std::unordered_map<std::string, GLint> uniformLocations;

    // Method to load object data
    void load(const std::string& path) {
        // Use the updated parseOBJ function
        parseOBJ(path, vertices, vertexCount, indices, indexCount);
    }
};