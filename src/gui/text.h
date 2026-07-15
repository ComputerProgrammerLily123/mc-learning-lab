#pragma once
#include "uiElement.h"
class Text : public UIElement
{
public:
    Text(PivotType pivotType, FlexType flexType, float x, float y, float width, float height, glm::vec4 uv, std::string textureID);
    glm::vec4 GetUV() override;
    std::string GetTextureID() override;
    glm::vec4 uv;
    std::string textureID;
private:
};