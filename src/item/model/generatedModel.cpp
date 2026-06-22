#include "generatedModel.h"
std::string GeneratedModel::GetTexture(const std::string& layer) const
{
    auto texture = textures.find(layer);
    return (texture != textures.end()) ? texture->second : "";
}