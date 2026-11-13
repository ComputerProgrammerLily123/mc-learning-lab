#pragma once

#include <glm/glm.hpp>
#include "uiElement.h"

/* enum class UIEventType : unsigned char
{
    OnPointerEnter, OnPointerExit,
    OnPointerDown, OnPointerUp,
    OnClick, OnScroll
};

struct UIEvent
{
    UIEventType type;
    glm::vec2 position;
    int button = -1;
    double scroll = 0.0;
    bool handled = false;
};
 */
class UIEventSystem
{
public:
    void Update(UIElement& root);
    void Reset();

private:
    UIElement* hovered = nullptr;
    UIElement* pressed = nullptr;
    UIElement* captured = nullptr;
    UIElement* focused = nullptr;
    UIElement* HitTest(UIElement& element, const glm::vec2& mousePosition);
};