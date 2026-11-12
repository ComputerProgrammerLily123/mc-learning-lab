#include "modelContainer.h"

std::shared_ptr<model::Model> ModelContainer::GetModel(const ResourceLocation& location) const
{
    auto it = bakedModels.find(location);
    if(it != bakedModels.end())
    {
        return it->second;
    }
    else
    {
        return nullptr;
    }
}
void ModelContainer::LoadModel(const ResourceLocation& location, const std::shared_ptr<model::Model>& bakedModel)
{
    bakedModels.emplace(location, bakedModel);
}