#include "outlineRenderer.h"

#include <glad/glad.h>
#include <glm/glm.hpp>
#include "camera.h"
#include "world.h"

OutlineRenderer::OutlineRenderer(unsigned size) : renderRadian(size)
{
    glEnable(GL_CULL_FACE);
    glClearColor(0.53f, 0.81f, 0.92f, 1.0f);

    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glGenBuffers(1, &EBO);

    glBindVertexArray(VAO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void *)0);
    glEnableVertexAttribArray(0);
    glBindVertexArray(0);
}
OutlineRenderer::~OutlineRenderer()
{
    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
    glDeleteBuffers(1, &EBO);
}
void OutlineRenderer::DrawOutline(World *world, Camera *camera, glm::vec3 pos)
{
    glLineWidth(5.0f);
    float vertices[] = {
        // 前面四个顶点 (z-)
        pos.x - 0.501f, pos.y - 0.501f, pos.z - 0.501f, // 0
        pos.x + 0.501f, pos.y - 0.501f, pos.z - 0.501f, // 1
        pos.x + 0.501f, pos.y + 0.501f, pos.z - 0.501f, // 2
        pos.x - 0.501f, pos.y + 0.501f, pos.z - 0.501f, // 3

        // 后面四个顶点 (z+)
        pos.x - 0.501f, pos.y - 0.501f, pos.z + 0.501f, // 4
        pos.x + 0.501f, pos.y - 0.501f, pos.z + 0.501f, // 5
        pos.x + 0.501f, pos.y + 0.501f, pos.z + 0.501f, // 6
        pos.x - 0.501f, pos.y + 0.501f, pos.z + 0.501f  // 7
    };

    unsigned int indices[] = {
        // 底面四边形
        0, 1, 1, 5, 5, 4, 4, 0,
        // 顶面四边形
        3, 2, 2, 6, 6, 7, 7, 3,
        // 垂直边
        0, 3, 1, 2, 4, 7, 5, 6};
    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);
    outline.Use();
    outline.setValue("projection", camera->GetPerspectiveMatrix());
    outline.setValue("view", camera->GetViewMatrix());
    outline.setValue("model", glm::mat4(1.0f));

    glDrawElements(GL_LINES, 24, GL_UNSIGNED_INT, 0);
}