#include "texture.h"

#include "stbImage/stb_image.h"
#include <glad/glad.h>
#include <iostream>

Texture::Texture(const std::string &path) : loadPath(path)
{
    glGenTextures(1, &ID);
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
void Texture::CreateTexture(const std::string &filename)
{
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    LoadSingleTexture(filename);
}
void Texture::CreateAtlas(const std::vector<std::string> &filename)
{
    glBindTexture(GL_TEXTURE_2D, ID);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, ATLAS_SIZE, ATLAS_SIZE, 0, GL_RGBA, GL_UNSIGNED_BYTE, nullptr);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    unsigned size = filename.size() / ATLAS_SIZE;
    unsigned offsetX = 0;
    unsigned offsetY = 0;
    for (auto &file : filename)
    {
        if (file != "")
        {
            LoadMultipleTexture(file, offsetX, offsetY, ATLAS_CELL, ATLAS_CELL);
        }
        offsetX += ATLAS_CELL;
        if (offsetX == ATLAS_SIZE)
        {
            offsetY += ATLAS_CELL;
            offsetX = 0;
        }
    }
}
void Texture::CreateCubemap(const std::vector<std::string> &filename)
{
    stbi_set_flip_vertically_on_load(false);
    glBindTexture(GL_TEXTURE_CUBE_MAP, ID);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    int width, height, nrChannels;
    for (unsigned int i = 0; i < filename.size(); i++)
    {
        unsigned char *data = stbi_load((loadPath + '/' + filename[i]).c_str(), &width, &height, &nrChannels, 0);
        if (data)
        {
            glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
            stbi_image_free(data);
        }
        else
        {
            std::cout << "Cubemap texture failed to load at path: " << (loadPath + '/' + filename[i]) << std::endl;
        }
    }
}
void Texture::LoadSingleTexture(const std::string &filename)
{
    stbi_set_flip_vertically_on_load(true);
    int w, h, channels;
    unsigned char *data = stbi_load((loadPath + "/" + filename).c_str(), &w, &h, &channels, 0);
    if (!data)
    {
        std::cout << "loading failed." << std::endl;
        return;
    }
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, w, h, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
    stbi_image_free(data);
}
void Texture::LoadMultipleTexture(const std::string &filename, int x, int y, int width, int height)
{
    stbi_set_flip_vertically_on_load(true);
    int w, h, channels;
    unsigned char *data = stbi_load((loadPath + "/" + filename).c_str(), &w, &h, &channels, 0);
    if (!data || w != width || h != height)
    {
        std::cout << "loading failed." << std::endl;
        return;
    }
    glTexSubImage2D(GL_TEXTURE_2D, 0, x, y, width, height, GL_RGBA, GL_UNSIGNED_BYTE, data);
    stbi_image_free(data);
}