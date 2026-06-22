#include "player.h"

#include "world/world.h"
#include "GLFW/glfw3.h"
#include "render/camera.h"
Player::Player(glm::vec3 position, Camera *camera, World *world) : position(position), movement(world, 0.08), rayCaster(world, 5, 0.01f), hotbar()
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
    int x = static_cast<int>(glm::round(position.x));
    int y = static_cast<int>(glm::floor(position.y) + 1);
    int z = static_cast<int>(glm::round(position.z));
    return glm::i16vec3(x, y, z);
}
glm::vec3 Player::GetPreviousTickPosition() const
{
    return previousPosition;
}
glm::i16vec3 Player::GetLookAtPosition() const
{
    return lookAtPosition;
}
glm::i16vec3 Player::GetChunkPosition() const
{
    auto pos = GetBlockPosition();
    return glm::i16vec3(pos.x / 16, 0, pos.z / 16);
}
HotBar &Player::GetHotBar()
{
    return hotbar;
}
void Player::UpdateTick()
{
    previousPosition = position;
    movement.Move(position, camera->front, camera->right);
    ProcessRayCast();
}

void Player::ProcessRayCast()
{
    glm::ivec3 offset = glm::ivec3(0);
    if (!rayCaster.Ray(position + glm::vec3(0.0f, 1.62f, 0.0f), camera->front, lookAtPosition, offset))
    {
        lookAtPosition = glm::i16vec3(0, -1, 0);
    }
    if (time.GetCurrentTime() - lastRayCastTime > rayCastDuration && input.IsMouseButtonDown(GLFW_MOUSE_BUTTON_1))
    {
        if (lookAtPosition.y != -1)
        {
            world->SetBlockAndRefresh(lookAtPosition.x, lookAtPosition.y, lookAtPosition.z, 0);
            lastRayCastTime = time.GetCurrentTime();
        }
    }
    if (time.GetCurrentTime() - lastRayCastTime > rayCastDuration && input.IsMouseButtonDown(GLFW_MOUSE_BUTTON_2))
    {
        if (lookAtPosition.y != -1)
        {
            glm::i16vec3 blockPos = glm::i16vec3(lookAtPosition.x + offset.x, lookAtPosition.y + offset.y, lookAtPosition.z + offset.z);
            if (!world->GetBlock(blockPos.x, blockPos.y, blockPos.z) && !GetPlayerAABB(position).Intersects(GetBlockAABB(blockPos.x, blockPos.y, blockPos.z)) && !GetPlayerAABB(position + glm::vec3(0, 1, 0)).Intersects(GetBlockAABB(blockPos.x, blockPos.y, blockPos.z)))
            {
                world->SetBlockAndRefresh(lookAtPosition.x + offset.x, lookAtPosition.y + offset.y, lookAtPosition.z + offset.z, hotbar.GetSlotItem());
                lastRayCastTime = time.GetCurrentTime();
            }
        }
    }
}