#pragma once
#include "uiElement.h"
class Text : public UIElement
{
public:
    Text(PivotType pivotType, FlexType flexType, float x, float y, float width, float height, const std::string& textureId = "font");
    void SetText(const std::string& content) { this->content = content; };
    [[nodiscard]] std::string GetText() const override { return content; };

    std::string GetTextureId() override { return textureId; };
private:
    std::string textureId;
    std::string content;
};