#pragma once
class Camera;
class World;

#include "shader.h"
#include "texture.h"
class SkyboxRenderer
{
public:
    SkyboxRenderer();
    ~SkyboxRenderer();
    void Draw(Camera *camera);

private:
    Shader skybox = Shader("shaders/skyboxVert.vert", "shaders/skyboxFrag.frag");
    Texture cubemap = Texture("textures/gui/title/background");
    unsigned VAO,VBO,EBO;
};