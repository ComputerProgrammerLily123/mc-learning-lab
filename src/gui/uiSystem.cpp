#include "uiSystem.h"

#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
UISystem::UISystem(unsigned screenWidth, unsigned scrrenHeight) : screenWidth(screenWidth), screenHeight(scrrenHeight), shader("uiVertex.vert", "uiFragment.frag"), texture("textures/gui")
{
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glGenBuffers(1, &EBO);

    glBindVertexArray(VAO);
    
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void *)0);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void *)(3 * sizeof(float)));
    glEnableVertexAttribArray(0);
    glEnableVertexAttribArray(1);
    glBindVertexArray(0);

    texture.LoadTexture("icons.png");
    texture.Bind(1);
}
UISystem::~UISystem() {
    glDeleteVertexArrays(1,&VAO);
    glDeleteBuffers(1,&VBO);
    glDeleteBuffers(1,&EBO);
}
void UISystem::DrawQuad(int startX, int startY, int width, int height, float u, float v, float uvWidth, float uvHeight)
{
    float x = static_cast<float>(startX);
    float y = static_cast<float>(startY);
    float vertices[] =
        {
            x, y, 0.0f,u,v,
            x, y + height, 0.0f,u, v - uvHeight,
            x + width, y + height, 0.0f,uvWidth,v - uvHeight,
            x + width, y, 0.0f,uvWidth,v};
    unsigned indices[] =
        {
            0, 1, 2,
            0, 2, 3};
    glDisable(GL_DEPTH_TEST);

    glBindVertexArray(VAO);
    
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_DYNAMIC_DRAW);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);

    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_DYNAMIC_DRAW);

    shader.use();
    shader.setInt("textureIn", 1);
    glm::mat4 projection = glm::ortho(0.0f, (float)screenWidth, (float)screenHeight, 0.0f, -1.0f, 1.0f);
    shader.setMat4("projection", projection);

    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
    glEnable(GL_DEPTH_TEST);
}