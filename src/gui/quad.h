#pragma once
#include "uiElement.h"
class Quad : public UIElement
{
public:
    Quad(PivotType pivotType, FlexType flexType, float x, float y, float width, float height, const std::string& textureId);
    std::string GetTextureId() override { return textureId; };
private:
    std::string textureId;
};