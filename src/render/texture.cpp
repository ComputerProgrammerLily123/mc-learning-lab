#include "texture.h"

#include "stbImage/stb_image.h"
#include <glad/glad.h>
#include <iostream>

Texture::Texture(const std::string &path) : loadPath(path), ID(0)
{
    glGenTextures(1, &ID);
    stbi_set_flip_vertically_on_load(true);
}
Texture::~Texture()
{
    if (ID)
    {
        glDeleteTextures(1, &ID);
    }
}
void Texture::Bind(unsigned slot) const
{
    glActiveTexture(GL_TEXTURE0 + slot);
    glBindTexture(GL_TEXTURE_2D, ID);
}
void Texture::Unbind() const
{
    glBindTexture(GL_TEXTURE_2D, 0);
}
void Texture::CreateAtlas()
{
    glBindTexture(GL_TEXTURE_2D, ID);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, ATLAS_SIZE, ATLAS_SIZE, 0, GL_RGBA, GL_UNSIGNED_BYTE, nullptr);
    LoadTextureToAtlas("grass_top.png", 0, 0, 16, 16);
    LoadTextureToAtlas("grass_side.png", 16, 0, 16, 16);
    LoadTextureToAtlas("dirt.png", 0, 16, 16, 16);
    LoadTextureToAtlas("stone.png", 16, 16, 16, 16);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glGenerateMipmap(GL_TEXTURE_2D);
}
void Texture::LoadTexture(const std::string &filename)
{
    int w, h, channels;
    unsigned char *data = stbi_load((loadPath + "/" + filename).c_str(), &w, &h, &channels, 0);
    glBindTexture(GL_TEXTURE_2D, ID);
    if (data)
    {
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, w, h, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
    }
    else
    {
        std::cout << "loading failed." << std::endl;
    }
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    stbi_image_free(data);
}
void Texture::LoadTextureToAtlas(const std::string &filename, int x, int y, int width, int height)
{
    int w, h, channels;
    unsigned char *data = stbi_load((loadPath + "/" + filename).c_str(), &w, &h, &channels, 0);
    if (data && w == width && h == height)
    {
        glTexSubImage2D(GL_TEXTURE_2D, 0, x, y, width, height, GL_RGBA, GL_UNSIGNED_BYTE, data);
    }
    stbi_image_free(data);
}