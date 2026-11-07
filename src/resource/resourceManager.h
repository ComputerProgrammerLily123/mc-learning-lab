#pragma once
#include <filesystem>

#include <glm/glm.hpp>
#include "stbImage/stb_image.h"
#include "textureRegion.h"
#include "modelParser.h"
#include "resourceLocation.h"
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

    const std::filesystem::path GetModelPath(const ResourceLocation& location);

    void RegisterTextureRegion(const ResourceLocation& location);
    void RegisterTextureRegion(const ResourceLocation& location, const ResourceLocation& fileLocation, const glm::i16vec4& rect);
    void RegisterTextureRegionInAtlas(const ResourceLocation& location);
    void UploadAtlas();
    void RegisterCubemap(const ResourceLocation& mapId, const std::array<ResourceLocation, 6>& locations);
    std::shared_ptr<model::ResolvedModel> LoadModel(const ResourceLocation& location);
    [[nodiscard]] std::shared_ptr<model::ResolvedModel> GetModel(const ResourceLocation& location);
    [[nodiscard]] std::shared_ptr<TextureRegion> GetTextureRegion(const ResourceLocation& location);
    [[nodiscard]] std::shared_ptr<Texture> GetCubemap(const ResourceLocation& mapId);   
    
    private:
    ResourceManager();
    ~ResourceManager() = default;
    
    std::filesystem::path assetRoot;

    std::unordered_map<ResourceLocation, std::shared_ptr<ResolvedModel>> models;
    
    std::unordered_map<ResourceLocation, std::shared_ptr<TextureRegion>> textureRegions;
    std::unordered_map<ResourceLocation, std::shared_ptr<Texture>> cubemaps;

    std::vector<unsigned char> blockAtlas;
    std::shared_ptr<Texture> blockAtlasPtr = std::make_shared<Texture>();
    unsigned blockAtlasOffsetX = 0;
    unsigned blockAtlasOffsetY = 0;
    
    std::shared_ptr<TextureRegion> LoadTexture(const ResourceLocation& location, bool flip);
    std::shared_ptr<TextureRegion> LoadTexture(const ResourceLocation& location, bool flip, const glm::i16vec4& rect);
    std::shared_ptr<Texture> LoadCubemap(const std::array<ResourceLocation, 6>& location, bool flip);
    
    std::vector<std::shared_ptr<TextureRegion>> LoadPartialTexture(const ResourceLocation& locations, bool flip);

    std::filesystem::path GetExecutableDirectory();
};