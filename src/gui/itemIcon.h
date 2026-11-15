#pragma once

#include "uiElement.h"
#include "item/itemRegister.h"

class ItemIcon final : public UIElement
{
public:
    ItemIcon(PivotType pivot, float x, float y, float width, float height, const ItemStack& stack) : UIElement(pivot, x, y, width, height),stack(&stack)
    {
    }

    [[nodiscard]] const ItemStack& GetStack() const
    {
        return *stack;
    }

private:
    const ItemStack* stack;
};