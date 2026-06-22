#pragma once

#include "shader.h"
#include "resource/texture.h"

#include <vector>
class UIRenderer{
public:
    UIRenderer(unsigned screenWidth, unsigned screenHeight);
    ~UIRenderer();
    void ResizeScreen(unsigned screenWidth, unsigned screenHeight);
    void DrawQuad(int startX, int startY, int width, int height, float u, float v, float uvWidth, float uvHeight, std::string textureID);
private:
    Shader shader = Shader("shaders/uiVertex.vert", "shaders/uiFragment.frag");
    Texture texture = Texture("textures/gui");

    unsigned screenWidth;
    unsigned screenHeight;

    unsigned VAO,VBO,EBO;

    std::vector<float> vertices;
    std::vector<unsigned> indices;
};