#pragma once

#include "shader.h"

#include <vector>
class UIRenderer{
public:
    UIRenderer(const UIRenderer&) = delete;
    UIRenderer(UIRenderer&&) = delete;
    UIRenderer& operator=(const UIRenderer&) = delete;
    UIRenderer& operator=(UIRenderer&&) = delete;
    UIRenderer(unsigned screenWidth, unsigned screenHeight);
    ~UIRenderer();
    void ResizeScreen(unsigned screenWidth, unsigned screenHeight);
    void ClearRenderQueue();
    void AddRenderQueue(int startX, int startY, int width, int height, const std::string& textureId);
    void AddTextRenderQueue(int startX, int startY, int glyphWidth, int glyphHeight, const std::string& textureId, const std::string& text);
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
        std::string textureId;
    };

    std::vector<RenderQueueEntry> renderQueue;

    std::vector<float> vertices_buffer;
    std::vector<unsigned> indices_buffer;
    std::vector<unsigned> offsets = {0};
};