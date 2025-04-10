#pragma once
#include <glad/glad.h>
#include "../FileSystem/File.h"
#include <stb/stb_image.h>
#include <iostream>

class Texture {
public:
    GLuint textureID;
    GLuint64 textureHandle; // Only used if bindless mode is active.
    const File& textureFile;  // File reference for the texture
    bool useBindless;

    /**
     * Constructs a Texture object and loads the texture from a file.
     * @param file The file containing the texture.
     * @param activeTexture The texture unit (default GL_TEXTURE0).
     * @param filterType The filtering type (GL_LINEAR, GL_NEAREST, etc.).
     * @param repetitionType The wrapping mode (GL_REPEAT, GL_CLAMP_TO_EDGE, etc.).
     * @param useBindless Set to true to use bindless textures.
     */
    Texture(const File& file, GLenum activeTexture = GL_TEXTURE0,
            GLint filterType = GL_LINEAR, GLint repetitionType = GL_REPEAT,
            bool useBindless = false)
        : textureFile(file), useBindless(useBindless) {
        textureID = loadTexture(textureFile, activeTexture, filterType, repetitionType);
        if(useBindless) {
            textureHandle = glGetTextureHandleARB(textureID);
            glMakeTextureHandleResidentARB(textureHandle);
        } else {
            textureHandle = 0; // Not used in traditional mode.
        }
    }

    ~Texture() {
        glDeleteTextures(1, &textureID);
    }

    // For traditional binding mode
    void bind(GLenum activeTexture = GL_TEXTURE0) const {
        if (!useBindless) {
            glActiveTexture(activeTexture);
            glBindTexture(GL_TEXTURE_2D, textureID);
        }
    }

    static void UnBind() {
        glBindTexture(GL_TEXTURE_2D, 0);
    }

    // For bindless mode, get the 64-bit handle.
    GLuint64 getHandle() const {
        return textureHandle;
    }

private:
    GLuint loadTexture(const File& file, GLenum activeTexture, GLint filterType, GLint repetitionType) {
        if (!file.exists()) {
            std::cerr << "Texture file does not exist: " << file.getPath() << std::endl;
            return 0;
        }

        int width, height, numColorCh;
        unsigned char* bytes = stbi_load(file.getPath().c_str(), &width, &height, &numColorCh, 0);
        if (!bytes) {
            std::cerr << "Failed to load texture: " << file.getPath() << std::endl;
            return 0;
        }
        std::cout << "Texture loaded at address: " << static_cast<void*>(bytes) << "\n";

        GLuint texture;
        glGenTextures(1, &texture);
        glActiveTexture(activeTexture);
        glBindTexture(GL_TEXTURE_2D, texture);

        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, filterType);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, filterType);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, repetitionType);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, repetitionType);

        GLenum internalFormat, dataFormat;
        switch (numColorCh) {
            case 1:
                internalFormat = GL_RED;
                dataFormat = GL_RED;
                break;
            case 3:
                internalFormat = GL_RGB;
                dataFormat = GL_RGB;
                break;
            case 4:
                internalFormat = GL_RGBA;
                dataFormat = GL_RGBA;
                break;
            default:
                std::cerr << "Unsupported number of color channels: " << numColorCh << std::endl;
                stbi_image_free(bytes);
                return 0;
        }

        glTexImage2D(GL_TEXTURE_2D, 0, internalFormat, width, height, 0, dataFormat, GL_UNSIGNED_BYTE, bytes);
        glGenerateMipmap(GL_TEXTURE_2D);

        stbi_image_free(bytes);
        glBindTexture(GL_TEXTURE_2D, 0);

        std::cout << "Loaded texture ID: " << texture << std::endl;
        return texture;
    }
};
