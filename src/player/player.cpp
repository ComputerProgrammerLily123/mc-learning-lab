#include "player.h"

#include "world.h"
#include "GLFW/glfw3.h"
#include "camera.h"
Player::Player(glm::vec3 position, Camera *camera, World *world) : position(position), movement(world, 0.08), rayCaster(world, 5, 0.01f), lastRayCastTime(0.0f), rayCastDuration(0.1f)
{
    this->camera = camera;
    this->world = world;
}
Player::~Player()
{
}
glm::vec3 Player::GetPosition() const
{
    return position;
}
glm::i16vec3 Player::GetBlockPosition() const
{
    int x = (int)glm::round(position.x);
    int y = (int)glm::floor(position.y) + 1;
    int z = (int)glm::round(position.z);
    return glm::i16vec3(x, y, z);
}
glm::vec3 Player::GetPreviousTickPosition() const
{
    return previousPosition;
}

void Player::UpdateTick()
{
    previousPosition = position;
    movement.Move(position, camera->front, camera->right);
    ProcessRayCast();
}

void Player::ProcessRayCast()
{
    glm::i16vec3 target = glm::i16vec3(0);
    if (time.GetCurrentTime() - lastRayCastTime > rayCastDuration && input.IsMouseButtonDown(GLFW_MOUSE_BUTTON_1))
    {
        if (rayCaster.Ray(position + glm::vec3(0.0f, 1.62f, 0.0f), camera->front, target))
        {
            world->SetBlockAndRefresh(target.x, target.y, target.z, 0);
            lastRayCastTime = time.GetCurrentTime();
        }
    }
    if (time.GetCurrentTime() - lastRayCastTime > rayCastDuration && input.IsMouseButtonDown(GLFW_MOUSE_BUTTON_2))
    {
        glm::ivec3 offset = glm::ivec3(0);
        if (rayCaster.Ray(position + glm::vec3(0.0f, 1.62f, 0.0f), camera->front, target, offset))
        {
            glm::i16vec3 blockPos = glm::i16vec3(target.x + offset.x, target.y + offset.y, target.z + offset.z);
            if (blockPos != GetBlockPosition() && blockPos != GetBlockPosition() - glm::i16vec3(0, -1, 0))
            {
                world->SetBlockAndRefresh(target.x + offset.x, target.y + offset.y, target.z + offset.z, 3);
                lastRayCastTime = time.GetCurrentTime();
            }
        }
    }
}