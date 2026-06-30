#include "rectTransform.h"
RectTransform::RectTransform(PivotType pivotType, FlexType flexType, float x, float y, float width, float height) : pivotType(pivotType), flexType(flexType), x(x), y(y), width(width), height(height)
{
}
glm::vec2 RectTransform::GetPosition()
{
    switch (pivotType)
    {
    case PivotType::Center:
        return glm::vec2(x - width / 2, y - height / 2);
    case PivotType::BottomMiddle:
        return glm::vec2(x - width / 2, y - height / 2);
    default:
        return glm::vec2(0, 0);
    }
}