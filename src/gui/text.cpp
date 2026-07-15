#include "text.h"
Text::Text(PivotType pivotType, FlexType flexType, float x, float y, float width, float height, glm::vec4 uv, std::string textureID) : UIElement(pivotType, flexType, x, y, width, height), uv(uv),textureID(textureID)
{
}
glm::vec4 Text::GetUV()
{
    return uv;
}
std::string Text::GetTextureID()
{
    return textureID;
}