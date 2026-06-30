#include "button.h"
Button::Button(PivotType pivotType, FlexType flexType, float x, float y, float width, float height, glm::vec4 normalUV, glm::vec4 highlightUV, std::string textureID) : UIElement(pivotType, flexType, x, y, width, height), uv(normalUV), normalUV(normalUV), highlightUV(highlightUV), textureID(textureID)
{
}
void Button::AddListener(std::function<void()> listener)
{
    onClick.push_back(listener);
}
glm::vec4 Button::GetUV()
{
    return uv;
}
std::string Button::GetTextureID()
{
    return textureID;
}
void Button::OnPointerEnter()
{
    SetButtonState(ButtonState::Highlight);
}
void Button::OnPointerExit()
{
    SetButtonState(ButtonState::Normal);
}
void Button::OnPointerClick()
{
    for(auto& event : onClick){
        event();
    }
}
void Button::SetButtonState(ButtonState state)
{
    switch (state)
    {
    case ButtonState::Normal:
        uv = normalUV;
        break;
    case ButtonState::Highlight:
        uv = highlightUV;
        break;
    }
}