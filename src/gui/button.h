#pragma once
#include "uiElement.h"
#include <string>
#include <functional>
#include <vector>
enum class ButtonState : unsigned char
{
    Normal,
    Highlight,
    Disable
};
class Button : public UIElement
{
public:
    Button(const Button&) = delete;
    ~Button() = default;
    Button(Button&&) = delete;
    Button& operator=(const Button&) = delete;
    Button& operator=(Button&&) = delete;
    Button(PivotType pivotType, float x, float y, float width, float height, const ResourceLocation& normalTextureId, const ResourceLocation& highlightedTextureId, const std::string& text = "");
    void AddListener(std::function<void()> listener);
    ResourceLocation GetTextureId() override { return currentTextureId; };
    void OnPointerEnter() override;
    void OnPointerExit() override;
    void OnPointerClick() override;

private:
    ResourceLocation currentTextureId, normalTextureId, highlightedTextureId;
    std::vector<std::function<void()>> onClick;
    void SetButtonState(ButtonState state);

protected:
};