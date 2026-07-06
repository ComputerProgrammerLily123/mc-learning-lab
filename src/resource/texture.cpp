#include "texture.h"

#include <glad/glad.h>

#include <iostream>

#include "stbImage/stb_image.h"

Texture::Texture(const std::string& path) : loadPath(path)
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
void Texture::SetLoadPath(const std::string& path)
{
    loadPath = path;
}
void Texture::Bind(unsigned slot) const
{
    glActiveTexture(GL_TEXTURE0 + slot);
    glBindTexture(GL_TEXTURE_2D, ID);
}
void Texture::CreateTexture(const std::string& filename)
{
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    LoadSingleTexture(filename);
}
void Texture::CreateAtlas(const std::vector<std::string>& filename)
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
    for (auto& file : filename)
    {
        if (file != "")
        {
            LoadMultipleTexture(file, offsetX, offsetY, ATLAS_CELL, ATLAS_CELL);

            offsetX += ATLAS_CELL;
            if (offsetX == ATLAS_SIZE)
            {
                offsetY += ATLAS_CELL;
                offsetX = 0;
            }
            UVRegion region{static_cast<float>(offsetX) / ATLAS_SIZE, static_cast<float>(offsetY) / ATLAS_SIZE, (static_cast<float>(offsetX) + ATLAS_CELL) / ATLAS_SIZE, (static_cast<float>(offsetY) + ATLAS_CELL) / ATLAS_SIZE};
            uvMap.emplace(file, region);
        }
    }
}
void Texture::CreateCubemap(const std::vector<std::string>& filename)
{
    stbi_set_flip_vertically_on_load(false);
    glBindTexture(GL_TEXTURE_CUBE_MAP, ID);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    int width = 0, height = 0, nrChannels = 0;
    for (unsigned int i = 0; i < filename.size(); i++)
    {
        unsigned char* data = stbi_load((loadPath + '/' + filename[i]).c_str(), &width, &height, &nrChannels, 0);
        if (data)
        {
            glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
            stbi_image_free(data);
        }
        else
        {
            std::cout << "Cubemap texture failed to load at path: " << loadPath << '/' << filename[i] << '\n';
        }
    }
}
void Texture::LoadSingleTexture(const std::string& filename)
{
    if (!ID)
    {
        std::cout << "binding failed when loading:" << filename << '\n';
        return;
    }
    stbi_set_flip_vertically_on_load(true);
    int width = 0, height = 0, nrChannels = 0;
    unsigned char* data = stbi_load((loadPath + "/" + filename).c_str(), &width, &height, &nrChannels, 0);
    if (!data)
    {
        std::cout << "loading failed:" << loadPath << "/" << filename << '\n';
        return;
    }
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
    stbi_image_free(data);
}
void Texture::LoadMultipleTexture(const std::string& filename, unsigned x, unsigned y, unsigned targetWidth, unsigned targetHeight)
{
    if (!ID)
    {
        std::cout << "binding failed when loading:" << filename << '\n';
        return;
    }
    stbi_set_flip_vertically_on_load(true);
    int width = 0, height = 0, nrChannels = 0;
    unsigned char* data = stbi_load((loadPath + "/" + filename).c_str(), &width, &height, &nrChannels, 0);
    if (!data || width != targetWidth || height != targetHeight)
    {
        std::cout << "loading failed:" << loadPath << "/" << filename << '\n';
        return;
    }
    glTexSubImage2D(GL_TEXTURE_2D, 0, static_cast<int>(x), static_cast<int>(y), static_cast<int>(targetWidth), static_cast<int>(targetHeight), GL_RGBA, GL_UNSIGNED_BYTE, data);
    stbi_image_free(data);
}
const std::unordered_map<std::string, UVRegion>& Texture::GetUVMap()
{
    return uvMap;
}