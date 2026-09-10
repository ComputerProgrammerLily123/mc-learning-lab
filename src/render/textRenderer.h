#pragma once
#pragma once

#include "shader.h"

#include <vector>

class TextRenderer{
public:
    TextRenderer(const TextRenderer&) = default;
    TextRenderer(TextRenderer&&) = delete;
    TextRenderer& operator=(const TextRenderer&) = default;
    TextRenderer& operator=(TextRenderer&&) = delete;
    TextRenderer(unsigned screenWidth, unsigned screenHeight);
    ~TextRenderer();
    void ResizeScreen(unsigned screenWidth, unsigned screenHeight);
    void ClearRenderQueue();
    void AddRenderQueue(int startX, int startY, int width, int height, const std::string& textureId);
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