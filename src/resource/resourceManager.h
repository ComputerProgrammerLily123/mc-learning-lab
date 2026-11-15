#pragma once
#include <filesystem>
#include <glm/glm.hpp>

#include "bitmapFont.h"
#include "blockstate/blockstateDefinition.h"
#include "model/modelContainer.h"
#include "resourceLocation.h"
#include "stbImage/stb_image.h"
#include "textureRegion.h"

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

    ModelContainer modelContainer;

    const std::filesystem::path GetModelPath(const ResourceLocation& location) const;
    const std::filesystem::path GetBlockstatePath(const ResourceLocation& location) const;
    std::shared_ptr<Texture> GetBlockAtlas() const;

    void RegisterTextureRegion(const ResourceLocation& location);
    void RegisterTextureRegion(const ResourceLocation& location, const ResourceLocation& fileLocation, const glm::i16vec4& rect);
    void RegisterTextureRegionInAtlas(const ResourceLocation& location);
    void RegisterBitmapFont(const ResourceLocation& location);
    void UploadAtlas();
    void RegisterCubemap(const ResourceLocation& mapId, const std::array<ResourceLocation, 6>& locations);
    std::shared_ptr<model::Model> LoadModel(const ResourceLocation& location);
    void BakeModel();

    std::shared_ptr<blockstate::BlockStateDefinition> LoadBlockstate(const ResourceLocation& location);
    void BakeBlockstates();

    void LoadItemModel(const ResourceLocation& item);
    void BakeItemModels();

    [[nodiscard]] std::shared_ptr<const model::BakedItemModel> GetItemModel(const ResourceLocation& item) const;

    [[nodiscard]] std::shared_ptr<TextureRegion> GetTextureRegion(const ResourceLocation& location) const;
    [[nodiscard]] std::shared_ptr<Texture> GetCubemap(const ResourceLocation& mapId) const;
    [[nodiscard]] std::shared_ptr<BitmapFont> GetBitmapFont() const;
    void GetBlockModels(const ResourceLocation& block, const blockstate::RuntimeBlockState& state, const glm::ivec3& position, std::vector<const model::BakedModel*>& output) const;

private:
    ResourceManager();
    ~ResourceManager() = default;

    std::filesystem::path assetRoot;

    std::unordered_map<ResourceLocation, std::shared_ptr<TextureRegion>> textureRegions;
    std::unordered_map<ResourceLocation, std::shared_ptr<Texture>> cubemaps;

    std::unordered_map<ResourceLocation, std::shared_ptr<blockstate::BlockStateDefinition>> blockstateDefinitions;
    std::unordered_map<ResourceLocation, std::shared_ptr<blockstate::BakedBlockStateDefinition>> bakedBlockstateDefinitions;

    std::unordered_map<ResourceLocation, std::shared_ptr<model::BakedItemModel>> bakedItemModels;

    std::unordered_map<ResourceLocation, ResourceLocation> itemModelLocations;

    std::vector<unsigned char> blockAtlas;
    std::shared_ptr<Texture> blockAtlasPtr = std::make_shared<Texture>();
    unsigned blockAtlasOffsetX = 0;
    unsigned blockAtlasOffsetY = 0;

    std::shared_ptr<BitmapFont> bitmapFont;

    std::shared_ptr<TextureRegion> LoadTexture(const ResourceLocation& location, bool flip);
    std::shared_ptr<TextureRegion> LoadTexture(const ResourceLocation& location, bool flip, const glm::i16vec4& rect);
    std::shared_ptr<Texture> LoadCubemap(const std::array<ResourceLocation, 6>& location, bool flip);

    std::vector<std::shared_ptr<TextureRegion>> LoadPartialTexture(const ResourceLocation& locations, bool flip);

    std::filesystem::path GetExecutableDirectory();

    static std::uint32_t HashBlockPosition(const glm::ivec3& position);
    static const blockstate::BakedModelVariant& SelectVariant(const std::vector<blockstate::BakedModelVariant>& models, const glm::ivec3& position, std::uint32_t salt = 0);
};