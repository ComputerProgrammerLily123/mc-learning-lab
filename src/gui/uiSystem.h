#pragma once

#include "canvas.h"
#include "config/gameConstant.h"
#include "render/uiRenderer.h"

using namespace GameConstant;
class UISystem
{
public:
    UISystem(UISystem&&) = delete;
    UISystem& operator=(UISystem&&) = delete;
    UISystem(const UISystem&) = delete;
    UISystem& operator=(const UISystem&) = delete;
    static UISystem& GetInstance()
    {
        static UISystem instance;
        return instance;
    }
    void ResizeScreen(unsigned width, unsigned height);
    void RenderAll();
    void SetMainCanvas(Canvas* canvas);
    Canvas* GetMainCanvas() const;
    void CreateButton(int startX, int startY, int width, int height, float u, float v, float uvWidth, float uvHeight, std::string textureID);
    void CreateQuad(int startX, int startY, int width, int height, float u, float v, float uvWidth, float uvHeight, std::string textureID);
    void OnUpdate();

private:
    UISystem() = default;
    ~UISystem() = default;
    UIRenderer uiRenderer = UIRenderer(INITIAL_SCREEN_WIDTH, INITIAL_SCREEN_HEIGHT);
    Canvas* mainCanvas = nullptr;
};