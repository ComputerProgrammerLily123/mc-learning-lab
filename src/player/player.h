#pragma once

#include "movement.h"
#include "rayCaster.h"
#include "hotBar.h"
#include "core/input.h"
#include "core/timeSystem.h"
#include "core/sceneManager.h"

class Camera;
class World;
class Player : public SceneObject
{
public:
    Player(const Player&) = default;
    ~Player() = default;
    Player(Player&&) = delete;
    Player& operator=(const Player&) = delete;
    Player& operator=(Player&&) = delete;
    Player(glm::vec3 position, Camera* camera, World* world);
    glm::vec3 GetPosition() const;
    glm::i16vec3 GetBlockPosition() const;
    glm::vec3 GetPreviousTickPosition() const;
    glm::i16vec3 GetLookAtPosition() const;
    glm::i16vec3 GetChunkPosition() const;
    HotBar& GetHotBar();
    void UpdateTick();
private:
    glm::vec3 position;
    glm::vec3 previousPosition;
    glm::i16vec3 lookAtPosition;
    Movement movement;
    RayCaster rayCaster;
    HotBar hotbar;
    Camera* camera;
    World* world;

    TimeSystem& time = TimeSystem::GetInstance();
    Input& input = Input::GetInstance();

    float lastRayCastTime = 0.0f;
    static constexpr float rayCastDuration = 0.1f;

    void ProcessRayCast();
};