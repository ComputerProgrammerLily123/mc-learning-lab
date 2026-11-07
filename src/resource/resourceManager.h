#pragma once
#include <filesystem>

#include <glm/glm.hpp>
#include "stbImage/stb_image.h"
#include "textureRegion.h"
#include "modelParser.h"
struct TextureData
{
    unsigned char* data;
    int width, height, channels;
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
    void RegisterTextureRegion(const std::string& id, const std::string& textureId);
    void RegisterTextureRegion(const std::string& id, const std::string& textureId, const glm::i16vec4& rect);
    void RegisterTextureRegionInAtlas(const std::string& id, const std::string& textureId);
    void UploadAtlas();
    void RegisterCubemap(const std::string& id, const std::array<std::string, 6>& textureId);
    std::shared_ptr<ResolvedModel> LoadModel(const std::string& id);
    [[nodiscard]] std::shared_ptr<ResolvedModel> GetModel(const std::string& id);
    [[nodiscard]] std::shared_ptr<TextureRegion> GetTextureRegion(const std::string& id);
    [[nodiscard]] std::shared_ptr<Texture> GetCubemap(const std::string& id);   
    
    private:
    ResourceManager();
    ~ResourceManager() = default;
    
    std::filesystem::path assetRoot;

    std::unordered_map<std::string, std::shared_ptr<ResolvedModel>> models;
    
    std::unordered_map<std::string, std::shared_ptr<TextureRegion>> textureRegions;
    std::unordered_map<std::string, std::shared_ptr<Texture>> cubemaps;

    std::vector<unsigned char> blockAtlas;
    std::shared_ptr<Texture> blockAtlasPtr = std::make_shared<Texture>();
    unsigned blockAtlasOffsetX = 0;
    unsigned blockAtlasOffsetY = 0;
    
    std::shared_ptr<TextureRegion> LoadTexture(const std::string& loadPath, bool flip);
    std::shared_ptr<TextureRegion> LoadTexture(const std::string& loadPath, bool flip, const glm::i16vec4& rect);
    std::shared_ptr<Texture> LoadCubemap(const std::array<std::string, 6>& loadPaths, bool flip);
    
    std::vector<std::shared_ptr<TextureRegion>> LoadPartialTexture(const std::string& loadPath, bool flip);

    std::filesystem::path GetExecutableDirectory();
};