#include "skyboxRenderer.h"
#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include "camera.h"

SkyboxRenderer::SkyboxRenderer()
{
    float vertices[] = {
        // 前面 (z = 1)
        -1.0f, -1.0f, 1.0f, // 0
        -1.0f, 1.0f, 1.0f,  // 1
        1.0f, 1.0f, 1.0f,   // 2
        1.0f, -1.0f, 1.0f,  // 3

        // 后面 (z = -1)
        1.0f, -1.0f, -1.0f, // 4
        1.0f, 1.0f, -1.0f,  // 5
        -1.0f, 1.0f, -1.0f, // 6
        -1.0f, -1.0f, -1.0f // 7
    };

    unsigned int indices[] = {
        // 前面
        0, 1, 2, 0, 2, 3,
        // 后面
        4, 5, 6, 4, 6, 7,
        // 左面
        7, 6, 1, 7, 1, 0,
        // 右面
        3, 2, 5, 3, 5, 4,
        // 顶面
        1, 6, 5, 1, 5, 2,
        // 底面
        7, 0, 3, 7, 3, 4};

    // 创建 VAO/VBO/EBO
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glGenBuffers(1, &EBO);

    glBindVertexArray(VAO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void *)0);
    glEnableVertexAttribArray(0);

    glBindVertexArray(0);
}

SkyboxRenderer::~SkyboxRenderer()
{
    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
    glDeleteBuffers(1, &EBO);
}

void SkyboxRenderer::Draw(Camera *camera)
{
    glDisable(GL_DEPTH_TEST);
    glDepthMask(GL_FALSE);
    skybox.Use();
    skybox.setValue("projection", camera->GetPerspectiveMatrix());
    skybox.setValue("view", camera->GetViewMatrix());
    skybox.setValue("textureIn", 2);
    cubemap.Bind(2);
    cubemap.CreateCubemap({"panorama_1.png", "panorama_3.png", "panorama_4.png", "panorama_5.png", "panorama_0.png", "panorama_2.png"});
    glBindVertexArray(VAO);
    glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);
    glDepthMask(GL_TRUE);
    glEnable(GL_DEPTH_TEST);
}