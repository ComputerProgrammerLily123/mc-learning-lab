#include "resourceManager.h"

#include <windows.h>
#include <iostream>
#include <array>
#include <cstdlib>

#include "config/gameConstant.h"

using namespace GameConstant;

ResourceManager::ResourceManager() : assetRoot(GetExecutableDirectory()), blockAtlas(ATLAS_SIZE * ATLAS_SIZE * 4, 0) {}
const std::filesystem::path ResourceManager::GetModelPath(const ResourceLocation& location)
{
    auto path = assetRoot / location.GetNamespace() / "models" / location.GetPath();
    path += ".json";
    return path;
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
    auto json = JsonReader::ReadJson("assets/"  + location.GetNamespace() + "/models/" + location.GetPath() + ".json");
    auto rawModel = ModelParser::Parse(json);
    auto model = std::make_shared<model::Model>(ModelParser::Bake(rawModel));
    modelContainer.LoadModel(location, model);
    return model;
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
    data.data = stbi_load(path.u8string().c_str(), &data.width, &data.height, &data.channels,0);

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
    const size_t  srcRowBytes  = size_t(data.width)  * data.channels;
    const size_t  dstRowBytes  = size_t(ATLAS_SIZE)  * data.channels;
    const size_t  cellRowBytes = size_t(ATLAS_CELL)  * data.channels;

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

            const unsigned char* src = data.data + size_t(i * ATLAS_CELL) * srcRowBytes + size_t(j * ATLAS_CELL) * data.channels;

            unsigned char* dst = blockAtlas.data() + size_t(oy) * dstRowBytes + size_t(ox) * data.channels;

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