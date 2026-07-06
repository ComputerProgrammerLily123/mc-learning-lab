#include "application.h"

#include <GLFW/glfw3.h>
#include <glad/glad.h>

#include <iostream>

#include "gui/button.h"
#include "gui/canvas.h"
#include "gui/quad.h"
#include "gui/uiSystem.h"
// #include "item/itemRegister.h"
#include "player/player.h"
#include "render/camera.h"
#include "render/outlineRenderer.h"
#include "render/skyboxRenderer.h"
#include "render/worldRenderer.h"
#include "util/jsonReader.h"
#include "window.h"
#include "world/world.h"

Application::Application()
{
    InitGLFW();
    InitGLAD();
    InitGLFWCallback();
    InitResource();
    InitScene();
}
void Application::InitGLFW()
{
    window = std::make_unique<Window>(screenWidth, screenHeight, "Minecraft:Papyrus Edition");
}
void Application::InitGLAD()
{
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cout << "Failed to initialize GLAD\n";
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
    window->SetFrameBufferSizeCallback([](GLFWwindow* win, int width, int height)
                                       {      
        auto app = static_cast<Application*>(glfwGetWindowUserPointer(win));
        if (app) app->OnWindowResize(width, height); });
}
void Application::InitScene()
{
    sceneManager.CreateScene("Welcome");
    sceneManager.CreateScene("Main");

    // Welcome Scene
    auto welcomeScene = sceneManager.GetScene("Welcome");
    welcomeScene->onLoad = [this]
    {
        auto canvas = std::make_unique<Canvas>();
        auto camera = std::make_unique<Camera>(screenWidth, screenHeight);
        auto skyboxRenderer = std::make_unique<SkyboxRenderer>();

        camera->SetZoom(90.0f);

        auto titleA = std::make_unique<Quad>(PivotType::Center, FlexType::None, -225, -215, 605.46875f, 171.875f, glm::vec4(0.0f, 1.0f, 0.60546875f, 0.828125f), "title/minecraft.png");
        auto titleB = std::make_unique<Quad>(PivotType::Center, FlexType::None, 305, -215, 464.84375f, 171.875f, glm::vec4(0.0f, 0.82421875f, 0.46484375f, 0.65234375f), "title/minecraft.png");
        auto buttonA = std::make_unique<Button>(PivotType::Center, FlexType::None, 0, 0, 728.0f, 88.0f, glm::vec4(0.0f, 0.7421875f, 0.78125f, 0.6640625f), glm::vec4(0.0f, 0.6640625f, 0.78125f, 0.5859375f), "widgets.png");
        auto buttonB = std::make_unique<Button>(PivotType::Center, FlexType::None, 0, 100, 728.0f, 88.0f, glm::vec4(0.0f, 0.7421875f, 0.78125f, 0.6640625f), glm::vec4(0.0f, 0.6640625f, 0.78125f, 0.5859375f), "widgets.png");
        auto buttonC = std::make_unique<Button>(PivotType::Center, FlexType::None, 0, 200, 728.0f, 88.0f, glm::vec4(0.0f, 0.7421875f, 0.78125f, 0.6640625f), glm::vec4(0.0f, 0.6640625f, 0.78125f, 0.5859375f), "widgets.png");
        auto buttonD = std::make_unique<Button>(PivotType::Center, FlexType::None, -450, 300, 88.0f, 88.0f, glm::vec4(0.0f, 0.5859375f, 0.078125f, 0.5078125f), glm::vec4(0.0f, 0.5078125f, 0.078125f, 0.4296875f), "widgets.png");
        buttonA->AddListener([this]
                             { sceneManager.LoadScene("Main"); });
        buttonC->AddListener([this]
                             { Quit(); });
        canvas->ClearElements();
        canvas->AddElement(std::move(titleA));
        canvas->AddElement(std::move(titleB));
        canvas->AddElement(std::move(buttonA));
        canvas->AddElement(std::move(buttonB));
        canvas->AddElement(std::move(buttonC));
        canvas->AddElement(std::move(buttonD));
        UISystem::GetInstance().SetMainCanvas(canvas.get());
        sceneManager.AddObject("Canvas", std::move(canvas));
        sceneManager.AddObject("Camera", std::move(camera));
        sceneManager.AddObject("SkyboxRenderer", std::move(skyboxRenderer));

        window->SetCursorState(true);
    };
    welcomeScene->onUpdate = [this]
    {
        auto camera = sceneManager.GetObject<Camera>("Camera");
        auto skyboxRenderer = sceneManager.GetObject<SkyboxRenderer>("SkyboxRenderer");
        camera->SetYaw(time.GetDeltaTime() * 10);
        skyboxRenderer->Draw(camera);
    };
    welcomeScene->onUnLoad = [this]
    {
        UISystem::GetInstance().SetMainCanvas(nullptr);
        sceneManager.ClearObjects();
    };

    // Main Scene
    auto mainScene = sceneManager.GetScene("Main");
    mainScene->onLoad = [this]
    {
        auto canvas = std::make_unique<Canvas>();
        auto camera = std::make_unique<Camera>(screenWidth, screenHeight);
        auto world = std::make_unique<World>();
        auto outlineRenderer = std::make_unique<OutlineRenderer>();
        auto worldRenderer = std::make_unique<WorldRenderer>(RENDER_RADIAN);
        auto player = std::make_unique<Player>(glm::vec3(10.0f, 100.0f, 10.0f), camera.get(), world.get());

        auto crosshair = std::make_unique<Quad>(PivotType::Center, FlexType::None, 0, 0, 32.0f, 32.0f, glm::vec4(0.0f, 1.0f, 0.05859375f, 0.94140625f), "icons.png");
        auto hotbar = std::make_unique<Quad>(PivotType::BottomMiddle, FlexType::None, 0, 350, 728.0f, 88.0f, glm::vec4(0.0f, 1.0f, 0.7109375f, 0.9140625f), "widgets.png");
        auto hotbarFrame = std::make_unique<Quad>(PivotType::Center, FlexType::None, player->GetHotBar().GetHotBarSlot() * 80, 350, 88.0f, 88.0f, glm::vec4(0.0f, 0.9140625f, 0.09375f, 0.8203125f), "widgets.png");
        canvas->AddElement(std::move(crosshair));
        canvas->AddElement(std::move(hotbar));
        canvas->AddElement(std::move(hotbarFrame));
        UISystem::GetInstance().SetMainCanvas(canvas.get());

        sceneManager.AddObject("Canvas", std::move(canvas));
        sceneManager.AddObject("Camera", std::move(camera));
        sceneManager.AddObject("World", std::move(world));
        sceneManager.AddObject("OutlineRenderer", std::move(outlineRenderer));
        sceneManager.AddObject("WorldRenderer", std::move(worldRenderer));
        sceneManager.AddObject("Player", std::move(player));

        window->SetCursorState(false);
    };
    mainScene->onUpdate = [this]
    {
        auto canvas = sceneManager.GetObject<Canvas>("Canvas");
        auto camera = sceneManager.GetObject<Camera>("Camera");
        auto world = sceneManager.GetObject<World>("World");
        auto outlineRenderer = sceneManager.GetObject<OutlineRenderer>("OutlineRenderer");
        auto worldRenderer = sceneManager.GetObject<WorldRenderer>("WorldRenderer");
        auto player = sceneManager.GetObject<Player>("Player");
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
        std::cout << player->GetBlockPosition().x << "," << player->GetBlockPosition().y << "," << player->GetBlockPosition().z << '\n';
        std::cout << player->GetChunkPosition().x << "," << player->GetChunkPosition().y << "," << player->GetChunkPosition().z << '\n';
    };
    mainScene->onTickUpdate = [this]
    {
        auto player = sceneManager.GetObject<Player>("Player");
        player->UpdateTick();
    };
    mainScene->onUnLoad = [this]
    {
        UISystem::GetInstance().SetMainCanvas(nullptr);
        sceneManager.ClearObjects();
    };

    sceneManager.LoadScene("Welcome");
}
void Application::InitResource()
{
    RegisterBlocks();
    resourceManager.LoadTexture();
}
void Application::RegisterBlocks()
{
    auto& blockRegister = BlockRegister::GetInstance();

    auto data = JsonReader::ReadJson("data/blocks.json")["blocks"];
    for (auto& block : data)
    {
        blockRegister.RegisterBlock(block["name"], block["uv"], block["texture"], block["solid"]);
    }
}
void Application::RegisterItems()
{
    /*
    auto &itemRegister = ItemRegister::GetInstance();
    itemRegister.RegisterItem(1, "grass", "");
    itemRegister.RegisterItem(2, "dirt", "");
    itemRegister.RegisterItem(3, "stone", "");
    */
}
void Application::OnWindowResize(unsigned width, unsigned height)
{
    window->ResizeWindow(width, height);
    glfwGetFramebufferSize(window->GetNativeWindow(), &screenWidth, &screenHeight);
    // camera->ResizeScreen(screenWidth, screenHeight);
    UISystem::GetInstance().ResizeScreen(screenWidth, screenHeight);
}
void Application::Run()
{
    while (!window->ShouldClose())
    {
        std::cout << "FPS:" << 1.0f / time.GetDeltaTime() << '\n';
        tick.UpdateTimer();
        if (tick.ShouldTick())
        {
            sceneManager.TickUpdate();
            tick.ConsumeTick();
        }
        time.UpdateTime();
        if (input.IsKeyDown(GLFW_KEY_ESCAPE))
            window->SetShouldClose();
        if (input.IsKeyDown(GLFW_KEY_F11))
            window->ToggleFullscreen();
        sceneManager.Update();
        UISystem::GetInstance().OnUpdate();
        UISystem::GetInstance().RenderAll();
        input.UpdateKey();
        input.UpdateScroll();
        glfwSwapBuffers(window->GetNativeWindow());
        glfwPollEvents();
    }
}
void Application::Quit()
{
    window->SetShouldClose();
}