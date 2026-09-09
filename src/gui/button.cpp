#include "button.h"
#include "text.h"
Button::Button(PivotType pivotType, FlexType flexType, float x, float y, float width, float height, const std::string& normalTextureId, const std::string& highlightedTextureId) : UIElement(pivotType, flexType, x, y, width, height), normalTextureId(normalTextureId), highlightedTextureId(highlightedTextureId)
{
    //childElements.push_back(std::make_unique<Text>(pivotType, flexType, x, y, width, height, normalTextureId, highlightedTextureId));
}
void Button::AddListener(std::function<void()> listener)
{
    onClick.push_back(listener);
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
        //uv = normalUV;
        break;
    case ButtonState::Highlight:
        //uv = highlightUV;
        break;
    }
}