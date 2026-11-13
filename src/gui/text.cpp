#include "text.h"
Text::Text(PivotType pivotType, float x, float y, float width, float height, const ResourceLocation& textureId) : UIElement(pivotType, x, y, width, height), textureId(textureId)
{
}