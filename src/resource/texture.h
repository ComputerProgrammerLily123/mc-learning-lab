#pragma once

#define ATLAS_CELL 16
#define ATLAS_SIZE 1024

#include <glad/glad.h>

#include <string>
#include <vector>
#include <unordered_map>
enum TextureType
{
    SingleSprite,
    MultipleSprite,
    Cubemap
};
struct UVRegion
{
    float u1,v1,u2,v2;
};
class Texture
{
public:
    Texture();
    Texture(const std::string &path);
    ~Texture();
    void SetLoadPath(const std::string& path);
    void Bind(unsigned slot = 0) const;
    void CreateAtlas(const std::vector<std::string> &filename);
    void CreateCubemap(const std::vector<std::string> &filename);
    void CreateTexture(const std::string &filename);
    const std::unordered_map<std::string, UVRegion>& GetUVMap();

private:
    std::string loadPath;
    unsigned ID;
    void LoadSingleTexture(const std::string &filename);
    void LoadMultipleTexture(const std::string &filename, int x, int y, int width, int height);
    std::unordered_map<std::string, UVRegion> uvMap;
};