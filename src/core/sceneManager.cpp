#include "sceneManager.h"

void SceneManager::CreateScene(const std::string& id)
{
    auto result = scenes.find(id);
    if (result != scenes.end())
    {
        std::cout << "[SceneManager] Scene(" << id << ") already exists.\n";
        return;
    }
    scenes.emplace(id, std::make_unique<Scene>());
    std::cout << "[SceneManager] Create Scene(" << id << ").\n";
}
Scene* SceneManager::GetCurrentScene() const
{
    return currentScene;
}
Scene* SceneManager::GetScene(const std::string& id) const
{
    auto result = scenes.find(id);
    if (result != scenes.end())
    {
        return result->second.get();
    }
    return nullptr;
}
void SceneManager::LoadScene(const std::string& id)
{
    auto result = scenes.find(id);
    if (result == scenes.end())
    {
        std::cout << "[SceneManager] Scene(" << id << ") not found!\n";
        return;
    }
    if (currentScene != nullptr)
    {
        currentScene->onUnLoad();
        std::cout << "[SceneManager] Scene unloaded.\n";
    }
    result = scenes.find(id);
    if (result != scenes.end())
    {
        currentScene = result->second.get();
        currentScene->onLoad();
    }
    else
    {
        std::cout << "[SceneManager] Scene load failed.\n";
    }
}
void SceneManager::Update()
{
    if (!pendingScene.empty())
    {
        LoadScene(pendingScene);
        pendingScene.clear();
        return;
    }
    if (currentScene && currentScene->onUpdate)
        currentScene->onUpdate();
}
void SceneManager::TickUpdate()
{
    if (currentScene->onTickUpdate)
        currentScene->onTickUpdate();
}
void SceneManager::ClearObjects()
{
    currentScene->sceneObject.clear();
}
void SceneManager::RequestLoadScene(const std::string& id)
{
    pendingScene = id;
}