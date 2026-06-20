#pragma once
enum SceneEnum{
    GameMenu,
    MainScene
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
private:
    SceneManager() = default;
    ~SceneManager() = default;
};