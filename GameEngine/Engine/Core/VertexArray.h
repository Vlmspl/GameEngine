#pragma once
#include <glad/glad.h>
#include <vector>
#include <iostream>
#include <cassert>
#include "Buffers.h"

class VertexFormat {
public:
    static VertexFormat Position;
    static VertexFormat PositionUv;
    static VertexFormat PositionUvNormal;

    struct Attribute {
        GLuint count;
        GLenum type;
        GLboolean normalized;
        GLsizei stride;
        const void* pointer;

        Attribute(GLuint c, GLenum t, GLboolean n, GLsizei s, const void* p)
            : count(c), type(t), normalized(n), stride(s), pointer(p) {}

        bool operator==(const Attribute &other) const {
            return (count == other.count) && (type == other.type) && (normalized == other.normalized) && (stride == other.stride);
        }
    };

private:
    std::vector<Attribute> attributes;

public:
    // Constructor is private to ensure it can only be built through FormatBuilder
    VertexFormat(std::vector<Attribute> attrs) : attributes(std::move(attrs)) {}

    // Accessor for the attributes
    const std::vector<Attribute>& getAttributes() const {
        return attributes;
    }

    // Static method to return a new instance of VertexFormat
    static VertexFormat createFromAttributes(const std::vector<Attribute>& attrs) {
        return VertexFormat(attrs);
    }
    bool operator==(const VertexFormat& other) const {
        return attributes == other.attributes;
    }
};

class FormatBuilder {
private:
    std::vector<VertexFormat::Attribute> attributes;

public:
    // Add an attribute to the list
    FormatBuilder& AddAttribute(GLuint count, GLenum type, GLboolean normalized, GLsizei stride, const void* pointer) {
        attributes.emplace_back(count, type, normalized, stride, pointer);
        return *this;
    }

    // Build and return a VertexFormat object
    VertexFormat Build() {
        return VertexFormat::createFromAttributes(attributes);
    }
};

class VertexArray {
public:
    GLuint ID;

    // Constructor: Initializes the Vertex Array Object
    VertexArray() {
        glGenVertexArrays(1, &ID);
    }

    // Destructor: Deletes the Vertex Array Object
    ~VertexArray() {
        glDeleteVertexArrays(1, &ID);
    }

    // Method to bind the Vertex Array Object
    void Bind() const {
        glBindVertexArray(ID);
    }

    // Method to unbind the Vertex Array Object
    void Unbind() const {
        glBindVertexArray(0);
    }

    // Method to attach a vertex buffer
    void AddVertexBuffer(const Buffer& buffer, const VertexFormat& format) {
        Bind();  // Bind VAO

        buffer.Bind();  // Bind VBO

        GLuint index = 0;
        // Bind each attribute from the format
        for (const auto& attr : format.getAttributes()) {
            // Set the vertex attribute pointer
            glEnableVertexAttribArray(index);  // Enable this vertex attribute
            glVertexAttribPointer(index, attr.count, attr.type, attr.normalized, attr.stride, attr.pointer);
            index++;
        }

        Unbind();  // Unbind VAO
    }

    // Method to attach an index buffer
    void AddIndexBuffer(const Buffer& buffer) {
        Bind();  // Bind VAO

        buffer.Bind();  // Bind EBO (Element Buffer Object)
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, buffer.ID);  // Specify the EBO to bind

        Unbind();  // Unbind VAO
    }
};


VertexFormat VertexFormat::Position = FormatBuilder()
                                    .AddAttribute(3, GL_FLOAT, GL_FALSE, 3*sizeof(float), (void*)0)
                                    .Build();

VertexFormat VertexFormat::PositionUv = FormatBuilder()
                                    .AddAttribute(3, GL_FLOAT, GL_FALSE, 5*sizeof(float), (void*)0)
                                    .AddAttribute(2, GL_FLOAT, GL_FALSE, 5*sizeof(float), (void*)(3*sizeof(float)))
                                    .Build();

VertexFormat VertexFormat::PositionUvNormal = FormatBuilder()
                                            .AddAttribute(3, GL_FLOAT, GL_FALSE, 8*sizeof(float), (void*)0)
                                            .AddAttribute(2, GL_FLOAT, GL_FALSE, 8*sizeof(float), (void*)(3*sizeof(float)))
                                            .AddAttribute(3, GL_FLOAT, GL_FALSE, 8*sizeof(float), (void*)(5*sizeof(float)))
                                            .Build();