#include "uiSystem.h"
#include <iostream>
void UISystem::ResizeScreen(float width, float height)
{
    uiRenderer.ResizeScreen(width, height);
    mainCanvas.SetCenterPosition(width / 2, height / 2);
}
void UISystem::RenderAll()
{
    for (auto element : mainCanvas.GetChildElement())
    {
        auto pos = element->GetStartPointPosition() + mainCanvas.GetCenterPosition();
        auto size = element->GetSize();
        auto uv = element->GetUV();
        uiRenderer.DrawQuad(pos.x, pos.y, size.x, size.y, uv.x, uv.y, uv.z - uv.x, uv.y - uv.w, element->GetTextureID());
    }
}
Canvas &UISystem::GetMainCanvas()
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