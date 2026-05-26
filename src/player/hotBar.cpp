#include "hotBar.h"
unsigned HotBar::GetSlotItem() const
{
    return slots[currentSlot];
}
unsigned HotBar::GetHotBarSlot() const
{
    return currentSlot;
}
void HotBar::SetHotBarSlot(unsigned slot)
{
    currentSlot = (slot + 9) % 9;
}