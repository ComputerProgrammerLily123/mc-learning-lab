#pragma once
#include <string>
#include <vector>
#include <optional>

#include "textureRegion.h"
struct TextureData
{
    unsigned char* data;
    int width, height, channels;
};
class TextureCreator
{
public:
    TextureCreator() = delete;

    //static std::optional<TextureRegion> CreateTextureRegion(const TextureData& data);

    //std::optional<TextureRegion> CreateTextureRegion(const TextureData& data, unsigned px1, unsigned py1, unsigned px2, unsigned py2);
    //std::optional<TextureRegion> CreateTextureRegion(const TextureRegion& textureRegion, unsigned px1, unsigned py1, unsigned px2, unsigned py2);
    //void CreateAtlas(const std::vector<std::string>& filename);
    //void CreateCubemap(const std::vector<std::string>& filename);
};