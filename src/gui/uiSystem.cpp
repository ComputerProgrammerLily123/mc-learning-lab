#include "uiSystem.h"

#include "GLFW/glfw3.h"
#include "core/input.h"
#include "resource/resourceManager.h"

void UISystem::ResizeScreen(unsigned width, unsigned height)
{
    mainCanvas->Resize(width, height);
    uiRenderer.ResizeScreen(width, height);
}
void UISystem::RenderAll()
{
    for (const auto& element : mainCanvas->GetChildElements())
    {
        auto pos = element->GetStartPointPosition() + mainCanvas->GetCenterPosition();
        auto size = element->GetSize();
        auto region = ResourceManager::GetInstance().GetTextureRegion(element->GetTextureId());
        //uiRenderer.AddRenderQueue(pos.x, pos.y, size.x, size.y, uv.x, uv.y, uv.z - uv.x, uv.y - uv.w);
        uiRenderer.AddRenderQueue(pos.x, pos.y, size.x, size.y, region->GetU1(), region->GetV1(), region->GetU2(), region->GetV2());
    }
    uiRenderer.Draw();
}
void UISystem::SetMainCanvas(Canvas* canvas)
{
    mainCanvas = canvas;
}
Canvas* UISystem::GetMainCanvas() const
{
    return mainCanvas;
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