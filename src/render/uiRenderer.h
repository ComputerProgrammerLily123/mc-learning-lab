#pragma once

#include "shader.h"
#include "resource/texture.h"

#include <vector>
class UIRenderer{
public:
    UIRenderer(const UIRenderer&) = default;
    UIRenderer(UIRenderer&&) = delete;
    UIRenderer& operator=(const UIRenderer&) = default;
    UIRenderer& operator=(UIRenderer&&) = delete;
    UIRenderer(unsigned screenWidth, unsigned screenHeight);
    ~UIRenderer();
    void ResizeScreen(unsigned screenWidth, unsigned screenHeight);
    void ClearRenderQueue();
    void AddRenderQueue(int startX, int startY, int width, int height, float u1, float v1, float u2, float v2);
    void Draw();
private:
    Shader shader = Shader("shaders/uiVertex.vert", "shaders/uiFragment.frag");
    Texture texture = Texture("textures/gui");

    unsigned screenWidth;
    unsigned screenHeight;

    unsigned VAO = 0,VBO = 0,EBO = 0;

    std::vector<float> vertices;
    std::vector<unsigned> indices;
};