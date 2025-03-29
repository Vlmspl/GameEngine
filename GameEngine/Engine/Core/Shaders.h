#pragma once
#include <glad/glad.h>
#include <string>
#include <iostream>
#include <stdexcept>
#include <unordered_map>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "../FileSystem/File.h"

// Shader Class: Handles shader compilation
class Shader {
public:
    GLuint ID;
    GLenum type;

    // Constructor that loads and compiles the shader from a File instance
    Shader(GLenum shaderType, const File& file) : type(shaderType) {
        if (!file.exists()) {
            throw std::runtime_error("Shader file not found: " + file.getPath());
        }

        std::string shaderCode = file.read();
        if (shaderCode.empty()) {
            throw std::runtime_error("Failed to read shader file: " + file.getPath());
        }

        // Compile the shader
        CompileShader(shaderCode.c_str());
    }

    // Method to compile the shader
    void CompileShader(const char* shaderSource) {
        ID = glCreateShader(type);
        glShaderSource(ID, 1, &shaderSource, nullptr);
        glCompileShader(ID);

        // Check for compilation errors
        GLint success;
        glGetShaderiv(ID, GL_COMPILE_STATUS, &success);
        if (!success) {
            GLint length;
            glGetShaderiv(ID, GL_INFO_LOG_LENGTH, &length);
            char* log = new char[length];
            glGetShaderInfoLog(ID, length, &length, log);
            std::cerr << "Shader compilation failed: " << log << std::endl;
            delete[] log;
            throw std::runtime_error("Shader compilation failed.");
        }
    }

    // Destructor
    ~Shader() {
        glDeleteShader(ID);
    }
};

class ShaderProgram {
public:
    GLuint ID;
    std::unordered_map<std::string, GLint> uniformLocations;

    // Constructor that creates a shader program
    ShaderProgram() {
        ID = glCreateProgram();
    }

    // Method to attach a shader (it can be a vertex, fragment, etc.)
    void AttachShader(const Shader& shader) {
        glAttachShader(ID, shader.ID);
    }

    // Method to link the shader program
    void LinkProgram() {
        glLinkProgram(ID);

        // Check for linking errors
        GLint success;
        glGetProgramiv(ID, GL_LINK_STATUS, &success);
        if (!success) {
            GLint length;
            glGetProgramiv(ID, GL_INFO_LOG_LENGTH, &length);
            char* log = new char[length];
            glGetProgramInfoLog(ID, length, &length, log);
            std::cerr << "Shader program linking failed: " << log << std::endl;
            delete[] log;
            throw std::runtime_error("Shader program linking failed.");
        }
    }

    // Method to use the shader program
    void Use() const {
        glUseProgram(ID);
    }

    // Method to delete the program
    void Delete() const {
        glDeleteProgram(ID);
    }

    // Destructor
    ~ShaderProgram() {
        Delete();
    }

    // Set uniform (internal helper method to retrieve uniform location and cache it)
    GLint getUniformLocation(const std::string& name) {
        if (uniformLocations.find(name) != uniformLocations.end()) {
            return uniformLocations[name];
        }

        GLint location = glGetUniformLocation(ID, name.c_str());
        if (location == -1) {
            std::cerr << "Warning: Uniform " << name << " not found in shader program!" << std::endl;
        }
        uniformLocations[name] = location;
        return location;
    }

    // Set uniform for texture
    void SetUniform(const std::string& name, GLuint textureUnit) {
        GLint location = getUniformLocation(name);
        if (location != -1) {
            glUniform1i(location, textureUnit);
        }
    }

    // Set uniform for vec3 (glm::vec3)
    void SetUniform(const std::string& name, const glm::vec3& value) {
        GLint location = getUniformLocation(name);
        if (location != -1) {
            glUniform3fv(location, 1, glm::value_ptr(value));
        }
    }

    // Set uniform for mat4 (glm::mat4)
    void SetUniform(const std::string& name, const glm::mat4& value) {
        GLint location = getUniformLocation(name);
        if (location != -1) {
            glUniformMatrix4fv(location, 1, GL_FALSE, glm::value_ptr(value));
        }
    }

    // Set uniform for float
    void SetUniform(const std::string& name, float value) {
        GLint location = getUniformLocation(name);
        if (location != -1) {
            glUniform1f(location, value);
        }
    }

    // Set uniform for int
    void SetUniform(const std::string& name, int value) {
        GLint location = getUniformLocation(name);
        if (location != -1) {
            glUniform1i(location, value);
        }
    }

    // Set uniform for bool
    void SetUniform(const std::string& name, bool value) {
        GLint location = getUniformLocation(name);
        if (location != -1) {
            glUniform1i(location, static_cast<int>(value));
        }
    }

    void SetUniform(const std::string& name, GLuint64 value) {
        GLint location = getUniformLocation(name);
        if (location != -1) {
            glUniformHandleui64vARB(location, value, 0);
        }
    }
};
