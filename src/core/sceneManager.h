#pragma once
#include <functional>
#include <iostream>
#include <memory>
#include <string>
#include <unordered_map>

class SceneObject
{
public:
    SceneObject() = default;

    SceneObject(const SceneObject&) = delete;
    SceneObject(SceneObject&&) = delete;

    SceneObject& operator=(const SceneObject&) = delete;
    SceneObject& operator=(SceneObject&&) = delete;
    virtual ~SceneObject() = default;
};
class Scene
{
    friend class SceneManager;

public:
    std::function<void()> onLoad;
    std::function<void()> onUpdate;
    std::function<void()> onTickUpdate;
    std::function<void()> onUnLoad;

private:
    std::unordered_map<std::string, std::unique_ptr<SceneObject>> sceneObject;
};
class SceneManager
{
public:
    SceneManager(SceneManager&&) = delete;
    SceneManager& operator=(SceneManager&&) = delete;
    SceneManager(const SceneManager&) = delete;
    SceneManager& operator=(const SceneManager&) = delete;
    static SceneManager& GetInstance()
    {
        static SceneManager instance;
        return instance;
    }
    void CreateScene(const std::string& id);
    Scene* GetCurrentScene() const;
    Scene* GetScene(const std::string& id) const;
    void LoadScene(const std::string& id);
    template <typename T>
    T* GetObject(const std::string& id) const
    {
        if (currentScene == nullptr)
        {
            std::cout << "No active scene!\n";
            return nullptr;
        }
        auto result = currentScene->sceneObject.find(id);
        if (result != currentScene->sceneObject.end())
        {
            return static_cast<T*>(result->second.get());
        }
        return nullptr;
    }
    template <typename T>
    void AddObject(const std::string& id, std::unique_ptr<T> object)
    {
        if (currentScene == nullptr)
        {
            std::cout << "No active scene!\n";
            return;
        }
        currentScene->sceneObject.emplace(id, std::move(object));
    }
    void ClearObjects();
    void Update();
    void TickUpdate();

private:
    SceneManager() = default;
    ~SceneManager() = default;
    Scene* currentScene = nullptr;
    std::unordered_map<std::string, std::unique_ptr<Scene>> scenes;
};