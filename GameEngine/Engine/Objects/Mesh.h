#pragma once
#include <vector>
#include "../Core.h"

class Mesh {
public:
	// Vertex Buffer and Index Buffer
	VertexBuffer vertexBuffer;
	ElementBuffer indexBuffer;

	// VertexArray object
	VertexArray vertexArray;

	// Constructor: Initialize with empty buffers
	Mesh() = default;

	// Set vertex data and index data
	void SetVertexData(const void* vertexData, GLsizeiptr vertexDataSize,
					   const void* indexData, GLsizeiptr indexDataSize,
					   const VertexFormat& vertexFormat, GLenum usage = GL_STATIC_DRAW) {
		// Set data for vertex buffer
		vertexBuffer.SetData(vertexData, vertexDataSize, usage);

		// Set data for index buffer
		indexBuffer.SetData(indexData, indexDataSize, usage);

		// Bind vertex array and add the buffers
		vertexArray.Bind();
		vertexArray.AddVertexBuffer(vertexBuffer, vertexFormat);
		vertexArray.AddIndexBuffer(indexBuffer);
		vertexArray.Unbind();
	}

	// Bind and unbind Mesh (VAO will handle everything)
	void Bind() const {
		vertexArray.Bind();
		vertexBuffer.Bind();
		indexBuffer.Bind();
	}

	void Unbind() const {
		vertexArray.Unbind();
		vertexBuffer.Unbind();
		indexBuffer.Unbind();
	}
};
