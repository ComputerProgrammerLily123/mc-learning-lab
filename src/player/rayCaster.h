#ifndef RAY_CASTER_H
#define RAY_CASTER_H

#include <glm/glm.hpp>
class World;
class RayCaster{
public:
    RayCaster(World* const world,float maxDistance,float step);
    int Ray(glm::vec3 startPoint,glm::vec3 direction,glm::i16vec3& targetPoint);
    int Ray(glm::vec3 startPoint,glm::vec3 direction,glm::i16vec3& targetPoint, glm::ivec3& offset);
private:
    World* world;
    float maxDistance;
    float step;
};
#endif