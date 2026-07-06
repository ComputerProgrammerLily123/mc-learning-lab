#include "canvas.h"
#include "config/gameConstant.h"
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
    return glm::vec2(GameConstant::INITIAL_SCREEN_WIDTH / 2, GameConstant::INITIAL_SCREEN_HEIGHT / 2);
}
void Canvas::ClearElements()
{
    childElements.clear();
}