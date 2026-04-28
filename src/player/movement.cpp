#include "movement.h"

#include "world.h"
#include <GLFW/glfw3.h>
#include "collision.h"
Movement::Movement(World *world, float gravityFac) : isGrounded(false), collision(world)
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
    if (currentVelocity.y > -3.92f)
        currentVelocity.y -= gravityFac;
    if (isGrounded)
        currentVelocity.y = 0;
}
void Movement::ProcessMove(glm::vec3 front, glm::vec3 right)
{
    glm::vec3 velocity = glm::vec3(0.0f);
    if (input.IsKeyPressed(GLFW_KEY_W))
        velocity += front;
    if (input.IsKeyPressed(GLFW_KEY_S))
        velocity -= front;
    if (input.IsKeyPressed(GLFW_KEY_A))
        velocity -= right;
    if (input.IsKeyPressed(GLFW_KEY_D))
        velocity += right;
    velocity.y = 0;
    if (glm::length(velocity))
    {
        velocity = glm::normalize(velocity) * speed;
    }
    currentVelocity.x = velocity.x;
    currentVelocity.z = velocity.z;
}
void Movement::ProcessJump()
{
    if (isGrounded && input.IsKeyPressed(GLFW_KEY_SPACE))
        currentVelocity.y = 0.42;
}