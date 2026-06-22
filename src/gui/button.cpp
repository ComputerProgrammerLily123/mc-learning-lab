#include "button.h"
Button::Button(PivotType pivotType, FlexType flexType, float x, float y, float width, float height, glm::vec4 uv, std::string textureID) : UIElement(pivotType, flexType, x, y, width, height), uv(uv),textureID(textureID)
{
}
glm::vec4 Button::GetUV()
{
    return uv;
}
std::string Button::GetTextureID()
{
    return textureID;
}
