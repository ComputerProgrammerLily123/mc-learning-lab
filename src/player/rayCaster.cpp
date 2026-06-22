#include "rayCaster.h"

#include "world/world.h"
RayCaster::RayCaster(World *const world, float maxDistance, float step) : world(world), maxDistance(maxDistance), step(step) {}
int RayCaster::Ray(glm::vec3 startPoint, glm::vec3 direction, glm::i16vec3 &targetPoint)
{
    float currentDistance = 0.0f;
    glm::vec3 currentPoint = startPoint;
    int result = 0;
    while (currentDistance <= maxDistance)
    {
        glm::i16vec3 target = glm::round(currentPoint);
        if (world->GetBlock(target.x, target.y, target.z))
        {
            targetPoint = target;
            result = 1;
            break;
        }
        currentDistance += step;
        currentPoint += step * direction;
    }
    return result;
}
int RayCaster::Ray(glm::vec3 startPoint, glm::vec3 direction, glm::i16vec3 &targetPoint, glm::ivec3 &offset)
{
    float currentDistance = 0.0f;
    glm::vec3 currentPoint = startPoint;
    int result = 0;
    while (currentDistance <= maxDistance)
    {
        glm::i16vec3 target = glm::round(currentPoint);
        if (world->GetBlock(target.x, target.y, target.z))
        {
            float offsetX = target.x - currentPoint.x;
            float offsetY = target.y - currentPoint.y;
            float offsetZ = target.z - currentPoint.z;
            if(abs(offsetX) > abs(offsetY) && abs(offsetX) > abs(offsetZ))offset = glm::ivec3(offsetX < 0 ? 1 : -1,0,0);
            if(abs(offsetY) > abs(offsetX) && abs(offsetY) > abs(offsetZ))offset = glm::ivec3(0,offsetY < 0 ? 1 : -1,0);
            if(abs(offsetZ) > abs(offsetX) && abs(offsetZ) > abs(offsetY))offset = glm::ivec3(0,0,offsetZ < 0 ? 1 : -1);
            targetPoint = target;
            result = 1;
            break;
        }
        currentDistance += step;
        currentPoint += step * direction;
    }
    return result;
}