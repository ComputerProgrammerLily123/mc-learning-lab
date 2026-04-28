#ifndef UI_SYSTEM_H
#define UI_SYSTEM_H

#include "shader.h"
#include "texture.h"
class UISystem{
public:
    UISystem(unsigned screenWidth, unsigned screenHeight);
    ~UISystem();
    void DrawQuad(int startX, int startY, int width, int height, float u, float v, float uvWidth, float uvHeight);
private:
    Shader shader;
    Texture texture;

    unsigned screenWidth;
    unsigned screenHeight;

    unsigned VAO,VBO,EBO;
};
#endif