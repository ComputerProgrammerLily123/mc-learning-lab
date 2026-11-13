#pragma once

#include <string>
#include <memory>
#include "rectTransform.h"
#include "resource/resourceLocation.h"

class UIElement
{
public:
    UIElement(const UIElement&) = delete;
    UIElement(UIElement&&) = delete;
    UIElement& operator=(const UIElement&) = delete;
    UIElement& operator=(UIElement&&) = delete;

    UIElement(PivotType pivotType, float x, float y, float width = 100.0f, float height = 100.0f);
    virtual ~UIElement() = default;

    bool isVisible = true;
    bool enable = true;
    bool raycastTarget = false;

    void AddChild(std::unique_ptr<UIElement> child);

    [[nodiscard]] const UIScreenRect& GetScreenRect() const;

    void ClearElements();
    [[nodiscard]] const std::vector<std::unique_ptr<UIElement>>& GetChildElements() const;

    void SetAnchoredPosition(const glm::vec2& position);

    virtual ResourceLocation GetTextureId() { return ResourceLocation(""); };
    [[nodiscard]] virtual const std::string GetText() const { return {}; };
    virtual void OnPointerEnter() {}
    virtual void OnPointerExit() {}
    virtual void OnPointerClick() {}
    virtual void OnPointerDown() {}
    virtual void OnPointerUp() {}
    virtual void OnScroll(double delta) {}

private:
    UIElement* parent = nullptr;
    std::vector<std::unique_ptr<UIElement>> children;
    RectTransform rectTransform;
    UIScreenRect screenRect;
    bool layoutDirty = true;

    friend class UILayoutSystem;
};