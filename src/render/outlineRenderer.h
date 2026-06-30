#pragma once
class Camera;
class World;

#include "shader.h"
class OutlineRenderer
{
public:
    OutlineRenderer();
    ~OutlineRenderer();
    void DrawOutline(World* world, Camera* camera, glm::vec3 pos);
private:
    Shader outline = Shader("shaders/outlineVert.vert", "shaders/outlineFrag.frag");

    unsigned VAO, VBO, EBO;
};