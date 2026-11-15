#include "itemRegister.h"
#include <limits>
#include <stdexcept>
ItemId ItemRegister::RegisterBlockItem(const ResourceLocation& location, unsigned blockId)
{
    if (const auto existing = FindItem(location))
        return *existing;

    if (items.size() > std::numeric_limits<ItemId>::max())
        throw std::runtime_error("Too many registered items.");
    const ItemId id = static_cast<ItemId>(items.size());

    ResourceLocation modelLocation(location.GetNamespace(), "item/" + location.GetPath());

    items.push_back({id, location, modelLocation, blockId});
    itemIds.emplace(location, id);
    return id;
}
const ItemDefinition& ItemRegister::GetItem(ItemId id) const
{
    return items.at(id);
}

std::optional<ItemId> ItemRegister::FindItem(const ResourceLocation& location) const
{
    const auto it = itemIds.find(location);
    if (it == itemIds.end())
        return std::nullopt;
    return it->second;
}

const std::vector<ItemDefinition>& ItemRegister::GetItems() const noexcept
{
    return items;
}