#include "canvas.h"

Canvas::Canvas(unsigned width, unsigned height) : width(width), height(height) {}
const std::vector<std::unique_ptr<UIElement>>& Canvas::GetChildElements() const
{
    return childElements;
}
void Canvas::AddElement(std::unique_ptr<UIElement> element)
{
    childElements.push_back(std::move(element));
}
const glm::vec2 Canvas::GetCenterPosition() const
{
    return glm::vec2(width / 2, height / 2);
}
void Canvas::ClearElements()
{
    childElements.clear();
}
void Canvas::Resize(unsigned width, unsigned height)
{
    this->width = width;
    this->height = height;
}