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
inline glm::vec2 PivotToPosition(PivotType type)
{
    switch (type)
    {
    case PivotType::TopLeft:      return {0.0f, 0.0f};
    case PivotType::TopMiddle:    return {0.5f, 0.0f};
    case PivotType::TopRight:     return {1.0f, 0.0f};
    case PivotType::MiddleLeft:   return {0.0f, 0.5f};
    case PivotType::Center:       return {0.5f, 0.5f};
    case PivotType::MiddleRight:  return {1.0f, 0.5f};
    case PivotType::BottomLeft:   return {0.0f, 1.0f};
    case PivotType::BottomMiddle: return {0.5f, 1.0f};
    case PivotType::BottomRight:  return {1.0f, 1.0f};
    }
    return {0.5f, 0.5f};
}

struct UIScreenRect
{
    glm::vec2 position {};
    glm::vec2 size {};

    [[nodiscard]] bool Contain(const glm::vec2& point) const
    {
        return point.x >= position.x && point.y >= position.y && point.x < position.x + size.x && point.y < position.y + size.y;
    }
};
struct RectTransform
{
    glm::vec2 anchor {0.5f, 0.5f};
    glm::vec2 pivot {0.5f, 0.5f};
    glm::vec2 anchoredPosition {};
    glm::vec2 size {100.0f, 100.0f};
};