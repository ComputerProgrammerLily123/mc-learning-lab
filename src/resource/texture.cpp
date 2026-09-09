#include "texture.h"

#include <glad/glad.h>

Texture::~Texture()
{
    if (ID)
    {
        glDeleteTextures(1, &ID);
        ID = 0;
    }
}
Texture::Texture(Texture&& other) noexcept : ID(other.ID), width(other.width), height(other.height), internalFormat(other.internalFormat), format(other.format), type(other.type) 
{
    other.ID = 0;
}
Texture& Texture::operator=(Texture&& other) noexcept
{
    if (this == &other)
    {
        return *this;
    }
    if (ID != 0)
    {
        glDeleteTextures(1, &ID);
    }
    ID = other.ID;
    width = other.width;
    height = other.height;
    internalFormat = other.internalFormat;
    format = other.format;
    type = other.type;

    other.ID = 0;
    return *this;
}
void Texture::Bind(unsigned slot) const
{
    glActiveTexture(GL_TEXTURE0 + slot);
    glBindTexture(target, ID);
}
void Texture::Upload(int width, int height, const unsigned char* data)
{
    Upload(width, height, GL_RGBA8, GL_RGBA, GL_UNSIGNED_BYTE, data);
}
void Texture::Upload(int width, int height, GLenum internalFormat, GLenum format, GLenum type, const unsigned char* data)
{
    if (!ID) glGenTextures(1, &ID);
    Bind();
    glTexImage2D(GL_TEXTURE_2D, 0, internalFormat, width, height, 0, format, type, data);
    this->width = width;
    this->height = height;
    this->internalFormat = internalFormat;
    this->format = format;
    this->type = type;
    SetFilter();
    SetWrap();
}
void Texture::UploadCube(int width, int height, const std::array<unsigned char*, 6>& data, GLenum internalFormat, GLenum format, GLenum type)
{
    if (!ID) glGenTextures(1, &ID);
    this->target = GL_TEXTURE_CUBE_MAP;
    Bind();
    for (int i = 0; i < 6; i++) {
        glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, internalFormat, width, height, 0, format, type, data[i]);
    }
    this->width = width;
    this->height = height;
    this->internalFormat = internalFormat;
    this->format = format;
    this->type = type;

    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
    glGenerateMipmap(GL_TEXTURE_CUBE_MAP);
}
Texture& Texture::SetFilter(GLenum minFilter, GLenum magFilter)
{
    Bind();
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, minFilter);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, magFilter);
    return *this;
}
Texture& Texture::SetWrap(GLenum wrapS, GLenum wrapT)
{
    Bind();
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, wrapS);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, wrapT);
    return *this;
}