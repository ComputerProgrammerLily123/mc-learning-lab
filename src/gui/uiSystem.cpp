#include "uiSystem.h"

#include "itemIcon.h"

void UISystem::ResizeScreen(unsigned width, unsigned height)
{
    screenWidth = width;
    screenHeight = height;
    uiRenderer.ResizeScreen(width, height);
}
void UISystem::RenderAll()
{
    if (!mainCanvas) return;
    uiRenderer.ClearRenderQueue();
    for (const auto& childElement : mainCanvas->GetChildElements())
    {
        CollectRenderInfo(childElement);
    }
    uiRenderer.Draw();
}
void UISystem::SetMainCanvas(std::unique_ptr<Canvas> canvas)
{
    eventSystem.Reset();
    mainCanvas = std::move(canvas);
}
void UISystem::ClearMainCanvas()
{
    eventSystem.Reset();
    mainCanvas.reset();
}
Canvas* UISystem::GetMainCanvas() const
{
    return mainCanvas.get();
}
void UISystem::OnUpdate()
{
    if (mainCanvas == nullptr) return;
    UIScreenRect rootRect{
        {0.0f, 0.0f},
        {static_cast<float>(screenWidth),
         static_cast<float>(screenHeight)}};
    layoutSystem.Layout(*mainCanvas, rootRect);
    eventSystem.Update(*mainCanvas);
}
void UISystem::CollectRenderInfo(const std::unique_ptr<UIElement>& element)
{
    if (!element->isVisible) return;
    const auto& rect = element->GetScreenRect();
    auto textureId = element->GetTextureId();
    if (const auto* icon = dynamic_cast<const ItemIcon*>(element.get()))
    {
        uiRenderer.AddItemRenderQueue(icon->GetStack(), rect);
    }
    else if (textureId == ResourceLocation("font/ascii"))
    {
        uiRenderer.AddTextRenderQueue(rect.position.x, rect.position.y, rect.size.x, rect.size.y, ResourceLocation(textureId), element->GetText());
    }
    else
    {
        uiRenderer.AddRenderQueue(rect.position.x, rect.position.y, rect.size.x, rect.size.y, ResourceLocation(textureId));
    }
    for (const auto& childElement : element->GetChildElements())
    {
        CollectRenderInfo(childElement);
    }
}