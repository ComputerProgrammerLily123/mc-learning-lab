#pragma once
#include "uiElement.h"
class Text : public UIElement
{
public:
    Text(PivotType pivotType, FlexType flexType, float x, float y, float width, float height, const ResourceLocation& textureId = ResourceLocation("font/ascii"));
    void SetText(const std::string& content) { this->content = content; };
    [[nodiscard]] const std::string GetText() const override { return content; };

    ResourceLocation GetTextureId() override { return textureId; };
private:
    ResourceLocation textureId;
    std::string content;
};