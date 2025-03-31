#pragma once
#include <string>
#include <sstream>
#include <iostream>
#include <vector>
#include <algorithm>
#include <unordered_map>
#include <glm/glm.hpp>
#include "../Objects.h"         // Contains Mesh, VertexBuffer, ElementBuffer, VertexArray, etc.
#include "../Core.h" // Contains VertexFormat, FormatBuilder, etc.

// **MeshFormat Enum**
enum class MeshFormat {
    OBJ,
    GLTF
};

// **PairHash** for deduplication (using position and UV indices)
struct PairHash {
    std::size_t operator()(const std::pair<GLuint, GLuint>& p) const {
        return std::hash<GLuint>()(p.first) ^ (std::hash<GLuint>()(p.second) << 1);
    }
};

/// **MeshLoader Class**
class MeshLoader {
public:
    /// <summary>
    /// Loads a mesh from a File based on the specified MeshFormat and VertexFormat.
    /// </summary>
    /// <param name="file">A pointer to a File object (must not be nullptr).</param>
    /// <param name="format">The mesh format (OBJ or GLTF).</param>
    /// <param name="vertexFormat">Desired VertexFormat (e.g. PositionUv or PositionUvNormal).</param>
    /// <returns>A Mesh instance with its buffers set up.</returns>
    static Mesh LoadMesh(File* file, MeshFormat format, const VertexFormat& vertexFormat) {
        if (!file) {
            std::cerr << "ERROR: Invalid file provided!" << std::endl;
            return Mesh();
        }

        switch (format) {
            case MeshFormat::OBJ:
                return LoadOBJMesh(*file, vertexFormat);
            case MeshFormat::GLTF:
                std::cerr << "GLTF parser not implemented yet." << std::endl;
                return Mesh();
            default:
                std::cerr << "Unsupported mesh format." << std::endl;
                return Mesh();
        }
    }

private:
    /// <summary>
    /// Loads an OBJ mesh from a File and builds a Mesh based on the desired VertexFormat.
    /// </summary>
    static Mesh LoadOBJMesh(File& file, const VertexFormat& vertexFormat) {
        // Read the entire file content into a string.
        std::string content = file.read();
        if (content.empty()) {
            std::cerr << "ERROR: File content is empty or could not be read." << std::endl;
            return Mesh();
        }

        // Use an istringstream to read the file line by line.
        std::istringstream stream(content);
        std::vector<glm::vec3> tempPositions;
        std::vector<glm::vec2> tempUVs;
        // The final vertex data is stored in a flat float array.
        std::vector<float> vertexData;
        std::vector<GLuint> indices;
        // Deduplication map: key = (positionIndex, uvIndex)
        std::unordered_map<std::pair<GLuint, GLuint>, GLuint, PairHash> vertexMap;

        // Determine the number of floats per vertex:
        // **PositionUv**: 5 floats (3 for pos, 2 for uv)
        // **PositionUvNormal**: 8 floats (3 for pos, 2 for uv, 3 for normal)
        const GLuint componentCount = (vertexFormat.getAttributes().size() == 2) ? 5 : 8;

        std::string line;
        while (std::getline(stream, line)) {
            std::istringstream lineStream(line);
            std::string prefix;
            lineStream >> prefix;

            if (prefix == "v") {
                glm::vec3 pos;
                lineStream >> pos.x >> pos.y >> pos.z;
                tempPositions.push_back(pos);
            }
            else if (prefix == "vt") {
                glm::vec2 uv;
                lineStream >> uv.x >> uv.y;
                tempUVs.push_back(uv);
            }
            else if (prefix == "f") {
                std::vector<GLuint> faceIndices;
                std::string vertexToken;
                while (lineStream >> vertexToken) {
                    // Replace '/' with space to easily extract indices.
                    std::replace(vertexToken.begin(), vertexToken.end(), '/', ' ');
                    std::istringstream tokenStream(vertexToken);
                    GLuint posIndex, uvIndex;
                    tokenStream >> posIndex >> uvIndex;
                    // Convert 1-based indices to 0-based.
                    if (posIndex > 0) posIndex--;
                    if (uvIndex > 0) uvIndex--;

                    std::pair<GLuint, GLuint> key(posIndex, uvIndex);
                    if (vertexMap.find(key) == vertexMap.end()) {
                        // Append position (3 floats)
                        const glm::vec3& pos = tempPositions[posIndex];
                        vertexData.push_back(pos.x);
                        vertexData.push_back(pos.y);
                        vertexData.push_back(pos.z);

                        // Append UV (2 floats)
                        const glm::vec2& uv = tempUVs[uvIndex];
                        vertexData.push_back(uv.x);
                        vertexData.push_back(uv.y);

                        // Append default normals if required by the vertex format.
                        if (componentCount > 5) {
                            vertexData.push_back(0.0f);
                            vertexData.push_back(0.0f);
                            vertexData.push_back(0.0f);
                        }

                        GLuint newIndex = static_cast<GLuint>(vertexData.size() / componentCount) - 1;
                        vertexMap[key] = newIndex;
                        faceIndices.push_back(newIndex);
                    } else {
                        faceIndices.push_back(vertexMap[key]);
                    }
                }

                // Triangulate the face (assuming convex polygons)
                if (faceIndices.size() >= 3) {
                    for (size_t i = 1; i < faceIndices.size() - 1; ++i) {
                        indices.push_back(faceIndices[0]);
                        indices.push_back(faceIndices[i]);
                        indices.push_back(faceIndices[i + 1]);
                    }
                }
            }
        }

        // Create a Mesh and populate its buffers.
        Mesh mesh;
        GLsizeiptr vertexDataSize = static_cast<GLsizeiptr>(vertexData.size() * sizeof(float));
        GLsizeiptr indexDataSize = static_cast<GLsizeiptr>(indices.size() * sizeof(GLuint));

        mesh.SetVertexData(vertexData.data(), vertexDataSize,
                           indices.data(), indexDataSize,
                           vertexFormat);
        return mesh;
    }
};
