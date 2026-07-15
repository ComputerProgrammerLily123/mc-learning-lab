#pragma once

#include <glad/glad.h>

#include <string>
#include <unordered_map>
#include <vector>

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
struct TextureData
{
    unsigned char* data;
    GLsizei width,height;
    GLint nrChannels;
};
class Texture
{
public:
    Texture() = default;
    Texture(const std::string& path);
    Texture(const Texture&) = default;
    Texture(Texture&&) = delete;
    Texture& operator=(const Texture&) = default;
    Texture& operator=(Texture&&) = delete;
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
    TextureData LoadTexture(const std::string& filename,bool flip);
    std::unordered_map<std::string, UVRegion> uvMap;
};