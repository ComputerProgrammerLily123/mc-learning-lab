#pragma once

#include <glad/glad.h>

#include <string>
#include <unordered_map>
#include <vector>

#include "config/gameConstant.h"

using namespace GameConstant;
enum class TextureType : unsigned char
{
    SingleSprite,
    MultipleSprite,
    Cubemap
};
struct UVRegion
{
    float u1, v1, u2, v2;
};
class Texture
{
public:
    Texture() = default;
    Texture(const Texture&) = default;
    Texture(Texture&&) = delete;
    Texture& operator=(const Texture&) = default;
    Texture& operator=(Texture&&) = delete;
    Texture(const std::string& path);
    ~Texture();
    void SetLoadPath(const std::string& path);
    void Bind(unsigned slot = 0) const;
    void CreateAtlas(const std::vector<std::string>& filename);
    void CreateCubemap(const std::vector<std::string>& filename);
    void CreateTexture(const std::string& filename);
    const std::unordered_map<std::string, UVRegion>& GetUVMap();

private:
    std::string loadPath;
    unsigned ID{0};
    void LoadSingleTexture(const std::string& filename);
    void LoadMultipleTexture(const std::string& filename, unsigned x, unsigned y, unsigned targetWidth, unsigned targetHeight);
    std::unordered_map<std::string, UVRegion> uvMap;
};