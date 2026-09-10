#pragma once

#include "shader.h"

#include <vector>
class UIRenderer{
public:
    UIRenderer(const UIRenderer&) = default;
    UIRenderer(UIRenderer&&) = delete;
    UIRenderer& operator=(const UIRenderer&) = default;
    UIRenderer& operator=(UIRenderer&&) = delete;
    UIRenderer(unsigned screenWidth, unsigned screenHeight);
    ~UIRenderer();
    void ResizeScreen(unsigned screenWidth, unsigned screenHeight);
    void ClearRenderQueue();
    void AddRenderQueue(int startX, int startY, int width, int height, const std::string& textureId);
    void AddTextRenderQueue(int startX, int startY, int width, int height, const std::string& textureId, const std::vector<glm::vec4>& uv_list);
    void Draw();
private:
    Shader shader = Shader("shaders/uiVertex.vert", "shaders/uiFragment.frag");

    unsigned screenWidth;
    unsigned screenHeight;

    unsigned VAO = 0,VBO = 0,EBO = 0;

    struct RenderQueueEntry
    {
        std::vector<float> vertices;
        std::vector<unsigned> indices;
    };

    std::unordered_map<std::string, RenderQueueEntry> renderQueue;

    std::vector<float> vertices_buffer;
    std::vector<unsigned> indices_buffer;
    std::vector<unsigned> offsets = {0};
};