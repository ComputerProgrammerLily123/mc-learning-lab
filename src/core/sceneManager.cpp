#include "sceneManager.h"
#include <iostream>
Scene *SceneManager::CreateScene(std::string id)
{
    auto scene = new Scene();
    scenes.emplace(id, scene);
    return scene;
}
void SceneManager::LoadScene(std::string id)
{
    if (currentScene != nullptr)
        currentScene->onUnLoad();
    auto result = scenes.find(id);
    if (result != scenes.end())
    {
        currentScene = result->second;
        currentScene->onLoad();
    }
    else
    {
        std::cout << "Scene load failed." << std::endl;
    }
}
void SceneManager::Update()
{
    if (currentScene->onUpdate)
        currentScene->onUpdate();
}
void SceneManager::TickUpdate()
{
    if (currentScene->onTickUpdate)
        currentScene->onTickUpdate();
}