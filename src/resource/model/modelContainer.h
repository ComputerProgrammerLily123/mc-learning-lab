#pragma once

#include <unordered_map>
#include <memory>
#include "resource/resourceLocation.h"
#include "modelDefinition.h"
#include "bakedModel.h"

class ModelContainer
{
    friend class ResourceManager;
public:
    [[nodiscard]] std::shared_ptr<model::BakedModel> GetModel(const ResourceLocation& location) const;
    void LoadModel(const ResourceLocation& location, const std::shared_ptr<model::BakedModel>& bakedModel);
private:
    std::unordered_map<ResourceLocation, std::shared_ptr<model::Model>> resolvedModels;
    std::unordered_map<ResourceLocation, std::shared_ptr<model::BakedModel>> bakedModels;
};