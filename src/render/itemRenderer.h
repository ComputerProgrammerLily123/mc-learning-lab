#pragma once

#include <vector>

#include "gui/rectTransform.h"
#include "item/itemRegister.h"
#include "resource/resourceLocation.h"
#include "shader.h"

class ItemRenderer
{
public:
    ItemRenderer(unsigned screenWidth, unsigned screenHeight);
    ~ItemRenderer();
    ItemRenderer(const ItemRenderer&) = delete;
    ItemRenderer(ItemRenderer&&) = delete;
    ItemRenderer& operator=(const ItemRenderer&) = delete;
    ItemRenderer& operator=(ItemRenderer&&) = delete;

    void ResizeScreen(unsigned width, unsigned height);
    void ClearRenderQueue();
    void AddRenderQueue(const ItemStack& stack, const UIScreenRect& rect);
    void Draw();

private:
    Shader shader = Shader("shaders/itemVert.vert", "shaders/itemFrag.frag");

    struct ItemVertex
    {
        glm::vec3 position;
        glm::vec2 uv;
        glm::vec3 normal;
        float tintIndex = -1.0f;
    };

    struct ItemMesh
    {
        unsigned vao = 0;
        unsigned vbo = 0;
        unsigned ebo = 0;
        unsigned indexCount = 0;
    };

    struct RenderCommand
    {
        ItemId itemId {};
        UIScreenRect rect;
    };
    
    ItemMesh BuildMesh(ItemId itemId);

    unsigned screenWidth;
    unsigned screenHeight;

    std::unordered_map<ItemId, ItemMesh> meshes;
    std::vector<RenderCommand> renderQueue;
};