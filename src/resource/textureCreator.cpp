#include "textureCreator.h"
#include "config/gameConstant.h"
using namespace GameConstant;
/*
std::optional<TextureRegion> TextureCreator::CreateTextureRegion(const TextureData& data)
{
    if (data.data == nullptr) return std::nullopt;
    auto textureRegion = TextureRegion(data);
    return textureRegion;
}
std::optional<TextureRegion> TextureCreator::CreateTextureRegion(const std::string& filename, unsigned px1, unsigned py1, unsigned px2, unsigned py2)
{
    auto data = LoadTexture(filename, true);
    if (data.data == nullptr) return std::nullopt;
    auto textureRegion = TextureRegion(data, px1, py1, px2, py2);
    ClearData(data);
    return textureRegion;
}
std::optional<TextureRegion> TextureCreator::CreateTextureRegion(const TextureRegion& textureRegion, unsigned px1, unsigned py1, unsigned px2, unsigned py2)
{
    auto textureRegion = TextureRegion(textureRegion.GetTexturePtr(), px1, py1, px2, py2);
    return textureRegion;
}
void TextureCreator::CreateAtlas(const std::vector<std::string>& filename)
{
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
                std::cout << "[Texture] Invalid size:" << texturePathRoot / file << '\n';
                return;
            }
            std::cout << "[Texture] Successfully load:" << texturePathRoot / file << '\n';
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
            ClearData(data);
            // TODO:注意这里的UV待改正
            //UVRegion region{static_cast<float>(offsetX) / ATLAS_SIZE, static_cast<float>(offsetY) / ATLAS_SIZE, (static_cast<float>(offsetX) + ATLAS_CELL) / ATLAS_SIZE, (static_cast<float>(offsetY) + ATLAS_CELL) / ATLAS_SIZE};
        }
    }
}
void TextureCreator::CreateCubemap(const std::vector<std::string>& filename)
{
    stbi_set_flip_vertically_on_load(false);

    for (unsigned int i = 0; i < filename.size(); i++)
    {
        auto data = LoadTexture(filename[i], false);
        if (data.data != nullptr)
        {
            glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_RGB, data.width, data.height, 0, GL_RGB, GL_UNSIGNED_BYTE, data.data);
            ClearData(data);
        }
    }
}*/