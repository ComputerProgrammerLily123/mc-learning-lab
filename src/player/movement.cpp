#include "movement.h"

#include "world.h"
#include <GLFW/glfw3.h>
#include "collision.h"
Movement::Movement(World *world, float gravityFac) : collision(world)
{
    this->gravityFac = gravityFac;
}
Movement::~Movement()
{
}
void Movement::Move(glm::vec3 &position, glm::vec3 front, glm::vec3 right)
{
    isGrounded = collision.IsOnGround(position);
    ProcessMove(front, right);
    ProcessGravity();
    ProcessJump();
    position = collision.Move(position, glm::vec3(currentVelocity.x, currentVelocity.y, currentVelocity.z));
}
void Movement::ProcessGravity()
{
    if (!isGrounded)
    {
        currentVelocity.y = glm::max(currentVelocity.y - gravityFac, -3.92f);
    }
    else
    {
        currentVelocity.y = 0;
    }
}
void Movement::ProcessMove(glm::vec3 front, glm::vec3 right)
{
    glm::vec3 acceleration = glm::vec3(0.0f);
    if (input.IsKeyPressed(GLFW_KEY_W))
        acceleration += front;
    if (input.IsKeyPressed(GLFW_KEY_S))
        acceleration -= front;
    if (input.IsKeyPressed(GLFW_KEY_A))
        acceleration -= right;
    if (input.IsKeyPressed(GLFW_KEY_D))
        acceleration += right;
    acceleration.y = 0;
    if (acceleration.x != 0.0f || acceleration.z != 0.0f)
    {
        acceleration = glm::normalize(acceleration) * walkAcceleration;
    }
    currentVelocity.x *= walkDragFac * blockFraction;
    currentVelocity.x += acceleration.x;
    currentVelocity.z *= walkDragFac * blockFraction;
    currentVelocity.z += acceleration.z;
    float speed = glm::length(glm::vec3(currentVelocity.x, 0, currentVelocity.z));
    if (speed > maxWalkSpeed)
    {
        currentVelocity.x = currentVelocity.x / speed * maxWalkSpeed;
        currentVelocity.z = currentVelocity.z / speed * maxWalkSpeed;
    }
}
void Movement::ProcessJump()
{
    currentJumpTime += 0.05f;
    if (currentJumpTime > jumpDuration && isGrounded && input.IsKeyPressed(GLFW_KEY_SPACE))
    {
        currentVelocity.y = 0.42;
        currentJumpTime = 0.0f;
    }
}