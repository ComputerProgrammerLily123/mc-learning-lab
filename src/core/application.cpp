#include "application.h"

#include <GLFW/glfw3.h>
#include <glad/glad.h>

#include <iostream>

#include "gui/button.h"
#include "gui/canvas.h"
#include "gui/quad.h"
#include "gui/uiSystem.h"
#include "item/itemRegister.h"
#include "player/player.h"
#include "render/camera.h"
#include "render/outlineRenderer.h"
#include "render/skyboxRenderer.h"
#include "render/worldRenderer.h"
#include "util/jsonReader.h"
#include "resource/modelParser.h"
#include "window.h"
#include "world/world.h"

Application::Application()
{
    std::cout<<"[Application] ========OpenGL init start========\n";
    InitGLFW();
    InitGLAD();
    InitGLFWCallback();
    std::cout<<"[Application] ========OpenGL init finish========\n";
    std::cout<<"[Application] ========Registry init start========\n";
    InitRegistry();
    std::cout<<"[Application] ========Reggistry init finish========\n";
    std::cout<<"[Application] ========Resource init start========\n";
    InitResource();
    std::cout<<"[Application] ========Resource init finish========\n";
    std::cout<<"[Application] ========Scene init start========\n";
    InitScene();
    std::cout<<"[Application] ========Scene init finish========\n";
}
void Application::InitGLFW()
{
    window = std::make_unique<Window>(INITIAL_SCREEN_WIDTH, INITIAL_SCREEN_HEIGHT, "Minecraft:Papyrus Edition");
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
void Application::InitRegistry()
{
    BlockRegister::GetInstance().RegisterAll();
    RegisterItems();
}
void Application::InitResource()
{
    for (int i = 0;i < BlockRegister::GetInstance().GetBlockCount();i++) 
    {
        auto model = resourceManager.LoadModel(ResourceLocation("block/" + BlockRegister::GetInstance().GetBlock(i).GetName()));
        if(!model->textures.has_value()) continue;
        for (const auto& [id, content] : model->textures.value())
        {
            resourceManager.RegisterTextureRegionInAtlas(ResourceLocation(content));
        }
    }
    resourceManager.UploadAtlas();
    resourceManager.RegisterTextureRegion(ResourceLocation("font/ascii"), ResourceLocation("font/ascii"), glm::i16vec4(0, 128, 8, 120));
    resourceManager.RegisterTextureRegion(ResourceLocation("gui/title_left"), ResourceLocation("gui/title/minecraft"), glm::i16vec4(0, 256, 155, 212));
    resourceManager.RegisterTextureRegion(ResourceLocation("gui/title_right"), ResourceLocation("gui/title/minecraft"), glm::i16vec4(0, 211, 119, 167));
    resourceManager.RegisterTextureRegion(ResourceLocation("gui/button_normal"), ResourceLocation("gui/widgets"), glm::i16vec4(0, 190, 200, 170));
    resourceManager.RegisterTextureRegion(ResourceLocation("gui/button_highlighted"), ResourceLocation("gui/widgets"), glm::i16vec4(0, 170, 200, 150));
    resourceManager.RegisterTextureRegion(ResourceLocation("gui/lang_button_normal"), ResourceLocation("gui/widgets"), glm::i16vec4(0, 150, 20, 130));
    resourceManager.RegisterTextureRegion(ResourceLocation("gui/lang_button_highlighted"), ResourceLocation("gui/widgets"), glm::i16vec4(0, 130, 20, 110));
    resourceManager.RegisterTextureRegion(ResourceLocation("gui/crosshair"), ResourceLocation("gui/icons"), glm::i16vec4(0, 256, 15, 241));
    resourceManager.RegisterTextureRegion(ResourceLocation("gui/hotbar"), ResourceLocation("gui/widgets"), glm::i16vec4(0, 256, 182, 234));
    resourceManager.RegisterTextureRegion(ResourceLocation("gui/hotbar_frame"), ResourceLocation("gui/widgets"), glm::i16vec4(0, 234, 24, 210));
    resourceManager.RegisterCubemap(ResourceLocation("gui/main_menu_skybox"), {ResourceLocation("gui/title/background/panorama_1"), ResourceLocation("gui/title/background/panorama_3"), ResourceLocation("gui/title/background/panorama_4"), ResourceLocation("gui/title/background/panorama_5"), ResourceLocation("gui/title/background/panorama_0"), ResourceLocation("gui/title/background/panorama_2")});
}
void Application::InitScene()
{
    sceneManager.CreateScene("Welcome");
    sceneManager.CreateScene("Main");

    // Welcome Scene
    auto welcomeScene = sceneManager.GetScene("Welcome");
    welcomeScene->onLoad = [this]
    {
        auto canvas = std::make_unique<Canvas>(INITIAL_SCREEN_WIDTH,INITIAL_SCREEN_HEIGHT);
        auto camera = std::make_unique<Camera>(window->GetWidth(), window->GetHeight());
        auto skyboxRenderer = std::make_unique<SkyboxRenderer>();

        camera->SetZoom(90.0f);

        auto titleA = std::make_unique<Quad>(PivotType::Center, FlexType::None, -225, -215, 605.46875f, 171.875f, "gui/title_left");
        auto titleB = std::make_unique<Quad>(PivotType::Center, FlexType::None, 305, -215, 464.84375f, 171.875f, "gui/title_right");
        auto buttonA = std::make_unique<Button>(PivotType::Center, FlexType::None, 0, 0, 728.0f, 88.0f, "gui/button_normal", "gui/button_highlighted", "Singleplayer");
        auto buttonB = std::make_unique<Button>(PivotType::Center, FlexType::None, 0, 100, 728.0f, 88.0f, "gui/button_normal", "gui/button_highlighted", "Multiplayer");
        auto buttonC = std::make_unique<Button>(PivotType::Center, FlexType::None, 0, 200, 728.0f, 88.0f, "gui/button_normal", "gui/button_highlighted", "Quit");
        auto buttonD = std::make_unique<Button>(PivotType::Center, FlexType::None, -450, 300, 88.0f, 88.0f, "gui/lang_button_normal", "gui/lang_button_highlighted");

        buttonA->AddListener([this]
                             { sceneManager.RequestLoadScene("Main"); });
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
        auto canvas = std::make_unique<Canvas>(INITIAL_SCREEN_WIDTH,INITIAL_SCREEN_HEIGHT);
        auto camera = std::make_unique<Camera>(window->GetWidth(), window->GetHeight());
        auto world = std::make_unique<World>();
        auto outlineRenderer = std::make_unique<OutlineRenderer>();
        auto worldRenderer = std::make_unique<WorldRenderer>(RENDER_RADIAN);
        auto player = std::make_unique<Player>(glm::vec3(10.0f, 100.0f, 10.0f), camera.get(), world.get());

        auto crosshair = std::make_unique<Quad>(PivotType::Center, FlexType::None, 0, 0, 32.0f, 32.0f, "gui/crosshair");
        auto hotbar = std::make_unique<Quad>(PivotType::BottomMiddle, FlexType::None, 0, 350, 728.0f, 88.0f, "gui/hotbar");
        auto hotbarFrame = std::make_unique<Quad>(PivotType::Center, FlexType::None, player->GetHotBar().GetHotBarSlot() * 80, 350, 88.0f, 88.0f, "gui/hotbar_frame");
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
void Application::RegisterItems()
{
    
    auto &itemRegister = ItemRegister::GetInstance();
    itemRegister.RegisterItem("grass");
    itemRegister.RegisterItem("dirt");
    itemRegister.RegisterItem("stone");
    
}
void Application::OnWindowResize(unsigned newWidth, unsigned newHeight)
{
    window->ResizeViewport(newWidth, newHeight);
    UISystem::GetInstance().ResizeScreen(newWidth, newHeight);
    sceneManager.GetObject<Camera>("Camera")->ResizeScreen(newWidth, newHeight);
}
void Application::Run()
{
    std::cout<<"[Application] ========Main loop start========\n";
    while (!window->ShouldClose())
    {
        //std::cout << "FPS:" << 1.0f / time.GetDeltaTime() << '\n';
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
    std::cout<<"[Application] ========Main loop end========\n";
}
void Application::Quit()
{
    window->SetShouldClose();
}