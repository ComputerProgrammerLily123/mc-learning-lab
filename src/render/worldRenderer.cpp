#include "worldRenderer.h"
#include <glad/glad.h>
#include <glm/gtc/matrix_access.hpp>
#include "world.h"
#include "camera.h"
WorldRenderer::WorldRenderer(unsigned size) : renderRadian(size)
{
    glClearColor(0.53f, 0.81f, 0.92f, 1.0f);
    texture.Bind(0);
    std::vector<std::string> names;
    for (int i = 0; i < BlockRegister::GetInstance().GetBlockCount(); i++)
    {
        auto &textureList = BlockRegister::GetInstance().GetBlock(i).GetTextures();
        names.insert(names.end(), textureList.begin(), textureList.end());
    }
    texture.CreateAtlas(names);
    glGenVertexArrays(1, &VAO);
}
WorldRenderer::~WorldRenderer()
{
    glDeleteVertexArrays(1, &VAO);
}
void WorldRenderer::Clear()
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}
void WorldRenderer::Draw(World *world, Camera *camera, int32_t centralX, int32_t centralZ)
{
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_CULL_FACE);
    Clear();
    shader.Use();
    texture.Bind(0);
    shader.setValue("textureIn", 0);
    shader.setValue("projection", camera->GetPerspectiveMatrix());
    shader.setValue("view", camera->GetViewMatrix());
    shader.setValue("model", glm::mat4(1.0f));
    int drawCount = 0;
    for (int32_t i = -renderRadian; i < renderRadian; i++)
    {
        for (int32_t j = -renderRadian; j < renderRadian; j++)
        {
            if (!IsChunkVisible(i + centralX, j + centralZ, camera->GetPerspectiveMatrix() * camera->GetViewMatrix()))
                continue;
            auto chunk = world->GetChunk(i + centralX, j + centralZ);
            if (!chunk)
            {
                world->CreateChunk(i + centralX, j + centralZ);
                world->InitChunk(i + centralX, j + centralZ);
                chunk = world->GetChunk(i + centralX, j + centralZ);
                chunk->UpdateMesh();
            }
            DrawMesh(i + centralX, j + centralZ, chunk->GetVertices(), chunk->GetIndices(), chunk->isDirty);
            drawCount++;
        }
    }
}

void WorldRenderer::DrawMesh(int32_t chunkX, int32_t chunkZ, const std::vector<float> &vertices, const std::vector<unsigned> &indices, bool &isDirty)
{
    if (indices.empty()) return;
    uint64_t key = ((uint64_t)chunkX << 32) | (uint32_t)chunkZ;
    auto& chunkVBO = VBO[key];
    auto& chunkEBO = EBO[key];
    
    if (chunkVBO == 0) {
        glGenBuffers(1, &chunkVBO);
        glGenBuffers(1, &chunkEBO);
    }
    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, chunkVBO);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, chunkEBO);
    if (isDirty) {
        glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(float), vertices.data(), GL_STATIC_DRAW);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned), indices.data(), GL_STATIC_DRAW);
        isDirty = false;
    }
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 9 * sizeof(float), (void *)0);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 9 * sizeof(float), (void *)(3 * sizeof(float)));
    glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 9 * sizeof(float), (void *)(5 * sizeof(float)));
    glVertexAttribPointer(3, 1, GL_FLOAT, GL_FALSE, 9 * sizeof(float), (void *)(8 * sizeof(float)));
    glEnableVertexAttribArray(0);
    glEnableVertexAttribArray(1);
    glEnableVertexAttribArray(2);
    glEnableVertexAttribArray(3);
    glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, 0);
    glBindVertexArray(0);
}
bool WorldRenderer::IsChunkVisible(int32_t chunkX, int32_t chunkZ, const glm::mat4 &vp)
{
    float x = chunkX * 16.0f;
    float z = chunkZ * 16.0f;
    glm::vec3 min(x, 0, z);
    glm::vec3 max(x + 16, 256, z + 16);
    glm::vec4 planes[6];
    planes[0] = glm::row(vp, 3) + glm::row(vp, 0);
    planes[1] = glm::row(vp, 3) - glm::row(vp, 0);
    planes[2] = glm::row(vp, 3) + glm::row(vp, 1);
    planes[3] = glm::row(vp, 3) - glm::row(vp, 1);
    planes[4] = glm::row(vp, 3) + glm::row(vp, 2);
    planes[5] = glm::row(vp, 3) - glm::row(vp, 2);
    for (auto &p : planes)
    {
        float len = glm::length(glm::vec3(p));
        if (len > 0)
            p /= len;
    }
    for (const auto &p : planes)
    {
        glm::vec3 positive;
        positive.x = (p.x > 0) ? max.x : min.x;
        positive.y = (p.y > 0) ? max.y : min.y;
        positive.z = (p.z > 0) ? max.z : min.z;
        if (glm::dot(glm::vec3(p), positive) + p.w < 0)
        {
            return false;
        }
    }
    return true;
}