#pragma once
#include <array>

#include "item/itemRegister.h"

#define SLOT_COUNT 9
class HotBar
{
public:
    [[nodiscard]] const ItemStack& GetSlotItem() const;
    [[nodiscard]] const ItemStack& GetSlotItem(unsigned slot) const;
    void SetSlotItem(unsigned slot, ItemStack stack);
    void SetHotBarSlot(unsigned slot);
    [[nodiscard]] unsigned GetHotBarSlot() const;

private:
    unsigned currentSlot = 0;
    std::array<ItemStack, SLOT_COUNT> slots{};
};