#include "uiLayoutSystem.h"

void UILayoutSystem::Layout(UIElement& root, const UIScreenRect& rootRect)
{
    root.screenRect = rootRect;
    root.layoutDirty = false;

    for (const auto& child : root.children)
    {
        LayoutRecursive(*child, rootRect);
    }
}
void UILayoutSystem::LayoutRecursive(UIElement& element, const UIScreenRect& parentRect)
{
    const RectTransform& transform = element.rectTransform;
    const glm::vec2 anchorPosition = parentRect.position + parentRect.size * transform.anchor;
    element.screenRect.position = anchorPosition + transform.anchoredPosition - transform.size * transform.pivot;
    element.screenRect.size = transform.size;
    element.layoutDirty = false;
    for (const auto& child : element.children)
    {
        LayoutRecursive(*child, element.screenRect);
    }
}