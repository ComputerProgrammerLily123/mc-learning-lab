#include "uiRenderer.h"

#include <glad/glad.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include "resource/resourceManager.h"

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
    renderQueue.clear();
}
void UIRenderer::AddRenderQueue(int startX, int startY, int width, int height, const std::string& textureId)
{
    auto x = static_cast<float>(startX);
    auto y = static_cast<float>(startY);

    auto& vertices = renderQueue[textureId].vertices;
    auto& indices = renderQueue[textureId].indices;

    unsigned startIndex = vertices.size() / 5;

    auto region = ResourceManager::GetInstance().GetTextureRegion(textureId);
    if(!region) return;

    float u1 = region->GetU1();
    float v1 = region->GetV1();
    float u2 = region->GetU2();
    float v2 = region->GetV2();

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

    vertices_buffer.clear();
    indices_buffer.clear();
    offsets.clear();
    offsets.push_back(0);

    for (const auto& [textureId, entry] : renderQueue) {
        auto vertexOffset = static_cast<unsigned int>(vertices_buffer.size() / 5);
        vertices_buffer.insert(vertices_buffer.end(), entry.vertices.begin(), entry.vertices.end());
        for (unsigned index : entry.indices) {
            indices_buffer.push_back(index + vertexOffset);
        }
        offsets.push_back(offsets.back() + entry.indices.size());
    }

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, vertices_buffer.size() * sizeof(float), vertices_buffer.data(), GL_DYNAMIC_DRAW);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices_buffer.size() * sizeof(unsigned), indices_buffer.data(), GL_DYNAMIC_DRAW);

    shader.Use();
    shader.setValue("textureIn", 1);
    glm::mat4 projection = glm::ortho(0.0f, (float)screenWidth, (float)screenHeight, 0.0f, -1.0f, 1.0f);
    shader.setValue("projection", projection);

    int groupIndex = 0;
    for(const auto& pairs : renderQueue)
    {
        auto region = ResourceManager::GetInstance().GetTextureRegion(pairs.first);
        if(region)
        {
            region->Bind(1);
            glDrawElements(GL_TRIANGLES, offsets[groupIndex + 1] - offsets[groupIndex], GL_UNSIGNED_INT, reinterpret_cast<const void*>(offsets[groupIndex] * sizeof(unsigned int)));
        }
        groupIndex++;
    }
    glEnable(GL_DEPTH_TEST);
    glDisable(GL_BLEND);
}