#pragma once
#include <filesystem>

#include "texture.h"

enum class AtlasType : unsigned char
{
    Block,
    Item,
    Text,
    GUI
};
class ResourceManager
{
public:
    ResourceManager(ResourceManager&&) = delete;
    ResourceManager& operator=(ResourceManager&&) = delete;
    ResourceManager(const ResourceManager&) = delete;
    ResourceManager& operator=(const ResourceManager&) = delete;
    static ResourceManager& GetInstance()
    {
        static ResourceManager instance;
        return instance;
    }

    void LoadTexture();
    void LoadModel();
    Texture GetAtlas(AtlasType type);
    const UVRegion& GetUVRegion(const std::string& id);

private:
    ResourceManager();
    ~ResourceManager() = default;

    std::unordered_map<std::string, UVRegion> uvMap;

    std::filesystem::path basePath;

    Texture blockAtlas;
    Texture itemAtlas;
    Texture textAtlas;
    Texture guiAtlas;
};