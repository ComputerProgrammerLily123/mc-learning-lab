#pragma once

#include <string>
#include <memory>
#include "rectTransform.h"

class UIElement
{
public:
    UIElement(const UIElement&) = delete;
    UIElement(UIElement&&) = delete;
    UIElement& operator=(const UIElement&) = delete;
    UIElement& operator=(UIElement&&) = delete;
    UIElement(PivotType pivotType, FlexType flexType, float x, float y, float width = 100.0f, float height = 100.0f);
    virtual ~UIElement() = default;
    [[nodiscard]] glm::vec2 GetStartPointPosition() const;
    [[nodiscard]] glm::vec2 GetCenterPosition() const;
    void SetCenterPosition(float x, float y);
    [[nodiscard]] glm::vec2 GetSize() const;
    void SetSize(float width, float height);
    void AddElement(std::unique_ptr<UIElement> uiElement);
    void ClearElements();
    [[nodiscard]] const std::vector<std::unique_ptr<UIElement>>& GetChildElements() const;

    virtual glm::vec4 GetUV() { return {0, 0, 0, 0}; }
    virtual std::string GetTextureID() { return ""; }
    virtual void OnPointerEnter() {}
    virtual void OnPointerExit() {}
    virtual void OnPointerClick() {}

protected:
    RectTransform rectTransform;
    std::vector<std::unique_ptr<UIElement>> childElements = {};
};