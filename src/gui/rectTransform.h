#pragma once
#include <glm/glm.hpp>
enum class PivotType : unsigned char
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
enum class FlexType : unsigned char
{
    None,
    Horizontal,
    Vertical,
    Full
};

struct CenterTransform
{
    float posX, posY, posZ, width, height;
};
struct AnchorTransform
{
    float posZ, left, right, top, bottom;
};
struct Anchors
{
    float minX, minY, maxX, maxY;
};
class RectTransform
{
public:
    RectTransform(PivotType pivotType, FlexType flexType, float x, float y, float width, float height);
    PivotType pivotType;
    FlexType flexType;
    float x, y, width, height;
    Anchors anchors;
    glm::vec2 pivot;
    glm::vec2 GetPosition() const;
};