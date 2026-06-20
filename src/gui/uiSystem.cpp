#include "uiSystem.h"
void UISystem::CreateButton(int startX, int startY, int width, int height, float u, float v, float uvWidth, float uvHeight, std::string textureID)
{
    uiRenderer.CreateQuad(startX,startY,width,height,u,v,uvWidth,uvHeight,textureID);
}
void UISystem::CreateQuad(int startX, int startY, int width, int height, float u, float v, float uvWidth, float uvHeight, std::string textureID){
    uiRenderer.CreateQuad(startX,startY,width,height,u,v,uvWidth,uvHeight,textureID);
}