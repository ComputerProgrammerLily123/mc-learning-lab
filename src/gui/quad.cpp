#include "quad.h"
Quad::Quad(PivotType pivotType, FlexType flexType, float x, float y, float width, float height, glm::vec4 uv, std::string textureID) : UIElement(pivotType, flexType, x, y, width, height), uv(uv),textureID(textureID)
{
}
glm::vec4 Quad::GetUV()
{
    return uv;
}
std::string Quad::GetTextureID()
{
    return textureID;
}