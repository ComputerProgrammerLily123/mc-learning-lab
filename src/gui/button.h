#pragma once
#include "uiElement.h"
#include <string>
//include "core/eventSystem.h"
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
    Button(PivotType pivotType, FlexType flexType, float x, float y, float width, float height, const std::string& normalTextureId, const std::string& highlightedTextureId);
    void AddListener(std::function<void()> listener);
    std::string GetTextureId() override { return normalTextureId; };
    void OnPointerEnter() override;
    void OnPointerExit() override;
    void OnPointerClick() override;

private:
    std::string normalTextureId, highlightedTextureId;
    std::vector<std::function<void()>> onClick;
    void SetButtonState(ButtonState state);

protected:
};