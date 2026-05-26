#ifndef UI_SYSTEM_H
#define UI_SYSTEM_H

#include "shader.h"
#include "texture.h"

#include <vector>
class UISystem{
public:
    UISystem(unsigned screenWidth, unsigned screenHeight);
    ~UISystem();
    void ResizeScreen(unsigned screenWidth, unsigned screenHeight);
    void CreateQuad(int startX, int startY, int width, int height, float u, float v, float uvWidth, float uvHeight, std::string textureID);
private:
    Shader shader = Shader("shaders/uiVertex.vert", "shaders/uiFragment.frag");
    Texture texture = Texture("textures/gui");

    unsigned screenWidth;
    unsigned screenHeight;

    unsigned VAO,VBO,EBO;

    std::vector<float> vertices;
    std::vector<unsigned> indices;
};
#endif