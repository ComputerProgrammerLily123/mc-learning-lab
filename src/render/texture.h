#pragma once

#define ATLAS_CELL 16
#define ATLAS_SIZE 64

#include <glad/glad.h>

#include <string>
#include <vector>
enum TextureType
{
    SingleSprite,
    MultipleSprite,
    Cubemap
};
class Texture
{
public:
    Texture(const std::string &path);
    ~Texture();
    void Bind(unsigned slot = 0) const;
    void CreateAtlas(const std::vector<std::string> &filename);
    void CreateCubemap(const std::vector<std::string> &filename);
    void CreateTexture(const std::string &filename);

private:
    std::string loadPath;
    unsigned ID;
    void LoadSingleTexture(const std::string &filename);
    void LoadMultipleTexture(const std::string &filename, int x, int y, int width, int height);
};