#include "hotBar.h"
const ItemStack& HotBar::GetSlotItem() const
{
    return slots.at(currentSlot);
}

void HotBar::SetSlotItem(unsigned slot, ItemStack stack)
{
    slots.at(slot) = stack;
}
void HotBar::SetHotBarSlot(unsigned slot)
{
    currentSlot = (slot + SLOT_COUNT) % SLOT_COUNT;
}
unsigned HotBar::GetHotBarSlot() const
{
    return currentSlot;
}

const ItemStack& HotBar::GetSlotItem(unsigned slot) const
{
    return slots.at(slot);
}