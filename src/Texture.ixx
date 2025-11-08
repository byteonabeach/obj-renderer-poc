module;

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"
#include <glad/glad.h>
#include <iostream>

export module Texture;

export namespace Graphics {
    [[nodiscard]]
    unsigned int loadTexture(std::string_view path, bool gammaCorrection = false) {
        unsigned int textureID{};
        glGenTextures(1, &textureID);

        int width = 0, height = 0, nrChannels = 0;
        unsigned char* data = stbi_load(path.data(), &width, &height, &nrChannels, 0);

        if (data) {
            GLenum internalFormat = GL_RGB;
            GLenum dataFormat = GL_RGB;

            switch (nrChannels) {
                case 1:
                    internalFormat = GL_RED;
                    dataFormat = GL_RED;
                    break;
                case 3:
                    internalFormat = gammaCorrection ? GL_SRGB : GL_RGB;
                    dataFormat = GL_RGB;
                    break;
                case 4:
                    internalFormat = gammaCorrection ? GL_SRGB_ALPHA : GL_RGBA;
                    dataFormat = GL_RGBA;
                    break;
                default:
                    internalFormat = gammaCorrection ? GL_SRGB : GL_RGB;
                    dataFormat = GL_RGB;
                    break;
            }

            glBindTexture(GL_TEXTURE_2D, textureID);
            glTexImage2D(GL_TEXTURE_2D, 0, internalFormat, width, height, 0, dataFormat,
                         GL_UNSIGNED_BYTE, data);
            glGenerateMipmap(GL_TEXTURE_2D);

            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

            stbi_image_free(data);
        } else {
            std::cout << "Failed to load texture: " << path << std::endl;

            if (data)
                stbi_image_free(data);

            return 0;
        }

        return textureID;
    }
}
