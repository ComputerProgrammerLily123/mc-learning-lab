#pragma once
#include <string>
#include <vector>
#include <unordered_map>
#include <functional>
#include <memory>
class Scene
{
public:
    std::function<void()> onLoad;
    std::function<void()> onUpdate;
    std::function<void()> onTickUpdate;
    std::function<void()> onUnLoad;
    std::unordered_map<std::string,std::shared_ptr<void>> sceneObject;
    bool availableFlag = false;
};
class SceneManager{
public:
    SceneManager(const SceneManager &) = delete;
    SceneManager &operator=(const SceneManager &) = delete;
    static SceneManager &GetInstance()
    {
        static SceneManager instance;
        return instance;
    }
    Scene* CreateScene(std::string id);
    void LoadScene(std::string id);
    void Update();
    void TickUpdate();
private:
    SceneManager() = default;
    ~SceneManager() = default;
    Scene* currentScene;
    std::unordered_map<std::string,Scene*> scenes;
};