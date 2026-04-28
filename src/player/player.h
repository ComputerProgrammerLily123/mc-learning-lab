#ifndef PLAYER_H
#define PLAYER_H

#include "glm/glm.hpp"
#include "movement.h"
#include "rayCaster.h"
#include "input.h"
#include "time.h"
class Camera;
class World;
class Player{
public:
    Player(glm::vec3 position,Camera* camera,World* world);
    ~Player();
    glm::vec3 GetPosition() const;
    glm::i16vec3 GetBlockPosition() const;
    glm::vec3 GetPreviousTickPosition() const;
    void UpdateTick();
private:
    glm::vec3 position;
    glm::vec3 previousPosition;
    Movement movement;
    RayCaster rayCaster;
    Camera* camera;
    World* world;

    TimeSystem& time = TimeSystem::GetInstance();
    Input& input = Input::GetInstance();

    float lastRayCastTime;
    float rayCastDuration;

    void ProcessRayCast();
};
#endif