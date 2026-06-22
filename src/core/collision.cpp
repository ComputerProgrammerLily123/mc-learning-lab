#include "collision.h"

#include "world/world.h"
#include <iostream>
CollisionSystem::CollisionSystem(World *world) : world(world) {}
glm::vec3 CollisionSystem::Move(const glm::vec3 &position, const glm::vec3 &movement)
{
    glm::vec3 result = position;
    unsigned step = glm::max(static_cast<int>(glm::max(glm::max(abs(movement.x), abs(movement.y)), abs(movement.z))), 1) * 4;
    for (unsigned i = 0; i < step; i++)
    {
        AABB playerBox = GetPlayerAABB(result + glm::vec3(movement.x / step, movement.y / step, movement.z / step));
        auto block0 = glm::i16vec3(static_cast<int>(glm::round(result.x)), static_cast<int>(glm::floor(result.y + (movement.y > 0 ? 2.5f : -0.5f))), static_cast<int>(glm::round(result.z)));
        auto block1 = glm::i16vec3(static_cast<int>(glm::round(result.x + movement.x / abs(movement.x))), static_cast<int>(glm::floor(result.y) + 1), static_cast<int>(glm::round(result.z)));
        auto block2 = glm::i16vec3(static_cast<int>(glm::round(result.x)), static_cast<int>(glm::floor(result.y) + 1), static_cast<int>(glm::round(result.z + movement.z / abs(movement.z))));
        if (world->GetBlock(block0.x, block0.y, block0.z) && playerBox.Intersects(GetBlockAABB(block0.x, block0.y, block0.z)))
        {
            if (movement.y > 0)
            {
                result.y = block0.y - 2.5f;
            }
            else if (movement.y < 0)
            {
                result.y = block0.y + 0.5f;
            }
        }
        else
        {
            result.y += movement.y / step;
        }
        if (world->GetBlock(block1.x, block1.y, block1.z) && playerBox.Intersects(GetBlockAABB(block1.x, block1.y, block1.z)) || world->GetBlock(block1.x, block1.y + 1, block1.z) && playerBox.Intersects(GetBlockAABB(block1.x, block1.y + 1, block1.z)))
        {
            if (movement.x > 0)
            {
                result.x = block1.x - 0.8f;
            }
            else if (movement.x < 0)
            {
                result.x = block1.x + 0.8f;
            }
        }
        else
        {
            result.x += movement.x / step;
        }
        if (world->GetBlock(block2.x, block2.y, block2.z) && playerBox.Intersects(GetBlockAABB(block2.x, block2.y, block2.z)) || world->GetBlock(block2.x, block2.y + 1, block2.z) && playerBox.Intersects(GetBlockAABB(block2.x, block2.y + 1, block2.z)))
        {
            if (movement.z > 0)
            {
                result.z = block2.z - 0.8f;
            }
            else if (movement.z < 0)
            {
                result.z = block2.z + 0.8f;
            }
        }
        else
        {
            result.z += movement.z / step;
        }
    }
    return result;
}

bool CollisionSystem::IsOnGround(const glm::vec3 &position)
{
    AABB playerBox = GetPlayerAABB(position - glm::vec3(0, 0.000001f, 0));
    int x = static_cast<int>(glm::round(position.x));
    int y = static_cast<int>(glm::floor(position.y) + 1);
    int z = static_cast<int>(glm::round(position.z));
    return world->GetBlock(x, y - 1, z) && playerBox.Intersects(GetBlockAABB(x, y - 1, z));
}