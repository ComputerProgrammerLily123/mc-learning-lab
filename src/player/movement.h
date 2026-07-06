#pragma once

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "core/collision.h"
#include "core/input.h"
#include "core/timeSystem.h"

class World;
class Movement
{
public:
    Movement(const Movement&) = default;
    ~Movement() = default;
    Movement(Movement&&) = delete;
    Movement& operator=(const Movement&) = delete;
    Movement& operator=(Movement&&) = delete;
    Movement(World* world, float gravityFac);
    void Move(glm::vec3& position, glm::vec3 front, glm::vec3 right);

private:
    static constexpr float walkAcceleration = 0.1f;
    static constexpr float walkDragFac = 0.91f;
    static constexpr float blockFraction = 0.6f;
    static constexpr float maxWalkSpeed = 0.215f;
    static constexpr float jumpDuration = 0.5f;
    float gravityFac;
    bool isGrounded = false;
    float currentJumpTime = 0.0f;
    glm::vec3 currentVelocity = glm::vec3(0.0f);
    Input& input = Input::GetInstance();
    TimeSystem& time = TimeSystem::GetInstance();

    CollisionSystem collision;
    void ProcessGravity();
    void ProcessMove(glm::vec3 front, glm::vec3 right);
    void ProcessJump();
};