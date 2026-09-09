#pragma once
#include <filesystem>

#include <unordered_map>
#include "textureCreator.h"
#include <glm/glm.hpp>
#include "stbImage/stb_image.h"

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
    void RegisterTextureRegion(const std::string& id, const std::string& textureId);
    void RegisterTextureRegion(const std::string& id, const std::string& textureId, const glm::vec4& uv);
    void LoadModel();
    [[nodiscard]] std::shared_ptr<TextureRegion> GetTextureRegion(const std::string& id);
    
    
    private:
    ResourceManager();
    ~ResourceManager() = default;
    
    std::filesystem::path assetRoot;
    
    std::unordered_map<std::string, std::shared_ptr<TextureRegion>> textureRegions;
    
    std::shared_ptr<TextureRegion> LoadTexture(const std::string& loadPath, bool flip);
    std::shared_ptr<TextureRegion> LoadPartialTexture(const std::string& loadPath, bool flip, const glm::vec4& uv);

    std::filesystem::path GetExecutableDirectory();
};