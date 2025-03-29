#pragma once
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include <vector>

// Buffer base class
class Buffer {
public:
    GLuint ID;
    GLenum type;

    // Constructor
    Buffer(GLenum bufferType) : type(bufferType) {
        glGenBuffers(1, &ID);
    }

    // Bind buffer
    void Bind() const {
        glBindBuffer(type, ID);
    }

    // Unbind buffer
    static void Unbind(GLenum bufferType) {
        glBindBuffer(bufferType, 0);
    }

    // Delete buffer
    void Delete() {
        glDeleteBuffers(1, &ID);
    }
};

// Vertex Buffer Object (VBO)
class VertexBuffer : public Buffer {
public:
    VertexBuffer() : Buffer(GL_ARRAY_BUFFER) {}

    void SetData(const void* data, GLsizeiptr size, GLenum usage) {
        Bind();
        glBufferData(type, size, data, usage);
    }
};

// Element Buffer Object (EBO)
class ElementBuffer : public Buffer {
public:
    ElementBuffer() : Buffer(GL_ELEMENT_ARRAY_BUFFER) {}

    void SetData(const void* data, GLsizeiptr size, GLenum usage) {
        Bind();
        glBufferData(type, size, data, usage);
    }
};

// Uniform Buffer Object (UBO)
class UniformBuffer : public Buffer {
public:
    UniformBuffer() : Buffer(GL_UNIFORM_BUFFER) {}

    void SetData(const void* data, GLsizeiptr size, GLenum usage) {
        Bind();
        glBufferData(type, size, data, usage);
    }

    // Bind to a specific binding point for UBO
    void BindBase(GLuint bindingPoint) const {
        glBindBufferBase(GL_UNIFORM_BUFFER, bindingPoint, ID);
    }
};