#include "application.h"

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>

#include "window.h"
#include "outlineRenderer.h"
#include "worldRenderer.h"
#include "skyboxRenderer.h"
#include "uiRenderer.h"
#include "camera.h"
#include "world.h"
#include "player.h"
#include "item.h"
#include "jsonReader.h"
Application::Application()
{
    InitGLFW();
    InitGLAD();
    InitGLFWCallback();
    InitGame();
}
Application::~Application()
{
    delete outlineRenderer;
    delete worldRenderer;
    delete camera;
    delete world;
    delete window;
}
void Application::InitGLFW()
{
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
    window->SetScrollCallback(Input::GetInstance().ScrollCallback);
    window->SetWindowUserPointer(this);
    window->SetFrameBufferSizeCallback([](GLFWwindow *win, int width, int height)
                                       {      
        Application* app = static_cast<Application*>(glfwGetWindowUserPointer(win));
        if (app) app->OnWindowResize(width, height); });
}
void Application::InitGame()
{
    RegisterBlocks();
    camera = new Camera(screenWidth, screenHeight);
    world = new World();
    outlineRenderer = new OutlineRenderer();
    worldRenderer = new WorldRenderer(RENDER_RADIAN);
    skyboxRenderer = new SkyboxRenderer();
    player = new Player(glm::vec3(10.0f, 100.0f, 10.0f), camera, world);
}
void Application::RegisterBlocks()
{
    auto &blockRegister = BlockRegister::GetInstance();

    auto data = JsonReader::ReadJson("data/blocks.json")["blocks"];
    for (auto &block : data)
    {
        blockRegister.RegisterBlock(block["name"], block["uv"], block["texture"], block["solid"]);
    }
}
void Application::RegisterItems()
{
    auto &itemRegister = ItemRegister::GetInstance();
    itemRegister.RegisterItem(1, "grass", "");
    itemRegister.RegisterItem(2, "dirt", "");
    itemRegister.RegisterItem(3, "stone", "");
}
void Application::OnWindowResize(unsigned width, unsigned height)
{
    window->ResizeWindow(width, height);
    glfwGetFramebufferSize(window->GetNativeWindow(), &screenWidth, &screenHeight);
    camera->ResizeScreen(screenWidth, screenHeight);
    //uiSystem->ResizeScreen(screenWidth, screenHeight);
}
void Application::Run()
{
    while (!window->ShouldClose())
    {
        std::cout << "FPS:" << 1.0f/TimeSystem::GetInstance().GetDeltaTime()<<std::endl;
        // std::cout << player->GetBlockPosition().x << "," << player->GetBlockPosition().y << "," << player->GetBlockPosition().z << std::endl;
        // std::cout << player->GetChunkPosition().x << "," << player->GetChunkPosition().y << "," << player->GetChunkPosition().z << std::endl;
        tick.UpdateTimer();
        if (tick.ShouldTick())
        {
            player->UpdateTick();
            tick.ConsumeTick();
        }
        time.UpdateTime();
        if (input.IsKeyDown(GLFW_KEY_ESCAPE))
            window->SetShouldClose();
        if (input.IsKeyDown(GLFW_KEY_F11))
            window->ToggleFullscreen();

        // Main Menu
        UISystem &uiSystem = UISystem::GetInstance();
/*        camera->SetZoom(90.0f);
        skyboxRenderer->Draw(camera);
        camera->SetYaw(TimeSystem::GetInstance().GetDeltaTime() * 10);
        uiSystem.CreateButton(screenWidth / 2 - 524, screenHeight / 2 - 300, 605.46875f, 171.875f, 0.0f, 1.0f, 0.60546875f, 0.171875f, "title/minecraft.png");
        uiSystem.CreateButton(screenWidth / 2 + 78, screenHeight / 2 - 300, 464.84375f, 171.875f, 0.0f, 0.82421875f, 0.46484375, 0.171875f, "title/minecraft.png");
        uiSystem.CreateButton(screenWidth / 2 - 364, screenHeight / 2 - 40, 728.0f, 88.0f, 0.0f, 0.7421875f, 0.78125f, 0.078125f, "widgets.png");
        uiSystem.CreateButton(screenWidth / 2 - 364, screenHeight / 2 + 60, 728.0f, 88.0f, 0.0f, 0.7421875f, 0.78125f, 0.078125f, "widgets.png");
        uiSystem.CreateButton(screenWidth / 2 - 364, screenHeight / 2 + 160, 728.0f, 88.0f, 0.0f, 0.7421875f, 0.78125f, 0.078125f, "widgets.png");
        uiSystem.CreateButton(screenWidth / 2 - 364, screenHeight / 2 + 260, 348.0f, 88.0f, 0.0f, 0.7421875f, 0.78125f, 0.078125f, "widgets.png");
        uiSystem.CreateButton(screenWidth / 2 + 16, screenHeight / 2 + 260, 348.0f, 88.0f, 0.0f, 0.7421875f, 0.78125f, 0.078125f, "widgets.png");
        uiSystem.CreateButton(screenWidth / 2 - 504, screenHeight / 2 + 260, 88.0f, 88.0f, 0.0f, 0.5859375f, 0.078125f, 0.078125f, "widgets.png");
*/        
        // Game Scene
        
        camera->SetPosition((player->GetPreviousTickPosition() * (1 - tick.GetAlpha()) + player->GetPosition() * tick.GetAlpha()) + glm::vec3(0.0f, 1.62f, 0.0f));
        glm::vec2 delta = input.GetMouseDeltaPosition();
        camera->ProcessMouseMovement(delta.x, -delta.y);
        input.UpdateMousePosition();
        if (Input::GetInstance().GetDeltaScroll() > 0)
        player->GetHotBar().SetHotBarSlot(player->GetHotBar().GetHotBarSlot() - 1);
        if (Input::GetInstance().GetDeltaScroll() < 0)
        player->GetHotBar().SetHotBarSlot(player->GetHotBar().GetHotBarSlot() + 1);
        worldRenderer->Draw(world, camera, player->GetChunkPosition().x, player->GetChunkPosition().z);
        outlineRenderer->DrawOutline(world, camera, player->GetLookAtPosition());
        uiSystem.CreateQuad(screenWidth / 2 - 16, screenHeight / 2 - 16, 32.0f, 32.0f, 0.0f, 1.0f, 0.05859375f, 0.05859375f, "icons.png");
        uiSystem.CreateQuad(screenWidth / 2 - 364, screenHeight - 88, 728.0f, 88.0f, 0.0f, 1.0f, 0.7109375f, 0.0859375f, "widgets.png");
        uiSystem.CreateQuad(screenWidth / 2 - 364 + player->GetHotBar().GetHotBarSlot() * 80, screenHeight - 88, 88.0f, 88.0f, 0.0f, 0.9140625f, 0.09375f, 0.09375f, "widgets.png");

        input.UpdateKey();
        input.UpdateScroll();
        glfwSwapBuffers(window->GetNativeWindow());
        glfwPollEvents();
    }
}
void Application::Quit()
{
    glfwTerminate();
}