#pragma once
#include <string>
#include "texture.h"
enum class AtlasType
{
    Block,Item
};
class ResourceManager
{
public:
    ResourceManager(const ResourceManager &) = delete;
    ResourceManager &operator=(const ResourceManager &) = delete;
    static ResourceManager &GetInstance()
    {
        static ResourceManager instance;
        return instance;
    }

    void LoadTexture();
    Texture GetAtlas(AtlasType type);
private:
    ResourceManager() = default;
    ~ResourceManager() = default;
    Texture blockAtlas;
    Texture itemAtlas;
};