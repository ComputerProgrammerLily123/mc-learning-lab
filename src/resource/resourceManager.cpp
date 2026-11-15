#include "resourceManager.h"

#include <windows.h>
#include <iostream>
#include <array>
#include <cstdlib>

#include "config/gameConstant.h"
#include "util/jsonReader.h"
#include "model/modelParser.h"
#include "model/modelBaker.h"
#include "blockstate/blockstateParser.h"
#include "blockstate/blockstateBaker.h"

using namespace GameConstant;

ResourceManager::ResourceManager() : assetRoot(GetExecutableDirectory()), blockAtlas(ATLAS_SIZE * ATLAS_SIZE * 4, 0) {}
const std::filesystem::path ResourceManager::GetModelPath(const ResourceLocation& location) const
{
    auto path = assetRoot / location.GetNamespace() / "models" / location.GetPath();
    path += ".json";
    return path;
}
const std::filesystem::path ResourceManager::GetBlockstatePath(const ResourceLocation& location) const
{
    auto path = assetRoot / location.GetNamespace() / "blockstates" / location.GetPath();
    path += ".json";
    return path;
}
std::shared_ptr<Texture> ResourceManager::GetBlockAtlas() const
{
    return blockAtlasPtr;
}
std::shared_ptr<TextureRegion> ResourceManager::GetTextureRegion(const ResourceLocation& location) const
{
    auto it = textureRegions.find(location);
    if(it != textureRegions.end())
    {
        return it->second;
    }else
    {
        return nullptr;
    }
}
std::shared_ptr<Texture> ResourceManager::GetCubemap(const ResourceLocation& mapId) const
{
    auto it = cubemaps.find(mapId);
    if(it != cubemaps.end())
    {
        return it->second;
    }
    else
    {
        return nullptr;
    }
}
std::shared_ptr<BitmapFont> ResourceManager::GetBitmapFont() const
{
    return bitmapFont;
}
void ResourceManager::GetBlockModels(const ResourceLocation& block, const blockstate::RuntimeBlockState& state, const glm::ivec3& position, std::vector<const model::BakedModel*>& output) const
{
    output.clear();

    const auto found = bakedBlockstateDefinitions.find(block);

    if (found == bakedBlockstateDefinitions.end())
        return;

    const auto& definition = *found->second;

    for (const auto& entry : definition.variants)
    {
        if (!blockstate::Matches(entry, state) || entry.models.empty())
        {
            continue;
        }

        const auto& selected = SelectVariant(entry.models, position, 0);

        output.push_back(selected.model.get());
        break;
    }

    for (std::size_t index = 0;index < definition.multipart.size();++index)
    {
        const auto& entry = definition.multipart[index];

        if (entry.models.empty())
            continue;

        if (entry.condition && !blockstate::Matches(*entry.condition, state))
        {
            continue;
        }

        const auto& selected = SelectVariant(entry.models, position, static_cast<std::uint32_t>(index + 1));

        output.push_back(selected.model.get());
    }
}
void ResourceManager::RegisterTextureRegion(const ResourceLocation& location)
{
    auto it = textureRegions.find(location);
    if(it != textureRegions.end()) return;
    auto region = LoadTexture(location, true);
    if(region != nullptr) textureRegions.emplace(location, region);
}
void ResourceManager::RegisterTextureRegion(const ResourceLocation& location, const ResourceLocation& fileLocation, const glm::i16vec4& rect)
{
    auto it = textureRegions.find(location);
    if(it != textureRegions.end()) return;
    auto region = LoadTexture(fileLocation, true, rect);
    if(region != nullptr) textureRegions.emplace(location, region);
}
void ResourceManager::RegisterTextureRegionInAtlas(const ResourceLocation& location)
{
    auto it = textureRegions.find(location);
    if(it != textureRegions.end()) return;
    auto regions = LoadPartialTexture(location, true);
    if (regions.empty()) return;
    textureRegions.emplace(location, regions.front()); //TODO:这里暂时假设图片都是16 * 16，只有一张
}
void ResourceManager::RegisterBitmapFont(const ResourceLocation& location)
{
    auto path = assetRoot / location.GetNamespace() / "textures" / location.GetPath();
    path += ".png";

    stbi_set_flip_vertically_on_load(true);

    int width = 0;
    int height = 0;
    int sourceChannels = 0;

    unsigned char* pixels = stbi_load(path.u8string().c_str(), &width, &height, &sourceChannels, STBI_rgb_alpha);

    if (pixels == nullptr)
    {
        std::cout<< "[ResourceManager] Font loading failed: " << path << '\n';
        return;
    }

    constexpr int channels = 4;

    bitmapFont = std::make_shared<BitmapFont>(pixels,width,height,channels,true);

    auto texture = std::make_shared<Texture>();
    texture->Upload(width, height, pixels);

    auto region = std::make_shared<TextureRegion>(texture);

    textureRegions.insert_or_assign(location, region);

    stbi_image_free(pixels);
}
void ResourceManager::RegisterCubemap(const ResourceLocation& mapId, const std::array<ResourceLocation, 6>& locations)
{
    auto it = cubemaps.find(mapId);
    if(it != cubemaps.end()) return;
    auto cubemap = LoadCubemap(locations, false);
    if(cubemap != nullptr) cubemaps.emplace(mapId, cubemap);
}
std::shared_ptr<model::Model> ResourceManager::LoadModel(const ResourceLocation& location)
{
    auto json = JsonReader::ReadJson(GetModelPath(location).string());
    auto rawModel = ModelParser::Parse(json);
    auto resolvedModel = std::make_shared<model::Model>(ModelParser::Resolve(rawModel));
    modelContainer.resolvedModels.insert_or_assign(location, resolvedModel);
    return resolvedModel;
}
void ResourceManager::BakeModel()
{
    for (const auto& [location, resolved] : modelContainer.resolvedModels)
    {
        auto baked = std::make_shared<model::BakedModel>(ModelBaker::Bake(*resolved));

        modelContainer.LoadModel(location, baked);
    }
}
std::shared_ptr<blockstate::BlockStateDefinition> ResourceManager::LoadBlockstate(const ResourceLocation& location)
{
    auto json = JsonReader::ReadJson(GetBlockstatePath(location).string());
    auto blockstate = std::make_shared<blockstate::BlockStateDefinition>(BlockstateParser::Parse(json));
    blockstateDefinitions.insert_or_assign(location, blockstate);
    return blockstate;
}
void ResourceManager::BakeBlockstates()
{
    for (const auto& [location, definition] : blockstateDefinitions)
    {
        auto baked =std::make_shared<blockstate::BakedBlockStateDefinition>(blockstate::BlockstateBaker::Bake(*definition,modelContainer));

        bakedBlockstateDefinitions.insert_or_assign(location, std::move(baked));
    }
}
void ResourceManager::LoadItemModel(const ResourceLocation& item)
{
    ResourceLocation modelLocation(item.GetNamespace(), "item/" + item.GetPath());

    auto model = LoadModel(modelLocation);
    itemModelLocations.insert_or_assign(item, modelLocation);

    if (model && model->textures)
    {
        for (const auto& [name, texture] : *model->textures)
            RegisterTextureRegionInAtlas(ResourceLocation(texture));
    }
}
void ResourceManager::BakeItemModels()
{
    for (const auto& [item, modelLocation] : itemModelLocations)
    {
        auto baked = modelContainer.GetModel(modelLocation);
        auto resolved = modelContainer.resolvedModels.at(modelLocation);

        auto result = std::make_shared<model::BakedItemModel>();
        result->model = baked;
        result->guiLight = resolved->gui_light.value_or("side");

        bakedItemModels.insert_or_assign(item, std::move(result));
    }
}
std::shared_ptr<const model::BakedItemModel> ResourceManager::GetItemModel(const ResourceLocation& item) const
{
    const auto it = bakedItemModels.find(item);
    if (it == bakedItemModels.end())
        return nullptr;
    return it->second;
}
std::shared_ptr<TextureRegion> ResourceManager::LoadTexture(const ResourceLocation& location, bool flip)
{
    TextureData data = {};
    auto path = assetRoot / location.GetNamespace() / "textures" / location.GetPath();
    path += ".png";
    stbi_set_flip_vertically_on_load(flip);
    data.data = stbi_load(path.u8string().c_str(), &data.width, &data.height, &data.channels, 0);
    if (data.data == nullptr)
    {
        std::cout << "[ResourceManager] Loading failed:" << path << '\n';
        return nullptr;
    }
    std::cout << "[ResourceManager] Successfully load:" << path << '\n';
    auto texturePtr = std::make_shared<Texture>();
    texturePtr->Upload(data.width, data.height, data.data);
    stbi_image_free(data.data);
    auto region = std::make_shared<TextureRegion>(texturePtr);
    return region;
}
std::shared_ptr<TextureRegion> ResourceManager::LoadTexture(const ResourceLocation& location, bool flip, const glm::i16vec4& rect)
{
    TextureData data = {};
    auto path = assetRoot / location.GetNamespace() / "textures" / location.GetPath();
    path += ".png";
    stbi_set_flip_vertically_on_load(flip);
    data.data = stbi_load(path.u8string().c_str(), &data.width, &data.height, &data.channels, 0);
    if (data.data == nullptr)
    {
        std::cout << "[ResourceManager] Loading failed:" << path << '\n';
        return nullptr;
    }
    std::cout << "[ResourceManager] Successfully load:" << path << '\n';
    auto texturePtr = std::make_shared<Texture>();
    texturePtr->Upload(data.width, data.height, data.data);
    stbi_image_free(data.data);
    auto region = std::make_shared<TextureRegion>(texturePtr, rect.x, rect.y, rect.z, rect.w, data.width, data.height);
    return region;
}
std::vector<std::shared_ptr<TextureRegion>> ResourceManager::LoadPartialTexture(const ResourceLocation& location, bool flip)
{
    std::vector<std::shared_ptr<TextureRegion>> regions;

    TextureData data = {};
    auto path = assetRoot / location.GetNamespace() / "textures" / location.GetPath();
    path += ".png";
    stbi_set_flip_vertically_on_load(flip);
    constexpr int AtlasChannels = 4;
    data.data = stbi_load(path.u8string().c_str(), &data.width, &data.height, &data.channels,STBI_rgb_alpha);

    if (data.data == nullptr)
    {
        std::cout << "[ResourceManager] Loading failed: " << path << '\n';
        return regions;
    }

    if (data.width <= 0 || data.height <= 0 || data.width  % ATLAS_CELL != 0 || data.height % ATLAS_CELL != 0)
    {
        std::cout << "[ResourceManager] Invalid size: " << path << " (" << data.width << "x" << data.height << ")\n";
        stbi_image_free(data.data);
        return regions;
    }

    std::cout << "[ResourceManager] Successfully load: " << path << '\n';

    const unsigned row    = data.height / ATLAS_CELL;
    const unsigned column = data.width  / ATLAS_CELL;
    const size_t  srcRowBytes  = size_t(data.width)  * AtlasChannels;
    const size_t  dstRowBytes  = size_t(ATLAS_SIZE)  * AtlasChannels;
    const size_t  cellRowBytes = size_t(ATLAS_CELL)  * AtlasChannels;

    for (unsigned i = 0; i < row; ++i)
    {
        for (unsigned j = 0; j < column; ++j)
        {
            if (blockAtlasOffsetX + ATLAS_CELL > ATLAS_SIZE)
            {
                blockAtlasOffsetY += ATLAS_CELL;
                blockAtlasOffsetX  = 0;
            }
            if (blockAtlasOffsetY + ATLAS_CELL > ATLAS_SIZE)
            {
                std::cout << "[ResourceManager] Atlas full, dropped: " << path << '\n';
                stbi_image_free(data.data);
                return regions;
            }

            const unsigned ox = blockAtlasOffsetX;
            const unsigned oy = blockAtlasOffsetY;

            const unsigned char* src = data.data + size_t(i * ATLAS_CELL) * srcRowBytes + size_t(j * ATLAS_CELL) * AtlasChannels;

            unsigned char* dst = blockAtlas.data() + size_t(oy) * dstRowBytes + size_t(ox) * AtlasChannels;

            for (int r = 0; r < ATLAS_CELL; ++r)
            {
                memcpy(dst + size_t(r) * dstRowBytes, src + size_t(r) * srcRowBytes, cellRowBytes);
            }

            blockAtlasOffsetX += ATLAS_CELL;
            if (blockAtlasOffsetX >= ATLAS_SIZE)
            {
                blockAtlasOffsetX = 0;
                blockAtlasOffsetY += ATLAS_CELL;
            }

            auto region = std::make_shared<TextureRegion>(blockAtlasPtr, ox, oy, ox + ATLAS_CELL, oy + ATLAS_CELL, ATLAS_SIZE, ATLAS_SIZE);
            regions.push_back(std::move(region));
        }
    }

    stbi_image_free(data.data);
    return regions;
}
void ResourceManager::UploadAtlas()
{
    blockAtlasPtr->Upload(ATLAS_SIZE, ATLAS_SIZE, blockAtlas.data());
}
std::shared_ptr<Texture> ResourceManager::LoadCubemap(const std::array<ResourceLocation, 6>& locations, bool flip)
{
    std::array<unsigned char*, 6> data = {};
    stbi_set_flip_vertically_on_load(flip);

    int dataIndex = 0;
    int width, height, channels;
    bool success = true;
    for(const auto& location : locations)
    {
        auto path = assetRoot / location.GetNamespace() / "textures" / location.GetPath();
        path += ".png";
        data[dataIndex] = stbi_load(path.u8string().c_str(), &width, &height, &channels, 0);
        if (data[dataIndex] == nullptr)
        {
            std::cout << "[ResourceManager] Loading failed:" << path << '\n';
            success = false;
        }
        std::cout << "[ResourceManager] Successfully load:" << path << '\n';
        dataIndex++;
    }
    auto texturePtr = std::make_shared<Texture>();
    if(success)
    {
        texturePtr->UploadCube(width, height, data);
        for (int i = 0; i < 6; i++) 
        {
            stbi_image_free(data[i]);
        }
        return texturePtr;
    }
    else
    {
        for (int i = 0; i < 6; i++) 
        {
            stbi_image_free(data[i]);
        }
        return nullptr;
    }
}
std::filesystem::path ResourceManager::GetExecutableDirectory()
{
    std::array<wchar_t, MAX_PATH> buffer{};

    const DWORD length = GetModuleFileNameW(nullptr, buffer.data(), MAX_PATH);

    if (length == 0)
    {
        throw std::system_error(static_cast<int>(GetLastError()), std::system_category(), "GetModuleFileNameW failed");
    }

    auto assetRoot = std::filesystem::path(std::wstring(buffer.data(), length)).parent_path() / "assets";

    const std::filesystem::path& rootMarker = assetRoot / ".mcassetsroot";
    std::error_code error;
    if (!std::filesystem::is_regular_file(rootMarker, error))
    {
        throw std::runtime_error("Missing asset marker: " + rootMarker.string());
    }
    return assetRoot;
}

std::uint32_t ResourceManager::HashBlockPosition(const glm::ivec3& position)
{
    std::uint32_t hash = 2166136261u;

    const auto mix = [&](int value)
    {
        hash ^= static_cast<std::uint32_t>(value);
        hash *= 16777619u;
    };

    mix(position.x);
    mix(position.y);
    mix(position.z);

    hash ^= hash >> 16;
    hash *= 0x7feb352du;
    hash ^= hash >> 15;
    hash *= 0x846ca68bu;
    hash ^= hash >> 16;

    return hash;
}

const blockstate::BakedModelVariant& ResourceManager::SelectVariant(const std::vector<blockstate::BakedModelVariant>& models, const glm::ivec3& position, std::uint32_t salt)
{
    if (models.empty())
        throw std::invalid_argument("Cannot select from empty models.");

    std::uint64_t totalWeight = 0;

    for (const auto& variant : models)
        totalWeight += static_cast<std::uint64_t>(variant.weight);

    std::uint32_t hash = HashBlockPosition(position);

    if (salt != 0)
    {
        hash ^= salt + 0x9e3779b9u + (hash << 6) + (hash >> 2);
    }

    std::uint64_t selected = hash % totalWeight;

    for (const auto& variant : models)
    {
        const auto weight = static_cast<std::uint64_t>(variant.weight);

        if (selected < weight)
            return variant;

        selected -= weight;
    }

    return models.back();
}