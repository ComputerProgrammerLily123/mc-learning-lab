#pragma once

#include "shader.h"

#include <vector>
#include "resource/resourceLocation.h"
#include "itemRenderer.h"
#include "config/gameConstant.h"
using namespace GameConstant;
class UIRenderer{
public:
    UIRenderer(const UIRenderer&) = delete;
    UIRenderer(UIRenderer&&) = delete;
    UIRenderer& operator=(const UIRenderer&) = delete;
    UIRenderer& operator=(UIRenderer&&) = delete;
    UIRenderer(unsigned screenWidth, unsigned screenHeight);
    ~UIRenderer();
    void ResizeScreen(unsigned screenWidth, unsigned screenHeight);
    void AddItemRenderQueue(const ItemStack& stack, const UIScreenRect& rect);
    void ClearRenderQueue();
    void AddRenderQueue(int startX, int startY, int width, int height, const ResourceLocation& location);
    void AddTextRenderQueue(int startX, int startY, int width, int height, const ResourceLocation& location, const std::string& text);
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
        ResourceLocation textureLocation;
    };

    std::vector<RenderQueueEntry> renderQueue;

    std::vector<float> vertices_buffer;
    std::vector<unsigned> indices_buffer;
    std::vector<unsigned> offsets = {0};

    ItemRenderer itemRenderer {INITIAL_SCREEN_WIDTH, INITIAL_SCREEN_HEIGHT};
};