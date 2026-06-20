#pragma once

#define RENDER_RADIAN 6

#include "input.h"
#include "timeSystem.h"
#include "uiSystem.h"
#include "gameConstant.h"

class Window;
class OutlineRenderer;
class WorldRenderer;
class SkyboxRenderer;
class UIRenderer;
class Camera;
class World;
class Player;
using namespace GameConstant;
class Application
{
public:
    Application();
    ~Application();
    Application(const Application &) = delete;
    Application &operator=(const Application &) = delete;

    void Run();
    void Quit();

private:
    int screenWidth = INITIAL_SCREEN_WIDTH;
    int screenHeight = INITIAL_SCREEN_HEIGHT;

    Window *window;
    OutlineRenderer *outlineRenderer;
    WorldRenderer *worldRenderer;
    SkyboxRenderer *skyboxRenderer;
    Camera *camera;
    World *world;
    Player *player;

    Input &input = Input::GetInstance();
    TimeSystem &time = TimeSystem::GetInstance();
    TickSystem &tick = TickSystem::GetInstance();
    void InitGLFW();
    void InitGLAD();
    void InitGLFWCallback();
    void InitGame();
    void RegisterBlocks();
    void RegisterItems();
    void OnWindowResize(unsigned width, unsigned height);
};