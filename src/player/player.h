#pragma once

#include "glm/glm.hpp"
#include "movement.h"
#include "rayCaster.h"
#include "hotBar.h"
#include "input.h"
#include "timeSystem.h"
class Camera;
class World;
class Player{
public:
    Player(glm::vec3 position,Camera* camera,World* world);
    ~Player();
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
    float rayCastDuration = 0.1f;

    void ProcessRayCast();
};