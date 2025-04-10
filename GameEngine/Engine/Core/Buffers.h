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
    GLsizeiptr bufferSize;

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
        bufferSize = size;
        glBufferData(type, size, data, usage);
    }

    static void Unbind() {
        glBindBuffer(GL_ARRAY_BUFFER, 0);
    }
};

// Element Buffer Object (EBO)
class ElementBuffer : public Buffer {
public:
    ElementBuffer() : Buffer(GL_ELEMENT_ARRAY_BUFFER) {}

    void SetData(const void* data, GLsizeiptr size, GLenum usage) {
        Bind();
        bufferSize = size;
        glBufferData(type, size, data, usage);
    }

    static void Unbind() {
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
    }
};

// Uniform Buffer Object (UBO)
class UniformBuffer : public Buffer {
public:
    UniformBuffer() : Buffer(GL_UNIFORM_BUFFER) {}

    void SetData(const void* data, GLsizeiptr size, GLenum usage) {
        Bind();
        bufferSize = size;
        glBufferData(type, size, data, usage);
    }

    static void Unbind() {
        glBindBuffer(GL_UNIFORM_BUFFER, 0);
    }

    // Bind to a specific binding point for UBO
    void BindBase(GLuint bindingPoint) const {
        glBindBufferBase(GL_UNIFORM_BUFFER, bindingPoint, ID);
    }
};