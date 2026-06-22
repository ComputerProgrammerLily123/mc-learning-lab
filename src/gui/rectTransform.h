#pragma once
#include <glm/glm.hpp>
enum class PivotType
{
    TopLeft,
    TopMiddle,
    TopRight,
    MiddleLeft,
    Center,
    MiddleRight,
    BottomLeft,
    BottomMiddle,
    BottomRight
};
enum class FlexType
{
    None,
    Horizontal,
    Vertical,
    Full
};
class RectTransform
{
public:
    RectTransform(PivotType pivotType, FlexType flexType, float x, float y, float width, float height);
    PivotType pivotType;
    FlexType flexType;
    float x, y, width, height;
    glm::vec2 GetPosition();
};