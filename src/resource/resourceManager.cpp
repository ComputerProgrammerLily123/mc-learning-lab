#include "resourceManager.h"
#include <filesystem>
#include <iostream>
#include <windows.h>
void ResourceManager::LoadTexture()
{
    char buffer[MAX_PATH];
    std::vector<std::string> files;
    GetModuleFileNameA(NULL,buffer,sizeof(buffer));
    for(auto& texture : std::filesystem::directory_iterator(std::filesystem::path(buffer).parent_path().string() + "\\textures\\blocks")){
        if(texture.path().extension().string() == ".png")
        {
            files.push_back(texture.path().filename().string());
        }
    }
    blockAtlas.SetLoadPath(std::filesystem::path(buffer).parent_path().string() + "\\textures\\blocks");
    blockAtlas.CreateAtlas(files);
    files.clear();
    for(auto& texture : std::filesystem::directory_iterator(std::filesystem::path(buffer).parent_path().string() + "\\textures\\items")){
        if(texture.path().extension().string() == ".png")
        {
            files.push_back(texture.path().filename().string());
        }
    }
    itemAtlas.SetLoadPath(std::filesystem::path(buffer).parent_path().string() + "\\textures\\items");
    itemAtlas.CreateAtlas(files);
}
Texture ResourceManager::GetAtlas(AtlasType type)
{
    switch(type)
    {
        case AtlasType::Block:
            return blockAtlas;
        case AtlasType::Item:
            return itemAtlas;
    }
    return Texture{};
}