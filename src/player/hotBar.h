#pragma once
#include<array>
#define SLOT_COUNT 9
class HotBar
{
public:
    [[nodiscard]] unsigned GetSlotItem() const;
    [[nodiscard]] unsigned GetHotBarSlot() const;
    void SetHotBarSlot(unsigned slot);
private:
    unsigned currentSlot = 0;
    std::array<unsigned,SLOT_COUNT> slots = {1,2,3,4,5,6,7,8,10};
};