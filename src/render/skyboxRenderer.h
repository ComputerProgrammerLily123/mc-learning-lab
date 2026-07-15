#pragma once
class Camera;

#include "shader.h"
#include "resource/texture.h"
#include "core/sceneManager.h"
class SkyboxRenderer : public SceneObject
{
public:
    SkyboxRenderer();
    SkyboxRenderer(const SkyboxRenderer&) = delete;
    SkyboxRenderer(SkyboxRenderer&&) = delete;
    SkyboxRenderer& operator=(const SkyboxRenderer&) = delete;
    SkyboxRenderer& operator=(SkyboxRenderer&&) = delete;
    ~SkyboxRenderer();
    void Draw(Camera *camera);

private:
    Shader skybox = Shader("shaders/skyboxVert.vert", "shaders/skyboxFrag.frag");
    Texture cubemap = Texture("textures/gui/title/background");
    unsigned VAO = 0,VBO = 0,EBO = 0;
};