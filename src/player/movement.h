#pragma once

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include "core/timeSystem.h"
#include "core/input.h"
#include "core/collision.h"
class World;
class Movement{
public:
    Movement(World* world,float gravityFac);
    ~Movement();
    void Move(glm::vec3& position,glm::vec3 front,glm::vec3 right);
    private:
    float walkAcceleration = 0.1f;
    float walkDragFac = 0.91f;
    float blockFraction = 0.6f;
    float maxWalkSpeed = 0.215f;
    float gravityFac;
    bool isGrounded = false;
    float jumpDuration = 0.5f;
    float currentJumpTime = 0.0f;
    glm::vec3 currentVelocity = glm::vec3(0.0f);
    Input& input = Input::GetInstance();
    TimeSystem& time = TimeSystem::GetInstance();

    CollisionSystem collision;
    void ProcessGravity();
    void ProcessMove(glm::vec3 front,glm::vec3 right);
    void ProcessJump();
};