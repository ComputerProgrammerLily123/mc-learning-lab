#pragma once
class Camera;
class World;

#include "shader.h"
#include "core/sceneManager.h"

class OutlineRenderer : public SceneObject
{
public:
    OutlineRenderer();
    OutlineRenderer(const OutlineRenderer&) = default;
    OutlineRenderer(OutlineRenderer&&) = delete;
    OutlineRenderer& operator=(const OutlineRenderer&) = delete;
    OutlineRenderer& operator=(OutlineRenderer&&) = delete;
    ~OutlineRenderer();
    void DrawOutline(World* world, Camera* camera, glm::vec3 pos);
private:
    Shader outline = Shader("shaders/outlineVert.vert", "shaders/outlineFrag.frag");

    unsigned VAO, VBO, EBO;
};