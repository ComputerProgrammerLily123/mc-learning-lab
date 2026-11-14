#include "chunk.h"

#include <glad/glad.h>

#include "resource/blockstate/blockstateRegistry.h"
#include "resource/model/direction.h"
#include "world.h"

Chunk::Chunk(int x, int z, World* world) : chunkX(x), chunkZ(z), world(world) 
{
    constexpr std::size_t EstimatedFaces = 4096;

    vertices.reserve(EstimatedFaces * 4 * 9);
    indices.reserve(EstimatedFaces * 6);
    selectedModels.reserve(16);
}
Chunk::~Chunk() {}
void Chunk::UpdateMesh()
{
    vertices.clear();
    indices.clear();
    for (unsigned i = 0; i < CHUNK_WIDTH * CHUNK_WIDTH * CHUNK_HEIGHT; i++)
    {
        if (blockstateIDs[i])
        {
            int x = i % CHUNK_WIDTH;
            int z = (i / CHUNK_WIDTH) % CHUNK_WIDTH;
            int y = i / (CHUNK_WIDTH * CHUNK_WIDTH);
            AddVertices(x, y, z, blockstateIDs[i]);
        }
    }
    isDirty = true;
}
int Chunk::GetBlock(glm::u8vec3 position)
{
    return blockstateIDs[(position.y * CHUNK_WIDTH + position.z) * CHUNK_WIDTH + position.x];
}
int Chunk::GetBlock(unsigned x, unsigned y, unsigned z)
{
    if (x > 15 || y > 255 || z > 15)
        return 0;
    return blockstateIDs[(y * CHUNK_WIDTH + z) * CHUNK_WIDTH + x];
}
void Chunk::SetBlock(glm::u8vec3 position, blockstate::BlockStateId stateId)
{
    blockstateIDs[(position.y * CHUNK_WIDTH + position.z) * CHUNK_WIDTH + position.x] = stateId;
}
void Chunk::SetBlock(unsigned x, unsigned y, unsigned z, blockstate::BlockStateId stateId)
{
    blockstateIDs[(y * CHUNK_WIDTH + z) * CHUNK_WIDTH + x] = stateId;
}
void Chunk::AddVertices(int x, int y, int z, blockstate::BlockStateId stateId)
{
    const auto& state = blockstate::BlockStateRegistry::GetInstance().Get(stateId);

    const BlockBase block = blockRegister.GetBlock(state.blockId);

    const glm::ivec3 worldPosition{chunkX * CHUNK_WIDTH + x, y, chunkZ * CHUNK_WIDTH + z};

    resourceManager.GetBlockModels(ResourceLocation(block.GetName()), state, worldPosition, selectedModels);

    const glm::i32vec3 position{x, y, z};

    for (const model::BakedModel* bakedModel : selectedModels)
    {
        if (!bakedModel)
            continue;

        for (const auto& quad : bakedModel->generalQuads)
            EmitQuad(quad, position, state.blockId);

        for (std::size_t i = 0;i < model::DirectionToIndex(model::Direction::Count);++i)
        {
            const auto direction = static_cast<model::Direction>(i);

            if (!ShouldRenderFace(x, y, z, direction))
                continue;

            for (const auto& quad : bakedModel->faceQuads[i])
                EmitQuad(quad, position, state.blockId);
        }
    }
}
void Chunk::EmitQuad(const model::BakedQuad& quad, const glm::i32vec3 blockPosition, int blockId)
{
    auto fx = static_cast<float>(blockPosition.x + chunkX * CHUNK_WIDTH);
    auto fy = static_cast<float>(blockPosition.y);
    auto fz = static_cast<float>(blockPosition.z + chunkZ * CHUNK_WIDTH);
    unsigned startIndex = vertices.size() / 9;
    for (const auto& vertex : quad.vertices)
    {
        vertices.insert(vertices.end(), {
            vertex.position.x + fx,
            vertex.position.y + fy,
            vertex.position.z + fz,
            vertex.uv.x,
            vertex.uv.y,
            vertex.normal.x,
            vertex.normal.y,
            vertex.normal.z,
            static_cast<float>(blockId)
        });
    }
    indices.insert(indices.end(), {startIndex, startIndex + 1, startIndex + 2,
        startIndex, startIndex + 2, startIndex + 3});
}
    
bool Chunk::ShouldRenderFace(int x, int y, int z, model::Direction face)
{
    glm::ivec3 offset{0};

    switch (face)
    {
    case model::Direction::North:
        offset = {0, 0, -1};
        break;

    case model::Direction::South:
        offset = {0, 0, 1};
        break;

    case model::Direction::West:
        offset = {-1, 0, 0};
        break;

    case model::Direction::East:
        offset = {1, 0, 0};
        break;

    case model::Direction::Up:
        offset = {0, 1, 0};
        break;

    case model::Direction::Down:
        offset = {0, -1, 0};
        break;

    case model::Direction::Count:
        return true;
    }

    const int worldX = chunkX * CHUNK_WIDTH + x;
    const int worldZ = chunkZ * CHUNK_WIDTH + z;

    const int neighborStateId = world->GetBlockStateId(worldX + offset.x, y + offset.y, worldZ + offset.z);
    const auto& neighborState = blockstate::BlockStateRegistry::GetInstance().Get(neighborStateId);
    return !blockRegister.GetBlock(neighborState.blockId).GetSolid();
}
const std::vector<float>& Chunk::GetVertices()
{
    return vertices;
}
const std::vector<unsigned>& Chunk::GetIndices()
{
    return indices;
}