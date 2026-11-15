#include "player.h"

#include "render/camera.h"
#include "world/world.h"
#include "core/input.h"

Player::Player(glm::vec3 position, Camera* camera, World* world) : position(position), previousPosition(position), movement(world, 0.08), rayCaster(world, 5, 0.01f), hotbar(), camera(camera), world(world)
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
    return {x, y, z};
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
    return {World::WorldToChunk(pos.x), 0, World::WorldToChunk(pos.z)};
}
HotBar& Player::GetHotBar()
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
    auto offset = glm::ivec3(0);
    if (!rayCaster.Ray(position + glm::vec3(0.0f, 1.62f, 0.0f), camera->front, lookAtPosition, offset))
    {
        lookAtPosition = glm::i16vec3(0, -1, 0);
    }
    if (time.GetCurrentTime() - lastRayCastTime > rayCastDuration && input.IsMouseButtonPressed(MOUSE_BUTTON_LEFT))
    {
        if (lookAtPosition.y != -1)
        {
            world->SetBlockAndRefresh(lookAtPosition.x, lookAtPosition.y, lookAtPosition.z, 0);
            lastRayCastTime = time.GetCurrentTime();
        }
    }
    if (time.GetCurrentTime() - lastRayCastTime > rayCastDuration && input.IsMouseButtonPressed(MOUSE_BUTTON_RIGHT))
    {
        if (lookAtPosition.y != -1)
        {
            const ItemStack& stack = hotbar.GetSlotItem();

            glm::i16vec3 blockPos = glm::i16vec3(lookAtPosition.x + offset.x, lookAtPosition.y + offset.y, lookAtPosition.z + offset.z);
            const bool targetIsAir = world->GetBlockStateId(blockPos.x, blockPos.y, blockPos.z) == 0;

            const bool intersectsPlayer = GetPlayerAABB(position).Intersects(GetBlockAABB(blockPos.x, blockPos.y, blockPos.z)) ||
            GetPlayerAABB(position + glm::vec3(0, 1, 0)).Intersects(GetBlockAABB(blockPos.x, blockPos.y, blockPos.z));

            if (!stack.IsEmpty() && targetIsAir && !intersectsPlayer)
            {
                const auto& item =
                    ItemRegister::GetInstance().GetItem(stack.itemId);

                if (item.blockId)
                {
                    world->SetBlockAndRefresh(blockPos.x, blockPos.y, blockPos.z, *item.blockId);
                    lastRayCastTime = time.GetCurrentTime();
                }
            }
        }
    }
}