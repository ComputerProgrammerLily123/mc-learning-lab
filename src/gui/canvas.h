#pragma once
#include "uiElement.h"
class Canvas : public UIElement
{
public:
    Canvas(float width,float height);

private:
    std::vector<std::unique_ptr<UIElement>> childElements = {};
};