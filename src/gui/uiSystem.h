#pragma once

#include "uiLayoutSystem.h"
#include "uiEventSystem.h"

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
    void SetMainCanvas(std::unique_ptr<Canvas> canvas);
    void ClearMainCanvas();
    [[nodiscard]] Canvas* GetMainCanvas() const;
    void OnUpdate();

private:
    UISystem() = default;
    ~UISystem() = default;

    unsigned screenWidth = INITIAL_SCREEN_WIDTH;
    unsigned screenHeight = INITIAL_SCREEN_HEIGHT;
    std::unique_ptr<Canvas> mainCanvas = nullptr;

    UILayoutSystem layoutSystem;
    UIEventSystem eventSystem;

    UIRenderer uiRenderer = UIRenderer(INITIAL_SCREEN_WIDTH, INITIAL_SCREEN_HEIGHT);

    void CollectRenderInfo(const std::unique_ptr<UIElement>& element);
};