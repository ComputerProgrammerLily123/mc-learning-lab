#pragma once
#include <string>

class ResourceLocation
{
public:
    ResourceLocation(const std::string& namespace_, const std::string& path_);
    ResourceLocation(const std::string& path_);

    [[nodiscard]] const std::string& GetNamespace() const noexcept;
    [[nodiscard]] const std::string& GetPath() const noexcept;

    bool operator==(const ResourceLocation& other) const;

private:
    std::string namespace_;
    std::string path_;

    bool IsValidNamespace(const std::string& value);
    bool IsValidPath(const std::string& value);
};

namespace std {
    template<>
    struct hash<ResourceLocation> {
        std::size_t operator()(const ResourceLocation& location) const noexcept {
            std::size_t h1 = std::hash<std::string>{}(location.GetNamespace());
            std::size_t h2 = std::hash<std::string>{}(location.GetPath());
            size_t seed = h1 ^ (h2 + 0x9e3779b9 + (h1 << 6) + (h1 >> 2));
            return seed;
        }
    };
}