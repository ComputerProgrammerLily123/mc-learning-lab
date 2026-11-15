#include "itemRenderer.h"

#include <glad/glad.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "resource/resourceManager.h"

ItemRenderer::ItemRenderer(unsigned screenWidth, unsigned screenHeight) : screenWidth(screenWidth), screenHeight(screenHeight)
{
}
ItemRenderer::~ItemRenderer()
{
    for (auto& [itemId, itemMesh] : meshes)
    {
        glDeleteVertexArrays(1, &itemMesh.vao);
        glDeleteBuffers(1, &itemMesh.ebo);
        glDeleteBuffers(1, &itemMesh.vbo);
    }
}
void ItemRenderer::ResizeScreen(unsigned width, unsigned height)
{
    screenWidth = width;
    screenHeight = height;
}

void ItemRenderer::AddRenderQueue(const ItemStack& stack, const UIScreenRect& rect)
{
    if (stack.IsEmpty())
        return;

    if (meshes.find(stack.itemId) == meshes.end())
        meshes.emplace(stack.itemId, BuildMesh(stack.itemId));

    renderQueue.push_back({stack.itemId, rect});
}

void ItemRenderer::ClearRenderQueue()
{
    renderQueue.clear();
}
ItemRenderer::ItemMesh ItemRenderer::BuildMesh(ItemId itemId)
{
    const auto& item = ItemRegister::GetInstance().GetItem(itemId);

    auto bakedItem = ResourceManager::GetInstance().GetItemModel(item.location);

    if (!bakedItem || !bakedItem->model)
        return {};

    std::vector<ItemVertex> vertices;
    std::vector<unsigned> indices;

    const auto appendQuad = [&](const model::BakedQuad& quad)
    {
        const unsigned base = static_cast<unsigned>(vertices.size());

        for (const auto& vertex : quad.vertices)
        {
            vertices.push_back({vertex.position,
                                vertex.uv,
                                vertex.normal,
                                static_cast<float>(quad.tintIndex)});
        }

        indices.insert(indices.end(), {base, base + 1, base + 2,
                                       base, base + 2, base + 3});
    };

    for (const auto& quad : bakedItem->model->generalQuads)
        appendQuad(quad);

    for (const auto& quads : bakedItem->model->faceQuads)
        for (const auto& quad : quads)
            appendQuad(quad);

    ItemMesh mesh;
    mesh.indexCount = static_cast<unsigned>(indices.size());
    glGenVertexArrays(1, &mesh.vao);
    glGenBuffers(1, &mesh.vbo);
    glGenBuffers(1, &mesh.ebo);

    glBindVertexArray(mesh.vao);

    glBindBuffer(GL_ARRAY_BUFFER, mesh.vbo);
    glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(ItemVertex), vertices.data(), GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mesh.ebo);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned), indices.data(), GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(ItemVertex),
                          reinterpret_cast<void*>(offsetof(ItemVertex, position)));
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(ItemVertex),
                          reinterpret_cast<void*>(offsetof(ItemVertex, uv)));
    glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, sizeof(ItemVertex),
                          reinterpret_cast<void*>(offsetof(ItemVertex, normal)));
    glVertexAttribPointer(3, 1, GL_FLOAT, GL_FALSE, sizeof(ItemVertex),
                          reinterpret_cast<void*>(offsetof(ItemVertex, tintIndex)));

    for (unsigned index = 0; index < 4; ++index)
        glEnableVertexAttribArray(index);
    glBindVertexArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    return mesh;
}
void ItemRenderer::Draw()
{
    if (renderQueue.empty())
        return;

    auto atlas = ResourceManager::GetInstance().GetBlockAtlas();
    if (!atlas)
        return;
    glClear(GL_DEPTH_BUFFER_BIT);
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    shader.Use();
    atlas->Bind(0);
    shader.setValue("textureIn", 0);
    glm::mat4 projection = glm::ortho(0.0f, static_cast<float>(screenWidth), 0.0f, static_cast<float>(screenHeight), -100.0f, 100.0f);
    shader.setValue("projection", projection);
    for (const RenderCommand& command : renderQueue)
    {
        const auto meshIt = meshes.find(command.itemId);
        if (meshIt == meshes.end())
            continue;

        const ItemMesh& mesh = meshIt->second;

        const auto& item = ItemRegister::GetInstance().GetItem(command.itemId);
        const auto bakedItem =
            ResourceManager::GetInstance().GetItemModel(item.location);

        if (!bakedItem || !bakedItem->model || mesh.indexCount == 0)
            continue;

        model::DisplayMode guiTransform;

        if (bakedItem->model->display &&
            bakedItem->model->display->gui)
        {
            guiTransform = *bakedItem->model->display->gui;
        }

        const glm::vec3 rotation =
            guiTransform.rotation.value_or(glm::vec3(0.0f));
        const glm::vec3 translation =
            guiTransform.translation.value_or(glm::vec3(0.0f)) / 16.0f;
        const glm::vec3 scale =
            guiTransform.scale.value_or(glm::vec3(1.0f));

        const float iconSize =
            std::min(command.rect.size.x, command.rect.size.y);

        const float centerX =
            command.rect.position.x + command.rect.size.x * 0.5f;
        const float centerY =
            static_cast<float>(screenHeight) -
            (command.rect.position.y + command.rect.size.y * 0.5f);

        glm::mat4 modelMatrix(1.0f);
        modelMatrix = glm::translate(
            modelMatrix, glm::vec3(centerX, centerY, 0.0f));
        modelMatrix = glm::scale(
            modelMatrix, glm::vec3(iconSize));
        modelMatrix = glm::translate(modelMatrix, translation);
        modelMatrix = glm::rotate(
            modelMatrix, glm::radians(rotation.x), glm::vec3(1, 0, 0));
        modelMatrix = glm::rotate(
            modelMatrix, glm::radians(rotation.y), glm::vec3(0, 1, 0));
        modelMatrix = glm::rotate(
            modelMatrix, glm::radians(rotation.z), glm::vec3(0, 0, 1));
        modelMatrix = glm::scale(modelMatrix, scale);

        shader.setValue("model", modelMatrix);

        glBindVertexArray(mesh.vao);
        glDrawElements(GL_TRIANGLES, mesh.indexCount, GL_UNSIGNED_INT, nullptr);
    }
    glBindVertexArray(0);
    glDisable(GL_DEPTH_TEST);
    glDisable(GL_BLEND);
}