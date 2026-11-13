#include "uiElement.h"
UIElement::UIElement(PivotType pivotType, float x, float y, float width, float height)
{
    const glm::vec2 normalizedPosition = PivotToPosition(pivotType);

    rectTransform.anchor = normalizedPosition;
    rectTransform.pivot = normalizedPosition;
    rectTransform.anchoredPosition = {x, y};
    rectTransform.size = {width, height};
}
void UIElement::AddChild(std::unique_ptr<UIElement> child)
{
    child->parent = this;
    children.push_back(std::move(child));
}
void UIElement::ClearElements()
{
    children.clear();
}
const UIScreenRect& UIElement::GetScreenRect() const
{
    return screenRect;
}
const std::vector<std::unique_ptr<UIElement>>& UIElement::GetChildElements() const
{
    return children;
}
void UIElement::SetAnchoredPosition(const glm::vec2& position)
{
    rectTransform.anchoredPosition = position;
    layoutDirty = true;
}