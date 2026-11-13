#include "uiEventSystem.h"

#include "core/input.h"
void UIEventSystem::Update(UIElement& root)
{
    Input& input = Input::GetInstance();
    const glm::vec2 mousePosition = input.GetMouseCurrentPosition();

    UIElement* target = HitTest(root, mousePosition);

    if (target != hovered)
    {
        if (hovered) hovered->OnPointerExit();
        hovered = target;
        if (hovered) hovered->OnPointerEnter();
    }

    if (input.IsMouseButtonDown(MOUSE_BUTTON_LEFT))
    {
        pressed = hovered;
        captured = pressed;
        focused = pressed;

        if (captured) captured->OnPointerDown();
    }

    if (input.IsMouseButtonUp(MOUSE_BUTTON_LEFT))
    {
        if (captured)
        {
            captured->OnPointerUp();

            if (pressed == hovered)
                captured->OnPointerClick();
        }

        pressed = nullptr;
        captured = nullptr;
    }

    const double scroll = input.GetDeltaScroll();

    if (scroll != 0.0 && hovered)
        hovered->OnScroll(scroll);
}
void UIEventSystem::Reset()
{
    hovered = nullptr;
    pressed = nullptr;
    captured = nullptr;
    focused = nullptr;
}
UIElement* UIEventSystem::HitTest(UIElement& element, const glm::vec2& mousePosition)
{
    if (!element.isVisible || !element.enable) return nullptr;

    const auto& children = element.GetChildElements();

    for (auto iter = children.rbegin(); iter != children.rend(); ++iter)
    {
        if (UIElement* hit = HitTest(**iter, mousePosition))
            return hit;
    }

    if (element.raycastTarget && element.GetScreenRect().Contain(mousePosition))
    {
        return &element;
    }

    return nullptr;
}