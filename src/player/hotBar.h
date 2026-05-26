#ifndef HOT_BAR_H
#define HOT_BAR_H
#include<array>
#include "block.h"
class HotBar
{
public:
    unsigned GetSlotItem() const;
    unsigned GetHotBarSlot() const;
    void SetHotBarSlot(unsigned slot);
private:
    unsigned currentSlot = 0;
    std::array<unsigned,9> slots = {1,2,3,4,5,6,7,8,10};
};
#endif