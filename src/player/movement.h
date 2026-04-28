#ifndef MOVEMENT_H
#define MOVEMENT_H

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include "time.h"
#include "input.h"
#include "collision.h"
class World;
class Movement{
public:
    Movement(World* world,float gravityFac);
    ~Movement();
    void Move(glm::vec3& position,glm::vec3 front,glm::vec3 right);
    private:
    float speed = 0.215;
    float gravityFac;
    bool isGrounded;
    glm::vec3 currentVelocity = glm::vec3(0.0f);
    Input& input = Input::GetInstance();
    TimeSystem& time = TimeSystem::GetInstance();

    CollisionSystem collision;
    void ProcessGravity();
    void ProcessMove(glm::vec3 front,glm::vec3 right);
    void ProcessJump();
};
#endif