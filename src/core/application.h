#pragma once

#include "input.h"
#include "timeSystem.h"
#include "config/gameConstant.h"
#include "sceneManager.h"
#include "resource/resourceManager.h"
#include "window.h"

using namespace GameConstant;
class Application
{
public:
    Application();
    ~Application() = default;
    Application(const Application &) = delete;
    Application &operator=(const Application &) = delete;
    Application(Application &&) = delete;
    Application &operator=(Application &&) = delete;

    void Run();
    void Quit();

private:

    std::unique_ptr<Window> window;

    Input &input = Input::GetInstance();
    TimeSystem &time = TimeSystem::GetInstance();
    TickSystem &tick = TickSystem::GetInstance();
    ResourceManager &resourceManager = ResourceManager::GetInstance();
    SceneManager &sceneManager = SceneManager::GetInstance();

    void InitGLFW();
    void InitGLAD();
    void InitGLFWCallback();
    void InitResource();
    void InitScene();
    
    void RegisterBlocks();
    void RegisterItems();
    void OnWindowResize(unsigned newWidth, unsigned newHeight);
};