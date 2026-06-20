#pragma once
#include "uiRenderer.h"
#include "gameConstant.h"
using namespace GameConstant;
class UISystem
{
public:
    UISystem(const UISystem &) = delete;
    UISystem &operator=(const UISystem &) = delete;
    static UISystem &GetInstance()
    {
        static UISystem instance;
        return instance;
    }
    void CreateButton(int startX, int startY, int width, int height, float u, float v, float uvWidth, float uvHeight, std::string textureID);
    void CreateQuad(int startX, int startY, int width, int height, float u, float v, float uvWidth, float uvHeight, std::string textureID);
private:
    UISystem() = default;
    ~UISystem() = default;
    UIRenderer uiRenderer = UIRenderer(INITIAL_SCREEN_WIDTH, INITIAL_SCREEN_HEIGHT);
};