#pragma once
#include "uiElement.h"
class Quad : public UIElement
{
public:
    Quad(PivotType pivotType, FlexType flexType, float x, float y, float width, float height, const ResourceLocation& textureId);
    ResourceLocation GetTextureId() override { return textureId; };
private:
    ResourceLocation textureId;
};