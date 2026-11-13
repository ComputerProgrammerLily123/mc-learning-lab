#pragma once

#include "uiElement.h"

class UILayoutSystem
{
public:
    void Layout(UIElement& root, const UIScreenRect& rootRect);

private:
    void LayoutRecursive(UIElement& element, const UIScreenRect& parentRect);
};