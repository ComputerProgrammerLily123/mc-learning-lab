#ifndef COLLISION_H
#define COLLISION_H
#include <glm/glm.hpp>
#include <vector>
struct AABB
{
    glm::vec3 min;
    glm::vec3 max;
    AABB() = default;
    AABB(const glm::vec3& center, const glm::vec3& halfSize) {
        min = center - halfSize;
        max = center + halfSize;
    }
    bool Intersects(const AABB &other) const
    {
        return (min.x <= other.max.x && max.x >= other.min.x) &&
               (min.y <= other.max.y && max.y >= other.min.y) &&
               (min.z <= other.max.z && max.z >= other.min.z);
    }
    glm::vec3 GetCenter() const { return (min + max) * 0.5f; }
    glm::vec3 GetHalfSize() const { return (max - min) * 0.5f; }
};
inline AABB GetPlayerAABB(const glm::vec3& position) {
    return AABB(position + glm::vec3(0.0f, 0.9f, 0.0f), glm::vec3(0.3f, 0.9f, 0.3f));
}
inline AABB GetBlockAABB(int x, int y, int z) {
    return AABB(glm::vec3(x, y, z), glm::vec3(0.5f, 0.5f, 0.5f));
}
class World;
class CollisionSystem
{
public:
    CollisionSystem(World* world);
    glm::vec3 Move(const glm::vec3& position, const glm::vec3& movement);
    bool IsOnGround(const glm::vec3& position);
private:
    World* world;
};
#endif