#include "canvas.h"

Canvas::Canvas(float width, float height) : UIElement(PivotType::TopLeft, 0.0f, 0.0f, width, height)
{
    raycastTarget = false;
}