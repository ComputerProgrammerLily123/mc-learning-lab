#ifndef APPLICATION_H
#define APPLICATION_H

#define WORLD_SIZE 64

#include "input.h"
#include "time.h"

class Window;
class Renderer;
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
    int screenWidth;
    int screenHeight;

    Window *window;
    Renderer *renderer;
    UISystem * uiSystem;
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
};
#endif