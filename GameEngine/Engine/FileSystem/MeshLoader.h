#pragma once
#include <string>
#include <sstream>
#include <iostream>
#include <vector>
#include <algorithm>
#include <unordered_map>
#include <glm/glm.hpp>
#include "../Core.h"
#include "../Objects/Mesh.h"

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
    static Mesh LoadMesh(File* file, MeshFormat format, VertexFormat& vertexFormat) {
        if (!file) {
            std::cerr << "ERROR: Invalid file provided!" << std::endl;
            return Mesh();
        }

        switch (format) {
            case MeshFormat::OBJ:
                return LoadOBJMesh(*file, &vertexFormat);
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
    static Mesh LoadOBJMesh(File &file, const VertexFormat *vertexFormat) {
        // **Inclusion options for vertex components; adjust these as needed**
        bool includePosition;
        bool includeUV;
        bool includeNormals;

        if (*vertexFormat == VertexFormat::Position) {
            includePosition = true;
            includeUV = false;
            includeNormals = false;
        } else if (*vertexFormat == VertexFormat::PositionUv) {
            includePosition = true;
            includeUV = true;
            includeNormals = false;
        } else if (*vertexFormat == VertexFormat::PositionUvNormal) {
            includePosition = true;
            includeUV = true;
            includeNormals = true;
        }

        // *Read the entire file content*
        std::string content = file.read();
        std::istringstream stream(content);

        // **Temporary storages for positions, UVs, and normals**
        std::vector<glm::vec3> tempPositions;
        std::vector<glm::vec2> tempUVs;
        std::vector<glm::vec3> tempNormals;

        // **Final buffers: vertex data (floats) and index data**
        std::vector<float> vertexData;
        std::vector<GLuint> indices;

        // **Map for deduplication: key -> vertex index**
        std::unordered_map<std::string, GLuint> vertexMap;

        // **Calculate stride (number of floats per vertex)**
        size_t stride = 0;
        if (includePosition) stride += 3;
        if (includeUV)       stride += 2;
        if (includeNormals)  stride += 3;

        std::string line;
        while (std::getline(stream, line)) {
            std::istringstream s(line);
            std::string prefix;
            s >> prefix;

            if (prefix == "v" && includePosition) {
                // *Parse vertex positions*
                glm::vec3 pos;
                s >> pos.x >> pos.y >> pos.z;
                tempPositions.push_back(pos);
            }
            else if (prefix == "vt" && includeUV) {
                // *Parse UV coordinates*
                glm::vec2 uv;
                s >> uv.x >> uv.y;
                tempUVs.push_back(uv);
            }
            else if (prefix == "vn" && includeNormals) {
                // *Parse normals*
                glm::vec3 norm;
                s >> norm.x >> norm.y >> norm.z;
                tempNormals.push_back(norm);
            }
            else if (prefix == "f") {
                // **Parse face definitions**
                std::vector<GLuint> faceIndices;
                std::string vertexToken;
                while (s >> vertexToken) {
                    // *Replace '/' with space to ease parsing*
                    std::replace(vertexToken.begin(), vertexToken.end(), '/', ' ');
                    std::istringstream vertexStream(vertexToken);

                    GLuint posIndex = 0, uvIndex = 0, normIndex = 0;
                    if (includePosition) vertexStream >> posIndex;
                    if (includeUV)       vertexStream >> uvIndex;
                    if (includeNormals)  vertexStream >> normIndex;

                    // **Convert from 1-based to 0-based indexing**
                    if (includePosition && posIndex > 0) posIndex--;
                    if (includeUV       && uvIndex > 0)   uvIndex--;
                    if (includeNormals  && normIndex > 0)  normIndex--;

                // **Retrieve the corresponding data or default to zero**
                    glm::vec3 pos  = (includePosition && posIndex < tempPositions.size()) ? tempPositions[posIndex] : glm::vec3(0.0f);
                    glm::vec2 uv   = (includeUV && uvIndex < tempUVs.size())         ? tempUVs[uvIndex]         : glm::vec2(0.0f);
                    glm::vec3 norm = (includeNormals && normIndex < tempNormals.size())   ? tempNormals[normIndex]   : glm::vec3(0.0f);

                    // **Generate a unique key based on enabled components**
                    std::string key;
                    if (includePosition)
                        key += std::to_string(pos.x) + "_" + std::to_string(pos.y) + "_" + std::to_string(pos.z);
                    if (includeUV)
                        key += "_" + std::to_string(uv.x) + "_" + std::to_string(uv.y);
                    if (includeNormals)
                        key += "_" + std::to_string(norm.x) + "_" + std::to_string(norm.y) + "_" + std::to_string(norm.z);

                    GLuint vertexIndex;
                    auto it = vertexMap.find(key);
                    if (it == vertexMap.end()) {
                        // **Add a new vertex**
                        vertexIndex = vertexData.size() / stride;
                        if (includePosition) {
                            vertexData.push_back(pos.x);
                            vertexData.push_back(pos.y);
                            vertexData.push_back(pos.z);
                        }
                        if (includeUV) {
                            vertexData.push_back(uv.x);
                            vertexData.push_back(uv.y);
                        }
                        if (includeNormals) {
                            vertexData.push_back(norm.x);
                            vertexData.push_back(norm.y);
                            vertexData.push_back(norm.z);
                        }
                        vertexMap[key] = vertexIndex;
                    } else {
                        vertexIndex = it->second;
                    }
                    faceIndices.push_back(vertexIndex);
                }

                // **Triangulate the face (supports triangles, quads, etc.)**
                if (faceIndices.size() >= 3) {
                    for (size_t i = 1; i < faceIndices.size() - 1; ++i) {
                        indices.push_back(faceIndices[0]);
                        indices.push_back(faceIndices[i]);
                        indices.push_back(faceIndices[i + 1]);
                    }
                }
            }
        }

        Mesh mesh;
        mesh.SetVertexData(vertexData.data(), vertexData.size() * sizeof(float),
            indices.data(), indices.size() * sizeof(GLuint), *vertexFormat, GL_STATIC_DRAW);

        return mesh;
    }
};
