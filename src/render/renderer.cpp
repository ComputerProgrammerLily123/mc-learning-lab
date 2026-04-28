#include "renderer.h"

#include <glad/glad.h>
#include <glm/glm.hpp>
#include "shader.h"
#include "camera.h"
#include "texture.h"
#include "world.h"
Renderer::Renderer(unsigned size) : worldSize(size), texture("textures/blocks"), shader("vertex.vert", "fragment.frag")
{
    glEnable(GL_CULL_FACE);
    glClearColor(0.53f, 0.81f, 0.92f, 1.0f);
    texture.CreateAtlas();
}
Renderer::~Renderer()
{
}
void Renderer::Clear()
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}
void Renderer::Draw(World *world, Camera *camera)
{
    glEnable(GL_DEPTH_TEST);
    Clear();
    shader.use();
    texture.Bind(0);
    shader.setInt("textureIn", 0);
    shader.setMat4("projection", camera->GetPerspectiveMatrix());
    shader.setMat4("view", camera->GetViewMatrix());
    shader.setMat4("model", glm::mat4(1.0f));
    for (unsigned i = 0; i < worldSize; i++)
    {
        for (unsigned j = 0; j < worldSize; j++)
        {
            if (world->GetChunk(i, j))
            {
                world->GetChunk(i, j)->DrawMesh();
            }
        }
    }
}