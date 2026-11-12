#pragma once

#include <unordered_map>
#include <memory>
#include "resource/resourceLocation.h"
#include "modelDefinition.h"

class ModelContainer
{
public:
    [[nodiscard]] std::shared_ptr<model::Model> GetModel(const ResourceLocation& location) const;
    void LoadModel(const ResourceLocation& location, const std::shared_ptr<model::Model>& bakedModel);
private:
    std::unordered_map<ResourceLocation, std::shared_ptr<model::Model>> bakedModels;
};