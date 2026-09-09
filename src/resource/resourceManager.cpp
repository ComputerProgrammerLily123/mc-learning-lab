#include "resourceManager.h"

#include <windows.h>
#include <iostream>
#include <array>

ResourceManager::ResourceManager() : assetRoot(GetExecutableDirectory()) {}
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
    }else
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
    auto region = LoadPartialTexture(textureId, true, rect);
    textureRegions.emplace(id, region);
}
void ResourceManager::RegisterCubemap(const std::string& id, const std::array<std::string, 6>& textureId)
{
    auto it = cubemaps.find(id);
    if(it != cubemaps.end()) return;
    auto cubemap = LoadCubemap(textureId, false);
    cubemaps.emplace(id, cubemap);
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
std::shared_ptr<TextureRegion> ResourceManager::LoadPartialTexture(const std::string& loadPath, bool flip, const glm::i16vec4& rect)
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