#pragma once
#include "uiElement.h"
#include <string>
#include "core/eventSystem.h"
#include <functional>
#include <vector>
enum class ButtonState
{
    Normal,
    Highlight,
    Disable
};
class Button : public UIElement
{
public:
    Button(PivotType pivotType, FlexType flexType, float x, float y, float width, float height, glm::vec4 normalUV, glm::vec4 highlightUV, std::string textureID);
    void AddListener(std::function<void()> listener);
    glm::vec4 GetUV() override;
    std::string GetTextureID() override;
    void OnPointerEnter() override;
    void OnPointerExit() override;
    void OnPointerClick() override;

private:
    glm::vec4 uv,normalUV, highlightUV;
    std::string textureID;
    std::vector<std::function<void()>> onClick;
    void SetButtonState(ButtonState state);

protected:
};