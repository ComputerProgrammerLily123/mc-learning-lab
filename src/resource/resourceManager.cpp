#include "resourceManager.h"

#include <windows.h>
#include <iostream>
#include <array>
#include <cstdlib>

#include "config/gameConstant.h"

using namespace GameConstant;

ResourceManager::ResourceManager() : assetRoot(GetExecutableDirectory()), blockAtlas(ATLAS_SIZE * ATLAS_SIZE * 4, 0) {}
std::shared_ptr<TextureRegion> ResourceManager::GetTextureRegion(const std::string& id)
{
    auto it = textureRegions.find(id);
    if(it != textureRegions.end())
    {
        return it->second;
    }else
    {
        return nullptr;
    }
}
std::shared_ptr<Texture> ResourceManager::GetCubemap(const std::string& id)
{
    auto it = cubemaps.find(id);
    if(it != cubemaps.end())
    {
        return it->second;
    }
    else
    {
        return nullptr;
    }
}
void ResourceManager::RegisterTextureRegion(const std::string& id, const std::string& textureId)
{
    auto it = textureRegions.find(id);
    if(it != textureRegions.end()) return;
    auto region = LoadTexture(textureId, true);
    textureRegions.emplace(id, region);
}
void ResourceManager::RegisterTextureRegion(const std::string& id, const std::string& textureId, const glm::i16vec4& rect)
{
    auto it = textureRegions.find(id);
    if(it != textureRegions.end()) return;
    auto region = LoadTexture(textureId, true, rect);
    textureRegions.emplace(id, region);
}
void ResourceManager::RegisterTextureRegionInAtlas(const std::string& id, const std::string& textureId)
{
    auto it = textureRegions.find(id);
    if(it != textureRegions.end()) return;
    auto regions = LoadPartialTexture(textureId, true);
    if (regions.empty()) return;
    textureRegions.emplace(id, regions.front()); //TODO:这里暂时假设图片都是16 * 16，只有一张
}
void ResourceManager::RegisterCubemap(const std::string& id, const std::array<std::string, 6>& textureId)
{
    auto it = cubemaps.find(id);
    if(it != cubemaps.end()) return;
    auto cubemap = LoadCubemap(textureId, false);
    cubemaps.emplace(id, cubemap);
}
std::shared_ptr<ResolvedModel> ResourceManager::LoadModel(const std::string& id)
{
    auto json = JsonReader::ReadJson("assets/minecraft/models/block/" + id + ".json");
    auto rawModel = ModelParser::Parse(json);
    auto model = std::make_shared<ResolvedModel>(ModelParser::Resolve(rawModel));
    models.emplace("block/" + id, model);
    return model;
}
std::shared_ptr<ResolvedModel> ResourceManager::GetModel(const std::string& id)
{
    auto it = models.find("block/" + id);
    if(it != models.end())
    {
        return it->second;
    }
    else
    {
        return nullptr;
    }
}
std::shared_ptr<TextureRegion> ResourceManager::LoadTexture(const std::string& loadPath, bool flip)
{
    TextureData data = {};
    auto path = assetRoot / "minecraft" / "textures" / loadPath;
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
std::shared_ptr<TextureRegion> ResourceManager::LoadTexture(const std::string& loadPath, bool flip, const glm::i16vec4& rect)
{
    TextureData data = {};
    auto path = assetRoot / "minecraft" / "textures" / loadPath;
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
std::vector<std::shared_ptr<TextureRegion>> ResourceManager::LoadPartialTexture(const std::string& loadPath, bool flip)
{
    std::vector<std::shared_ptr<TextureRegion>> regions;

    TextureData data = {};
    auto path = assetRoot / "minecraft" / "textures" / loadPath;
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
std::shared_ptr<Texture> ResourceManager::LoadCubemap(const std::array<std::string, 6>& loadPaths, bool flip)
{
    std::array<unsigned char*, 6> data = {};
    stbi_set_flip_vertically_on_load(flip);

    int dataIndex = 0;
    int width, height, channels;
    bool success = true;
    for(const auto& loadPath : loadPaths)
    {
        auto path = assetRoot / "minecraft" / "textures" / loadPath;
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