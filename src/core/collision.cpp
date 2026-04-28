#include "collision.h"

#include "world.h"
CollisionSystem::CollisionSystem(World *world) : world(world) {}
void CollisionSystem::CheckCollisionAxis(glm::vec3 &pos, const glm::vec3 &move, int axis)
{
    glm::vec3 testPos = pos;
    testPos[axis] += move[axis];
    glm::i16vec3 testBlockPos = glm::round(testPos);
    AABB playerBox = GetPlayerAABB(testPos);
    auto block1 = GetBlockAABB(testBlockPos.x, testBlockPos.y, testBlockPos.z);
    auto block2 = GetBlockAABB(testBlockPos.x, testBlockPos.y + 1, testBlockPos.z);
    if (world->GetBlock(testBlockPos.x, testBlockPos.y, testBlockPos.z) && playerBox.Intersects(block1))
    {
        if (axis == 1)
        {
            if (move[axis] > 0)
            {
                pos[axis] = block1.min[axis];
            }
            else if (move[axis] < 0)
            {
                pos[axis] = block1.max[axis];
            }
        }
        return;
    }
    if (world->GetBlock(testBlockPos.x, testBlockPos.y + 1, testBlockPos.z) && playerBox.Intersects(block2))
    {
        if (axis == 1)
        {
            if (move[axis] > 0)
            {
                pos[axis] = block2.min[axis];
            }
            else if (move[axis] < 0)
            {
                pos[axis] = block2.max[axis];
            }
        }
        return;
    }
    pos[axis] += move[axis];
}

glm::vec3 CollisionSystem::Move(const glm::vec3 &position, const glm::vec3 &movement)
{
    glm::vec3 result = position;

    CheckCollisionAxis(result, movement, 1); // axis 1 = Y
    CheckCollisionAxis(result, movement, 0); // axis 0 = X
    CheckCollisionAxis(result, movement, 2); // axis 2 = Z
    return result;
}

bool CollisionSystem::IsOnGround(const glm::vec3 &position)
{
    int x = (int)glm::round(position.x);
    int y = (int)glm::floor(position.y);
    int z = (int)glm::round(position.z);
    return world->GetBlock(x, y, z) && GetPlayerAABB(position).Intersects(GetBlockAABB(x, y, z));
}