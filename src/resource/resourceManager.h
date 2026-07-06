#pragma once
#include "texture.h"
enum class AtlasType : unsigned char
{
    Block,Item
};
class ResourceManager
{
public:
    ResourceManager(ResourceManager&&) = delete;
    ResourceManager& operator=(ResourceManager&&) = delete;
    ResourceManager(const ResourceManager&) = delete;
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