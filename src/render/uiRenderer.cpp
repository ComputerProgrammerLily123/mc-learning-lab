#include "uiRenderer.h"

#include <glad/glad.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

UIRenderer::UIRenderer(unsigned screenWidth, unsigned scrrenHeight) : screenWidth(screenWidth), screenHeight(scrrenHeight)
{
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glGenBuffers(1, &EBO);

    glBindVertexArray(VAO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(0);
    glEnableVertexAttribArray(1);
    glBindVertexArray(0);
}
UIRenderer::~UIRenderer()
{
    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
    glDeleteBuffers(1, &EBO);
}
void UIRenderer::ResizeScreen(unsigned screenWidth, unsigned screenHeight)
{
    this->screenWidth = screenWidth;
    this->screenHeight = screenHeight;
}
void UIRenderer::ClearRenderQueue()
{
    vertices.clear();
    indices.clear();
}
void UIRenderer::AddRenderQueue(int startX, int startY, int width, int height, float u1, float v1, float u2, float v2)
{
    auto x = static_cast<float>(startX);
    auto y = static_cast<float>(startY);

    unsigned startIndex = vertices.size() / 5;
    vertices.insert(vertices.end(),
                    {x, y, 0.0f, u1, v1,
                     x, y + height, 0.0f, u1, v2,
                     x + width, y + height, 0.0f, u2, v2,
                     x + width, y, 0.0f, u2, v1});
    indices.insert(indices.end(),
                   {startIndex, startIndex + 1, startIndex + 2,
                    startIndex, startIndex + 2, startIndex + 3});
}
void UIRenderer::Draw()
{
    glDisable(GL_DEPTH_TEST);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glBindVertexArray(VAO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);

    glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(float), vertices.data(), GL_STATIC_DRAW);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);

    glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned), indices.data(), GL_STATIC_DRAW);

    shader.Use();
    shader.setValue("textureIn", 1);
    texture.Bind(1);
    texture.CreateTexture("widgets.png");
    glm::mat4 projection = glm::ortho(0.0f, (float)screenWidth, (float)screenHeight, 0.0f, -1.0f, 1.0f);
    shader.setValue("projection", projection);

    glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, 0);
    glEnable(GL_DEPTH_TEST);
    glDisable(GL_BLEND);
}