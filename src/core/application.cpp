#include "application.h"

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>

#include "window.h"
#include "render/outlineRenderer.h"
#include "render/worldRenderer.h"
#include "render/skyboxRenderer.h"
#include "render/uiRenderer.h"
#include "render/camera.h"
#include "world/world.h"
#include "player/player.h"
#include "item/itemRegister.h"
#include "util/jsonReader.h"
#include "gui/canvas.h"
#include "gui/quad.h"
#include "gui/button.h"
Application::Application()
{
    InitGLFW();
    InitGLAD();
    InitGLFWCallback();
    InitResource();
    InitScene();
}
Application::~Application()
{
    delete window;
}
void Application::InitGLFW()
{
    window = new Window(screenWidth, screenHeight, "Minecraft:Papyrus Edition");
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
void Application::InitScene()
{
    auto welcomeScene = sceneManager.CreateScene("Welcome");
    auto mainScene = sceneManager.CreateScene("Main");
    // Welcome Scene
    {
        // Scene Object definition
        welcomeScene->sceneObject.emplace("Canvas", nullptr);
        welcomeScene->sceneObject.emplace("Camera", nullptr);
        welcomeScene->sceneObject.emplace("SkyboxRenderer", nullptr);
        //========================
        welcomeScene->onLoad = [this, welcomeScene]
        {
            auto canvas = new Canvas(PivotType::Center, FlexType::Full, INITIAL_SCREEN_WIDTH / 2, INITIAL_SCREEN_HEIGHT / 2);
            auto camera = new Camera(screenWidth, screenHeight);
            auto skyboxRenderer = new SkyboxRenderer();

            camera->SetZoom(90.0f);

            auto titleA = new Quad(PivotType::Center, FlexType::None, -225, -215, 605.46875f, 171.875f, glm::vec4(0.0f, 1.0f, 0.60546875f, 0.828125f), "title/minecraft.png");
            auto titleB = new Quad(PivotType::Center, FlexType::None, 305, -215, 464.84375f, 171.875f, glm::vec4(0.0f, 0.82421875f, 0.46484375f, 0.65234375f), "title/minecraft.png");
            auto buttonA = new Button(PivotType::Center, FlexType::None, 0, 0, 728.0f, 88.0f, glm::vec4(0.0f, 0.7421875f, 0.78125f, 0.6640625f), glm::vec4(0.0f, 0.6640625f, 0.78125f, 0.5859375f), "widgets.png");
            auto buttonB = new Button(PivotType::Center, FlexType::None, 0, 100, 728.0f, 88.0f, glm::vec4(0.0f, 0.7421875f, 0.78125f, 0.6640625f), glm::vec4(0.0f, 0.6640625f, 0.78125f, 0.5859375f), "widgets.png");
            auto buttonC = new Button(PivotType::Center, FlexType::None, 0, 200, 728.0f, 88.0f, glm::vec4(0.0f, 0.7421875f, 0.78125f, 0.6640625f), glm::vec4(0.0f, 0.6640625f, 0.78125f, 0.5859375f), "widgets.png");
            auto buttonD = new Button(PivotType::Center, FlexType::None, -450, 300, 88.0f, 88.0f, glm::vec4(0.0f, 0.5859375f, 0.078125f, 0.5078125f), glm::vec4(0.0f, 0.5078125f, 0.078125f, 0.4296875f), "widgets.png");
            buttonA->AddListener([this]
                                 { sceneManager.LoadScene("Main"); });
            canvas->ClearElement();
            canvas->AddElement(titleA);
            canvas->AddElement(titleB);
            canvas->AddElement(buttonA);
            canvas->AddElement(buttonB);
            canvas->AddElement(buttonC);
            canvas->AddElement(buttonD);
            UISystem::GetInstance().SetMainCanvas(canvas);
            welcomeScene->sceneObject["Canvas"] = std::shared_ptr<Canvas>(canvas);
            welcomeScene->sceneObject["Camera"] = std::shared_ptr<Camera>(camera);
            welcomeScene->sceneObject["SkyboxRenderer"] = std::shared_ptr<SkyboxRenderer>(skyboxRenderer);
            welcomeScene->availableFlag = true;
        };
        welcomeScene->onUpdate = [this, welcomeScene]
        {
            if (welcomeScene->availableFlag)
            {
                auto camera = static_cast<Camera *>(welcomeScene->sceneObject["Camera"].get());
                auto skyboxRenderer = static_cast<SkyboxRenderer *>(welcomeScene->sceneObject["SkyboxRenderer"].get());
                camera->SetYaw(time.GetDeltaTime() * 10);
                skyboxRenderer->Draw(camera);
            }
        };
        welcomeScene->onUnLoad = [welcomeScene]
        {
            welcomeScene->availableFlag = false;
            welcomeScene->sceneObject.clear();
        };
    }

    // Main Scene
    {
        // Scene Object definition
        mainScene->sceneObject.emplace("Canvas", nullptr);
        mainScene->sceneObject.emplace("Camera", nullptr);
        mainScene->sceneObject.emplace("World", nullptr);
        mainScene->sceneObject.emplace("OutlineRenderer", nullptr);
        mainScene->sceneObject.emplace("WorldRenderer", nullptr);
        mainScene->sceneObject.emplace("Player", nullptr);
        //========================
        mainScene->onLoad = [this, mainScene]
        {
            auto canvas = new Canvas(PivotType::Center, FlexType::Full, INITIAL_SCREEN_WIDTH / 2, INITIAL_SCREEN_HEIGHT / 2);
            auto camera = new Camera(screenWidth, screenHeight);
            auto world = new World();
            auto outlineRenderer = new OutlineRenderer();
            auto worldRenderer = new WorldRenderer(RENDER_RADIAN);
            auto player = new Player(glm::vec3(10.0f, 100.0f, 10.0f), camera, world);

            auto crosshair = new Quad(PivotType::Center, FlexType::None, 0, 0, 32.0f, 32.0f, glm::vec4(0.0f, 1.0f, 0.05859375f, 0.94140625f), "icons.png");
            auto hotbar = new Quad(PivotType::BottomMiddle, FlexType::None, 0, 350, 728.0f, 88.0f, glm::vec4(0.0f, 1.0f, 0.7109375f, 0.9140625f), "widgets.png");
            auto hotbarFrame = new Quad(PivotType::Center, FlexType::None, player->GetHotBar().GetHotBarSlot() * 80, 350, 88.0f, 88.0f, glm::vec4(0.0f, 0.9140625f, 0.09375f, 0.8203125f), "widgets.png");
            canvas->AddElement(crosshair);
            canvas->AddElement(hotbar);
            canvas->AddElement(hotbarFrame);

            mainScene->sceneObject["Canvas"] = std::shared_ptr<Canvas>(canvas);
            mainScene->sceneObject["Camera"] = std::shared_ptr<Camera>(camera);
            mainScene->sceneObject["World"] = std::shared_ptr<World>(world);
            mainScene->sceneObject["OutlineRenderer"] = std::shared_ptr<OutlineRenderer>(outlineRenderer);
            mainScene->sceneObject["WorldRenderer"] = std::shared_ptr<WorldRenderer>(worldRenderer);
            mainScene->sceneObject["Player"] = std::shared_ptr<Player>(player);
            mainScene->availableFlag = true;
        };
        mainScene->onUpdate = [this, mainScene]
        {
            if (mainScene->availableFlag)
            {
                auto canvas = static_cast<Canvas *>(mainScene->sceneObject["Canvas"].get());
                auto camera = static_cast<Camera *>(mainScene->sceneObject["Camera"].get());
                auto world = static_cast<World *>(mainScene->sceneObject["World"].get());
                auto outlineRenderer = static_cast<OutlineRenderer *>(mainScene->sceneObject["OutlineRenderer"].get());
                auto worldRenderer = static_cast<WorldRenderer *>(mainScene->sceneObject["WorldRenderer"].get());
                auto player = static_cast<Player *>(mainScene->sceneObject["Player"].get());
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
            }
        };
        mainScene->onUnLoad = [mainScene]
        {
            mainScene->availableFlag = false;
            mainScene->sceneObject.clear();
        };
    }
    sceneManager.LoadScene("Main");
}
void Application::InitResource()
{
    RegisterBlocks();
    resourceManager.LoadTexture();
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
        // std::cout << "FPS:" << 1.0f / time.GetDeltaTime() << std::endl;
        //  std::cout << player->GetBlockPosition().x << "," << player->GetBlockPosition().y << "," << player->GetBlockPosition().z << std::endl;
        //  std::cout << player->GetChunkPosition().x << "," << player->GetChunkPosition().y << "," << player->GetChunkPosition().z << std::endl;
        tick.UpdateTimer();
        if (tick.ShouldTick())
        {
            // player->UpdateTick();
            tick.ConsumeTick();
        }
        time.UpdateTime();
        if (input.IsKeyDown(GLFW_KEY_ESCAPE))
            window->SetShouldClose();
        if (input.IsKeyDown(GLFW_KEY_F11))
            window->ToggleFullscreen();
        //sceneManager.UpdateScene();
        //UISystem::GetInstance().OnUpdate();
        //UISystem::GetInstance().RenderAll();
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