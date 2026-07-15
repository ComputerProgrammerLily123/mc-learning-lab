#pragma once
class Camera;
class World;

#include "shader.h"
#include "core/sceneManager.h"

class OutlineRenderer : public SceneObject
{
public:
    OutlineRenderer();
    OutlineRenderer(const OutlineRenderer&) = delete;
    OutlineRenderer(OutlineRenderer&&) = delete;
    OutlineRenderer& operator=(const OutlineRenderer&) = delete;
    OutlineRenderer& operator=(OutlineRenderer&&) = delete;
    ~OutlineRenderer();
    void DrawOutline(World* world, Camera* camera, glm::vec3 pos);
private:
    Shader outline = Shader("shaders/outlineVert.vert", "shaders/outlineFrag.frag");

    unsigned VAO = 0, VBO = 0, EBO = 0;
};