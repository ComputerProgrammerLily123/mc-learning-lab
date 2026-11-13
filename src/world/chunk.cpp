#include "chunk.h"

#include <glad/glad.h>

#include "resource/model/direction.h"
#include "world.h"

Chunk::Chunk(int x, int z, World* world) : chunkX(x), chunkZ(z), world(world) 
{
    constexpr std::size_t EstimatedFaces = 4096;

    vertices.reserve(EstimatedFaces * 4 * 9);
    indices.reserve(EstimatedFaces * 6);
}
Chunk::~Chunk() {}
void Chunk::UpdateMesh()
{
    vertices.clear();
    indices.clear();
    for (unsigned i = 0; i < CHUNK_WIDTH * CHUNK_WIDTH * CHUNK_HEIGHT; i++)
    {
        if (blockIDs[i])
        {
            int x = i % CHUNK_WIDTH;
            int z = (i / CHUNK_WIDTH) % CHUNK_WIDTH;
            int y = i / (CHUNK_WIDTH * CHUNK_WIDTH);
            AddVertices(x, y, z, blockIDs[i]);
        }
    }
    isDirty = true;
}
int Chunk::GetBlock(glm::u8vec3 position)
{
    return blockIDs[(position.y * CHUNK_WIDTH + position.z) * CHUNK_WIDTH + position.x];
}
int Chunk::GetBlock(unsigned x, unsigned y, unsigned z)
{
    if (x > 15 || y > 255 || z > 15)
        return 0;
    return blockIDs[(y * CHUNK_WIDTH + z) * CHUNK_WIDTH + x];
}
void Chunk::SetBlock(glm::u8vec3 position, unsigned id)
{
    blockIDs[(position.y * CHUNK_WIDTH + position.z) * CHUNK_WIDTH + position.x] = id;
}
void Chunk::SetBlock(unsigned x, unsigned y, unsigned z, unsigned id)
{
    blockIDs[(y * CHUNK_WIDTH + z) * CHUNK_WIDTH + x] = id;
}
void Chunk::AddVertices(int x, int y, int z, float blockId)
{
    auto bakedModel = resourceManager.modelContainer.GetModel(ResourceLocation("block/" + blockRegister.GetBlock(blockId).GetName()));
    if(!bakedModel) return;
    const glm::i32vec3 position {x, y, z};
    for (const auto& quad : bakedModel->generalQuads)
    {
        EmitQuad(quad, position, blockId);
    }

    for (std::size_t i = 0;i < model::DirectionToIndex(model::Direction::Count);++i)
    {
        auto direction = static_cast<model::Direction>(i);
        if (!ShouldRenderFace(x, y, z, direction))
            continue;

        for (const auto& quad : bakedModel->faceQuads[i])
        {
            EmitQuad(quad, position, blockId);
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
    if (face == model::Direction::South && (BlockRegister::GetInstance().GetBlock(GetBlock(x, y, z + 1)).GetSolid() || z == 15 && world->GetChunk(chunkX, chunkZ + 1) && BlockRegister::GetInstance().GetBlock(world->GetChunk(chunkX, chunkZ + 1)->GetBlock(x, y, 0)).GetSolid()))
    {
        return false;
    }
    if (face == model::Direction::North && (BlockRegister::GetInstance().GetBlock(GetBlock(x, y, z - 1)).GetSolid() || z == 0 && world->GetChunk(chunkX, chunkZ - 1) && BlockRegister::GetInstance().GetBlock(world->GetChunk(chunkX, chunkZ - 1)->GetBlock(x, y, 15)).GetSolid()))
    {
        return false;
    }
    if (face == model::Direction::West && (BlockRegister::GetInstance().GetBlock(GetBlock(x - 1, y, z)).GetSolid() || x == 0 && world->GetChunk(chunkX - 1, chunkZ) && BlockRegister::GetInstance().GetBlock(world->GetChunk(chunkX - 1, chunkZ)->GetBlock(15, y, z)).GetSolid()))
    {
        return false;
    }
    if (face == model::Direction::East && (BlockRegister::GetInstance().GetBlock(GetBlock(x + 1, y, z)).GetSolid() || x == 15 && world->GetChunk(chunkX + 1, chunkZ) && BlockRegister::GetInstance().GetBlock(world->GetChunk(chunkX + 1, chunkZ)->GetBlock(0, y, z)).GetSolid()))
    {
        return false;
    }
    if (face == model::Direction::Up && BlockRegister::GetInstance().GetBlock(GetBlock(x, y + 1, z)).GetSolid())
    {
        return false;
    }
    if (face == model::Direction::Down && BlockRegister::GetInstance().GetBlock(GetBlock(x, y - 1, z)).GetSolid())
    {
        return false;
    }
    return true;
}
const std::vector<float>& Chunk::GetVertices()
{
    return vertices;
}
const std::vector<unsigned>& Chunk::GetIndices()
{
    return indices;
}