#pragma once
#include "../Core.h"

class GeometryContainer {
public:
  VertexBuffer vertexBuffer;
  ElementBuffer indexBuffer;

  // VertexArray object
  VertexArray vertexArray;

  // Constructor: Initialize with empty buffers
  GeometryContainer() = default;

  // Set vertex data and index data
  void SetVertexData(const Mesh& mesh) {
    // Set data for vertex buffer
    vertexBuffer.SetData(mesh.vertexData.get(), mesh.vertexDataSize, mesh.usage);

    // Set data for index buffer
    indexBuffer.SetData(mesh.indexData.get(), mesh.indexDataSize, mesh.usage);

    // Bind vertex array and add the buffers
    vertexArray.Bind();
    vertexArray.AddVertexBuffer(vertexBuffer, mesh.vertexFormat);
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
    VertexBuffer::Unbind();
    ElementBuffer::Unbind();
  }

  void Draw() const {
    glDrawElements(
      GL_TRIANGLES,
      indexBuffer.bufferSize/sizeof(unsigned int),
      GL_UNSIGNED_INT,
      0
    );
  }
};