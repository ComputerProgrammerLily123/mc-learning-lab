#include "uiElement.h"
UIElement::UIElement(PivotType pivotType, FlexType flexType, float x, float y, float width, float height) : rectTransform(pivotType, flexType, x, y, width, height)
{
}
void UIElement::AddElement(std::unique_ptr<UIElement> uiElement)
{
    childElements.push_back(std::move(uiElement));
}
void UIElement::ClearElements()
{
    childElements.clear();
}
const std::vector<std::unique_ptr<UIElement>>& UIElement::GetChildElements() const
{
    return childElements;
}
glm::vec2 UIElement::GetStartPointPosition() const
{
    return {rectTransform.GetPosition()};
}
glm::vec2 UIElement::GetCenterPosition() const
{
    return {rectTransform.x, rectTransform.y};
}
void UIElement::SetCenterPosition(float x, float y)
{
    rectTransform.x = x;
    rectTransform.y = y;
}
glm::vec2 UIElement::GetSize() const
{
    return {rectTransform.width, rectTransform.height};
}
void UIElement::SetSize(float width, float height)
{
    rectTransform.width = width;
    rectTransform.height = height;
}