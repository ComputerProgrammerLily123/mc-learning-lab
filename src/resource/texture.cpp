#include "texture.h"

#include <glad/glad.h>

#include <iostream>

#include "config/gameConstant.h"
#include "stbImage/stb_image.h"

using namespace GameConstant;
Texture::Texture(const std::string& path) : loadPath(path) {}
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
    if (!ID) glGenTextures(1, &ID);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

    auto data = LoadTexture(filename, true);
    if (data.data == nullptr) return;
    std::cout << "[Texture] Successfully load:" << filename << '\n';
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, data.width, data.height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data.data);
    stbi_image_free(data.data);
}
void Texture::CreateAtlas(const std::vector<std::string>& filename)
{
    if (!ID) glGenTextures(1, &ID);
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
            auto data = LoadTexture(file, true);
            if (data.data == nullptr) return;
            if (data.width % ATLAS_CELL != 0 || data.height % ATLAS_CELL != 0)
            {
                std::cout << "[Texture] Invalid size:" << loadPath << "/" << file << '\n';
                return;
            }
            std::cout << "[Texture] Successfully load:" << file << '\n';
            unsigned row = data.height / ATLAS_CELL;
            unsigned column = data.width / ATLAS_CELL;
            for (unsigned i = 0; i < row; i++)
            {
                for (unsigned j = 0; j < column; j++)
                {
                    int byteOffset = (i * data.width + j) * data.nrChannels * sizeof(unsigned char);
                    glTexSubImage2D(GL_TEXTURE_2D, 0, static_cast<int>(offsetX), static_cast<int>(offsetY), static_cast<int>(ATLAS_CELL), static_cast<int>(ATLAS_CELL), GL_RGBA, GL_UNSIGNED_BYTE, data.data + byteOffset);
                    offsetX += ATLAS_CELL;
                    if (offsetX == ATLAS_SIZE)
                    {
                        offsetY += ATLAS_CELL;
                        offsetX = 0;
                    }
                }
            }
            stbi_image_free(data.data);
            // TODO:注意这里的UV待改正
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

    for (unsigned int i = 0; i < filename.size(); i++)
    {
        auto data = LoadTexture(filename[i], false);
        if (data.data != nullptr)
        {
            glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_RGB, data.width, data.height, 0, GL_RGB, GL_UNSIGNED_BYTE, data.data);
            stbi_image_free(data.data);
        }
        else
        {
            std::cout << "[Texture] Cubemap texture failed to load at path: " << loadPath << '/' << filename[i] << '\n';
        }
    }
}
TextureData Texture::LoadTexture(const std::string& filename, bool flip)
{
    TextureData data = {};
    stbi_set_flip_vertically_on_load(flip);
    data.data = stbi_load((loadPath + "/" + filename).c_str(), &data.width, &data.height, &data.nrChannels, 0);
    if (data.data == nullptr)
    {
        std::cout << "[Texture] Loading failed:" << loadPath << "/" << filename << '\n';
    }
    return data;
}
const std::unordered_map<std::string, UVRegion>& Texture::GetUVMap()
{
    return uvMap;
}