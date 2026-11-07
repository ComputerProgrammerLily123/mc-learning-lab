#pragma once

#include <glad/glad.h>

#include <array>

class Texture
{
public:
    Texture() = default;
    ~Texture();

    Texture(const Texture&) = delete;
    Texture& operator=(const Texture&) = delete;

    Texture(Texture&& other) noexcept;
    Texture& operator=(Texture&& other) noexcept;

    void Bind(unsigned slot = 0) const;
    void Upload(int width, int height, const unsigned char* data, GLenum internalFormat = GL_RGBA8, GLenum format = GL_RGBA, GLenum type = GL_UNSIGNED_BYTE);
    void UploadCube(int width, int height, const std::array<unsigned char*, 6>& data, GLenum internalFormat = GL_RGBA8, GLenum format = GL_RGB, GLenum type = GL_UNSIGNED_BYTE);

    Texture& SetFilter(GLenum minFilter = GL_NEAREST, GLenum magFilter = GL_NEAREST);
    Texture& SetWrap(GLenum wrapS = GL_CLAMP_TO_EDGE, GLenum wrapT = GL_CLAMP_TO_EDGE);

    [[nodiscard]] int GetWidth() const { return width; };
    [[nodiscard]] int GetHeight() const { return height; };
    [[nodiscard]] GLuint GetID() const { return ID; };

private:
    GLuint ID {0};
    int width {0}, height {0};
    GLenum internalFormat {GL_RGBA8}, format {GL_RGBA}, type {GL_UNSIGNED_BYTE}, target {GL_TEXTURE_2D};
};