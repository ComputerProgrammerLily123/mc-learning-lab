#pragma once
#include <cstdint>
#include <optional>
#include <unordered_map>
#include <vector>

#include "itemBase.h"
#include "resource/resourceLocation.h"

using ItemId = std::uint16_t;
struct ItemDefinition
{
    ItemId id = 0;
    ResourceLocation location;
    ResourceLocation modelLocation;

    std::optional<unsigned> blockId;
};
struct ItemStack
{
    ItemId itemId = 0;
    std::uint16_t count = 0;

    [[nodiscard]] bool IsEmpty() const
    {
        return count == 0;
    }
};
class ItemRegister
{
public:
    static ItemRegister& GetInstance()
    {
        static ItemRegister instance;
        return instance;
    }
    ItemRegister(ItemRegister&&) = delete;
    ItemRegister& operator=(ItemRegister&&) = delete;
    ItemRegister(const ItemRegister&) = delete;
    ItemRegister& operator=(const ItemRegister&) = delete;
    ItemId RegisterBlockItem(const ResourceLocation& location, unsigned blockId);
    [[nodiscard]] const ItemDefinition& GetItem(ItemId id) const;
    [[nodiscard]] std::optional<ItemId> FindItem(const ResourceLocation& location) const;
    [[nodiscard]] const std::vector<ItemDefinition>& GetItems() const noexcept;

private:
    ItemRegister() = default;
    ~ItemRegister() = default;
    std::vector<ItemDefinition> items;
    std::unordered_map<ResourceLocation, ItemId> itemIds;
};