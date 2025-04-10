#pragma once
#include <memory>
#include "Material.h"
#include "../Core.h"

class Mesh {
public:
	std::unique_ptr<float[]> vertexData;
	std::unique_ptr<uint8_t[]> indexData;
	GLsizeiptr vertexDataSize;
	GLsizeiptr indexDataSize;
	VertexFormat vertexFormat;
	GLenum usage;

	Mesh() : vertexFormat(VertexFormat::PositionUvNormal) {}

	void SetVertexData(const void* VertexData, GLsizeiptr VertexDataSize,
					   const void* IndexData, GLsizeiptr IndexDataSize,
					   VertexFormat VertexFormat, GLenum Usage = GL_STATIC_DRAW) {
		vertexData = std::make_unique<float[]>(VertexDataSize);
		std::memcpy(vertexData.get(), VertexData, VertexDataSize);

		indexData = std::make_unique<uint8_t[]>(IndexDataSize);
		std::memcpy(indexData.get(), IndexData, IndexDataSize);

		vertexDataSize = VertexDataSize;
		indexDataSize = IndexDataSize;
		vertexFormat = VertexFormat;
		usage = Usage;
	}
};
