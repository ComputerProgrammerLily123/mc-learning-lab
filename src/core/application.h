#ifndef APPLICATION_H
#define APPLICATION_H

#define WORLD_SIZE 6

#include "input.h"
#include "timeSystem.h"

class Window;
class OutlineRenderer;
class WorldRenderer;
class SkyboxRenderer;
class UISystem;
class Camera;
class World;
class Player;
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
    int screenWidth = 1200;
    int screenHeight = 800;

    Window *window;
    OutlineRenderer *outlineRenderer;
    WorldRenderer *worldRenderer;
    SkyboxRenderer *skyboxRenderer;
    UISystem *uiSystem;
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
#endif