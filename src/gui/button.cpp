#include "button.h"
#include "text.h"
Button::Button(PivotType pivotType, float x, float y, float width, float height, const ResourceLocation& normalTextureId, const ResourceLocation& highlightedTextureId, const std::string& text) : UIElement(pivotType, x, y, width, height), currentTextureId(normalTextureId), normalTextureId(normalTextureId), highlightedTextureId(highlightedTextureId)
{
    raycastTarget = true;
    if(text == "") return;
    auto textObj = std::make_unique<Text>(PivotType::Center, 0.0f, 0.0f, 32.0f, 32.0f);
    textObj->SetText(text);
    AddChild(std::move(textObj));
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
        currentTextureId = normalTextureId;
        break;
    case ButtonState::Highlight:
        currentTextureId = highlightedTextureId;
        break;
    }
}