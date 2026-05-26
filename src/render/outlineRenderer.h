#ifndef OUTLINE_RENDERER_H
#define OUTLINE_RENDERER_H
class Camera;
class World;

#include "shader.h"
class OutlineRenderer{
public:
    OutlineRenderer(unsigned size);
    ~OutlineRenderer();
    void DrawOutline(World* world, Camera* camera, glm::vec3 pos);
private:
    unsigned renderRadian;
    Shader outline = Shader("shaders/outlineVert.vert", "shaders/outlineFrag.frag");

    unsigned VAO, VBO, EBO;
};
#endif