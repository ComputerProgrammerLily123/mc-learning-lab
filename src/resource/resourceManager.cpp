#include "resourceManager.h"

#include <windows.h>

ResourceManager::ResourceManager()
{
    char buffer[MAX_PATH];
    GetModuleFileNameA(nullptr, buffer, sizeof(buffer));
    basePath = std::filesystem::path(buffer).parent_path() / "textures";
}
void ResourceManager::LoadTexture()
{
    std::vector<std::string> files;
    for (const auto& texture : std::filesystem::directory_iterator(basePath / "blocks"))
    {
        if (texture.path().extension().string() == ".png")
        {
            files.push_back(texture.path().filename().string());
        }
    }
    blockAtlas.SetLoadPath((basePath / "blocks").string());
    blockAtlas.CreateAtlas(files);
    files.clear();

    for (const auto& texture : std::filesystem::directory_iterator(basePath / "items"))
    {
        if (texture.path().extension().string() == ".png")
        {
            files.push_back(texture.path().filename().string());
        }
    }
    itemAtlas.SetLoadPath((basePath / "items").string());
    itemAtlas.CreateAtlas(files);
    files.clear();

    for (const auto& texture : std::filesystem::directory_iterator(basePath / "font"))
    {
        if (texture.path().extension().string() == ".png")
        {
            files.push_back(texture.path().filename().string());
        }
    }
    textAtlas.SetLoadPath((basePath / "font").string());
    textAtlas.CreateAtlas(files);
    files.clear();

    for (const auto& texture : std::filesystem::directory_iterator(basePath / "gui"))
    {
        if (texture.path().extension().string() == ".png")
        {
            files.push_back(texture.path().filename().string());
        }
    }
    guiAtlas.SetLoadPath((basePath / "gui").string());
    guiAtlas.CreateAtlas(files);
    files.clear();
}
const UVRegion& ResourceManager::GetUVRegion(const std::string& id)
{
    return uvMap[id];
}
Texture ResourceManager::GetAtlas(AtlasType type)
{
    switch (type)
    {
        case AtlasType::Block:
            return blockAtlas;
        case AtlasType::Item:
            return itemAtlas;
        case AtlasType::Text:
            return textAtlas;
        case AtlasType::GUI:
            return guiAtlas;
    }
    return Texture{};
}