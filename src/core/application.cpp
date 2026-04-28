#include "application.h"

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>

#include "window.h"
#include "renderer.h"
#include "uiSystem.h"
#include "camera.h"
#include "world.h"
#include "player.h"
Application::Application() : screenWidth(1200), screenHeight(800), window(nullptr), camera(nullptr), renderer(nullptr), uiSystem(nullptr), world(nullptr), player(nullptr)
{
    InitGLFW();
    InitGLAD();
    InitGLFWCallback();
    InitGame();
}
Application::~Application()
{
    delete renderer;
    delete camera;
    delete world;
    delete window;
}
void Application::InitGLFW()
{
    if (!glfwInit())
    {
        std::cout << "Failed to initialize GLFW" << std::endl;
        return;
    }
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    window = new Window(screenWidth, screenHeight, "Minecraft:Papyrus Edition");
    window->SetCursorState(false);
}
void Application::InitGLAD()
{
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cout << "Failed to initialize GLAD" << std::endl;
        return;
    }
}
void Application::InitGLFWCallback()
{
    window->SetCursorPosCallback(Input::GetInstance().MouseMoveCallback);
    window->SetKeyCallback(Input::GetInstance().KeyboardCallback);
    window->SetMouseButtonCallback(Input::GetInstance().MouseButtonCallback);
}
void Application::InitGame()
{
    RegisterBlocks();
    camera = new Camera(screenWidth, screenHeight);
    world = new World(WORLD_SIZE);
    renderer = new Renderer(WORLD_SIZE);
    uiSystem = new UISystem(screenWidth,screenHeight);
    player = new Player(glm::vec3(10.0f, 60.0f, 10.0f),camera,world);
    for (unsigned a = 0; a < 16; a++)
    {
        for (unsigned b = 0; b < 16; b++)
        {
            world->CreateChunk(a, b);
        }
    }
    world->InitChunk();
}
void Application::RegisterBlocks()
{
    auto &blockRegister = BlockRegister::GetInstance();
    blockRegister.RegisterBlock(1, {{1, 1, 1, 1, 0, 2}});
    blockRegister.RegisterBlock(2, {{2, 2, 2, 2, 2, 2}});
    blockRegister.RegisterBlock(3, {{3, 3, 3, 3, 3, 3}});
}
void Application::Run()
{
    while (!glfwWindowShouldClose(window->GetNativeWindow()))
    {
        tick.UpdateTimer();
        if (tick.ShouldTick())
        {
            player->UpdateTick();
            tick.ConsumeTick();
        }
        camera->SetPosition((player->GetPreviousTickPosition() * (1 - tick.GetAlpha()) + player->GetPosition() * tick.GetAlpha())+glm::vec3(0.0f,1.62f,0.0f));
        time.UpdateTime();
        if (input.IsKeyDown(GLFW_KEY_ESCAPE))
            window->SetShouldClose();
        glfwGetFramebufferSize(window->GetNativeWindow(), &screenWidth, &screenHeight);
        camera->ResizeScreen(screenWidth, screenHeight);
        glm::vec2 delta = input.GetMouseDeltaPosition();
        camera->ProcessMouseMovement(delta.x, -delta.y);

        renderer->Draw(world, camera);
        uiSystem->DrawQuad(screenWidth/2 - 16, screenHeight/2 - 16,32.0f,32.0f,0.0f,1.0f,0.05859375f,0.05859375f);
        input.UpdateMousePosition();
        input.UpdateKey();
        glfwSwapBuffers(window->GetNativeWindow());
        glfwPollEvents();
    }
}
void Application::Quit()
{
    glfwTerminate();
}