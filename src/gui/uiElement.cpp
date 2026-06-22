#include "uiElement.h"
UIElement::UIElement(PivotType pivotType, FlexType flexType, float x, float y, float width, float height) : rectTransform(pivotType, flexType, x, y, width, height)
{
}
void UIElement::AddElement(UIElement* uiElement)
{
    childElements.push_back(uiElement);
}
void UIElement::ClearElement()
{
    childElements.clear();
}
std::vector<UIElement*>& UIElement::GetChildElement()
{
    return childElements;
}
glm::vec2 UIElement::GetStartPointPosition()
{
    return rectTransform.GetPosition();
}
glm::vec2 UIElement::GetCenterPosition()
{
    return glm::vec2(rectTransform.x, rectTransform.y);
}
void UIElement::SetCenterPosition(float x, float y)
{
    rectTransform.x = x;
    rectTransform.y = y;
}
glm::vec2 UIElement::GetSize()
{
    return glm::vec2(rectTransform.width, rectTransform.height);
}
void UIElement::SetSize(float width, float height)
{
    rectTransform.width = width;
    rectTransform.height = height;
}