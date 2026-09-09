#pragma once
#include "uiElement.h"
class Text : public UIElement
{
public:
    Text(PivotType pivotType, FlexType flexType, float x, float y, float width, float height);
};