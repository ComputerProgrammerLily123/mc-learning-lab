#pragma once
#include "core/sceneManager.h"
#include "uiElement.h"
class Canvas : public SceneObject
{
public:
    [[nodiscard]] const std::vector<std::unique_ptr<UIElement>>& GetChildElements() const;
    void AddElement(std::unique_ptr<UIElement> element);
    [[nodiscard]] const glm::vec2 GetCenterPosition() const;
    void ClearElements();

private:
    std::vector<std::unique_ptr<UIElement>> childElements = {};
};