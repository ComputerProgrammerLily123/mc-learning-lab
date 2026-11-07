#include "text.h"
Text::Text(PivotType pivotType, FlexType flexType, float x, float y, float width, float height, const ResourceLocation& textureId) : UIElement(pivotType, flexType, x, y, width, height), textureId(textureId)
{
}