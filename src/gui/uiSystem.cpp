#include "uiSystem.h"

#include "GLFW/glfw3.h"
#include "core/input.h"

#include <iostream>
void UISystem::ResizeScreen(unsigned width, unsigned height)
{
    uiRenderer.ResizeScreen(width, height);
}
void UISystem::RenderAll()
{
    for (const auto& element : mainCanvas->GetChildElements())
    {
        auto pos = element->GetStartPointPosition() + mainCanvas->GetCenterPosition();
        auto size = element->GetSize();
        auto uv = element->GetUV();
        uiRenderer.DrawQuad(pos.x, pos.y, size.x, size.y, uv.x, uv.y, uv.z - uv.x, uv.y - uv.w, element->GetTextureID());
    }
}
void UISystem::SetMainCanvas(Canvas* canvas)
{
    mainCanvas = canvas;
}
Canvas* UISystem::GetMainCanvas() const
{
    return mainCanvas;
}
void UISystem::CreateButton(int startX, int startY, int width, int height, float u, float v, float uvWidth, float uvHeight, std::string textureID)
{
    uiRenderer.DrawQuad(startX, startY, width, height, u, v, uvWidth, uvHeight, textureID);
}
void UISystem::CreateQuad(int startX, int startY, int width, int height, float u, float v, float uvWidth, float uvHeight, std::string textureID)
{
    uiRenderer.DrawQuad(startX, startY, width, height, u, v, uvWidth, uvHeight, textureID);
}
void UISystem::OnUpdate()
{
    glm::vec2 mousePos = Input::GetInstance().GetMouseCurrentPosition();
    mainCanvas = SceneManager::GetInstance().GetObject<Canvas>("Canvas");
    if(mainCanvas == nullptr) return;
    const auto& childElements = mainCanvas->GetChildElements();
    for (const auto& element : childElements)
    {
        if (element == nullptr) continue;
        auto pos = element->GetStartPointPosition() + mainCanvas->GetCenterPosition();
        auto size = element->GetSize();
        if (mousePos.x > pos.x && mousePos.y > pos.y && mousePos.x < pos.x + size.x && mousePos.y < pos.y + size.y)
        {
            element->OnPointerEnter();
            if (Input::GetInstance().IsMouseButtonDown(GLFW_MOUSE_BUTTON_1))
            {
                element->OnPointerClick();
            }
        }
        else
        {
            element->OnPointerExit();
        }
    }
}