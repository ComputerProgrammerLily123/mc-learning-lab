#include "chunk.h"
#include <glad/glad.h>

#include "world.h"
Chunk::Chunk(int x, int z, World *world) : chunkX(x), chunkZ(z), world(world) {}
Chunk::~Chunk() {}
void Chunk::UpdateMesh()
{
    vertices.clear();
    indices.clear();
    auto& uvMap = resourceManager.GetAtlas(AtlasType::Block).GetUVMap();
    for (unsigned i = 0; i < CHUNK_WIDTH * CHUNK_WIDTH * CHUNK_HEIGHT; i++)
    {
        if (blockIDs[i])
        {
            int x = i % CHUNK_WIDTH;
            int z = (i / CHUNK_WIDTH) % CHUNK_WIDTH;
            int y = i / (CHUNK_WIDTH * CHUNK_WIDTH);
            //auto uv = uvMap.at(blockRegister.GetBlock(blockIDs[i]).GetName());
            
            AddVertices(x, y, z, blockRegister.GetBlock(blockIDs[i]).GetUVOffsets().data(), blockIDs[i]);
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
void Chunk::AddVertices(int x, int y, int z, const int *const uvOffset, float blockId)
{
    float fx = static_cast<float>(x + chunkX * 16);
    float fy = static_cast<float>(y);
    float fz = static_cast<float>(z + chunkZ * 16);
    float cellSize = 0.25f;
    float u1 = 0.0f, v1 = 0.0f;
    float u2 = cellSize, v2 = cellSize;
    unsigned cellCount = 4;
    unsigned startIndex;

    if (ShouldRenderFace(x, y, z, Face::front))
    {
        startIndex = vertices.size() / 9;
        vertices.insert(vertices.end(), {fx - 0.5f, fy - 0.5f, fz + 0.5f, u1 + uvOffset[static_cast<int>(Face::front)] % cellCount * cellSize, v1 + uvOffset[static_cast<int>(Face::front)] / cellCount * cellSize, 0, 0, 1, blockId,
                                         fx + 0.5f, fy - 0.5f, fz + 0.5f, u2 + uvOffset[static_cast<int>(Face::front)] % cellCount * cellSize, v1 + uvOffset[static_cast<int>(Face::front)] / cellCount * cellSize, 0, 0, 1, blockId,
                                         fx + 0.5f, fy + 0.5f, fz + 0.5f, u2 + uvOffset[static_cast<int>(Face::front)] % cellCount * cellSize, v2 + uvOffset[static_cast<int>(Face::front)] / cellCount * cellSize, 0, 0, 1, blockId,
                                         fx - 0.5f, fy + 0.5f, fz + 0.5f, u1 + uvOffset[static_cast<int>(Face::front)] % cellCount * cellSize, v2 + uvOffset[static_cast<int>(Face::front)] / cellCount * cellSize, 0, 0, 1, blockId});
        indices.insert(indices.end(), {startIndex, startIndex + 1, startIndex + 2,
                                       startIndex, startIndex + 2, startIndex + 3});
    }

    if (ShouldRenderFace(x, y, z, Face::back))
    {
        startIndex = vertices.size() / 9;
        vertices.insert(vertices.end(), {fx + 0.5f, fy - 0.5f, fz - 0.5f, u1 + uvOffset[static_cast<int>(Face::back)] % cellCount * cellSize, v1 + uvOffset[static_cast<int>(Face::back)] / cellCount * cellSize, 0, 0, -1, blockId,
                                         fx - 0.5f, fy - 0.5f, fz - 0.5f, u2 + uvOffset[static_cast<int>(Face::back)] % cellCount * cellSize, v1 + uvOffset[static_cast<int>(Face::back)] / cellCount * cellSize, 0, 0, -1, blockId,
                                         fx - 0.5f, fy + 0.5f, fz - 0.5f, u2 + uvOffset[static_cast<int>(Face::back)] % cellCount * cellSize, v2 + uvOffset[static_cast<int>(Face::back)] / cellCount * cellSize, 0, 0, -1, blockId,
                                         fx + 0.5f, fy + 0.5f, fz - 0.5f, u1 + uvOffset[static_cast<int>(Face::back)] % cellCount * cellSize, v2 + uvOffset[static_cast<int>(Face::back)] / cellCount * cellSize, 0, 0, -1, blockId});
        indices.insert(indices.end(), {startIndex, startIndex + 1, startIndex + 2,
                                       startIndex, startIndex + 2, startIndex + 3});
    }

    if (ShouldRenderFace(x, y, z, Face::left))
    {
        startIndex = vertices.size() / 9;
        vertices.insert(vertices.end(), {fx - 0.5f, fy - 0.5f, fz - 0.5f, u1 + uvOffset[static_cast<int>(Face::left)] % cellCount * cellSize, v1 + uvOffset[static_cast<int>(Face::left)] / cellCount * cellSize, -1, 0, 0, blockId,
                                         fx - 0.5f, fy - 0.5f, fz + 0.5f, u2 + uvOffset[static_cast<int>(Face::left)] % cellCount * cellSize, v1 + uvOffset[static_cast<int>(Face::left)] / cellCount * cellSize, -1, 0, 0, blockId,
                                         fx - 0.5f, fy + 0.5f, fz + 0.5f, u2 + uvOffset[static_cast<int>(Face::left)] % cellCount * cellSize, v2 + uvOffset[static_cast<int>(Face::left)] / cellCount * cellSize, -1, 0, 0, blockId,
                                         fx - 0.5f, fy + 0.5f, fz - 0.5f, u1 + uvOffset[static_cast<int>(Face::left)] % cellCount * cellSize, v2 + uvOffset[static_cast<int>(Face::left)] / cellCount * cellSize, -1, 0, 0, blockId});
        indices.insert(indices.end(), {startIndex, startIndex + 1, startIndex + 2,
                                       startIndex, startIndex + 2, startIndex + 3});
    }

    if (ShouldRenderFace(x, y, z, Face::right))
    {
        startIndex = vertices.size() / 9;
        vertices.insert(vertices.end(), {fx + 0.5f, fy - 0.5f, fz + 0.5f, u1 + uvOffset[static_cast<int>(Face::right)] % cellCount * cellSize, v1 + uvOffset[static_cast<int>(Face::right)] / cellCount * cellSize, 1, 0, 0, blockId,
                                         fx + 0.5f, fy - 0.5f, fz - 0.5f, u2 + uvOffset[static_cast<int>(Face::right)] % cellCount * cellSize, v1 + uvOffset[static_cast<int>(Face::right)] / cellCount * cellSize, 1, 0, 0, blockId,
                                         fx + 0.5f, fy + 0.5f, fz - 0.5f, u2 + uvOffset[static_cast<int>(Face::right)] % cellCount * cellSize, v2 + uvOffset[static_cast<int>(Face::right)] / cellCount * cellSize, 1, 0, 0, blockId,
                                         fx + 0.5f, fy + 0.5f, fz + 0.5f, u1 + uvOffset[static_cast<int>(Face::right)] % cellCount * cellSize, v2 + uvOffset[static_cast<int>(Face::right)] / cellCount * cellSize, 1, 0, 0, blockId});
        indices.insert(indices.end(), {startIndex, startIndex + 1, startIndex + 2,
                                       startIndex, startIndex + 2, startIndex + 3});
    }

    if (ShouldRenderFace(x, y, z, Face::top))
    {
        startIndex = vertices.size() / 9;
        vertices.insert(vertices.end(), {fx - 0.5f, fy + 0.5f, fz - 0.5f, u1 + uvOffset[static_cast<int>(Face::top)] % cellCount * cellSize, v1 + uvOffset[static_cast<int>(Face::top)] / cellCount * cellSize, 0, 1, 0, blockId,
                                         fx - 0.5f, fy + 0.5f, fz + 0.5f, u1 + uvOffset[static_cast<int>(Face::top)] % cellCount * cellSize, v2 + uvOffset[static_cast<int>(Face::top)] / cellCount * cellSize, 0, 1, 0, blockId,
                                         fx + 0.5f, fy + 0.5f, fz + 0.5f, u2 + uvOffset[static_cast<int>(Face::top)] % cellCount * cellSize, v2 + uvOffset[static_cast<int>(Face::top)] / cellCount * cellSize, 0, 1, 0, blockId,
                                         fx + 0.5f, fy + 0.5f, fz - 0.5f, u2 + uvOffset[static_cast<int>(Face::top)] % cellCount * cellSize, v1 + uvOffset[static_cast<int>(Face::top)] / cellCount * cellSize, 0, 1, 0, blockId});
        indices.insert(indices.end(), {startIndex, startIndex + 1, startIndex + 2,
                                       startIndex, startIndex + 2, startIndex + 3});
    }

    if (ShouldRenderFace(x, y, z, Face::bottom))
    {
        startIndex = vertices.size() / 9;
        vertices.insert(vertices.end(), {fx - 0.5f, fy - 0.5f, fz - 0.5f, u1 + uvOffset[static_cast<int>(Face::bottom)] % cellCount * cellSize, v1 + uvOffset[static_cast<int>(Face::bottom)] / cellCount * cellSize, 0, -1, 0, blockId,
                                         fx + 0.5f, fy - 0.5f, fz - 0.5f, u2 + uvOffset[static_cast<int>(Face::bottom)] % cellCount * cellSize, v1 + uvOffset[static_cast<int>(Face::bottom)] / cellCount * cellSize, 0, -1, 0, blockId,
                                         fx + 0.5f, fy - 0.5f, fz + 0.5f, u2 + uvOffset[static_cast<int>(Face::bottom)] % cellCount * cellSize, v2 + uvOffset[static_cast<int>(Face::bottom)] / cellCount * cellSize, 0, -1, 0, blockId,
                                         fx - 0.5f, fy - 0.5f, fz + 0.5f, u1 + uvOffset[static_cast<int>(Face::bottom)] % cellCount * cellSize, v2 + uvOffset[static_cast<int>(Face::bottom)] / cellCount * cellSize, 0, -1, 0, blockId});
        indices.insert(indices.end(), {startIndex, startIndex + 1, startIndex + 2,
                                       startIndex, startIndex + 2, startIndex + 3});
    }
}
bool Chunk::ShouldRenderFace(int x, int y, int z, Face face)
{
    if (face == Face::front && (BlockRegister::GetInstance().GetBlock(GetBlock(x, y, z + 1)).GetSolid() || z == 15 && world->GetChunk(chunkX, chunkZ + 1) && BlockRegister::GetInstance().GetBlock(world->GetChunk(chunkX, chunkZ + 1)->GetBlock(x, y, 0)).GetSolid()))
    {
        return false;
    }
    if (face == Face::back && (BlockRegister::GetInstance().GetBlock(GetBlock(x, y, z - 1)).GetSolid() || z == 0 && world->GetChunk(chunkX, chunkZ - 1) && BlockRegister::GetInstance().GetBlock(world->GetChunk(chunkX, chunkZ - 1)->GetBlock(x, y, 15)).GetSolid()))
    {
        return false;
    }
    if (face == Face::left && (BlockRegister::GetInstance().GetBlock(GetBlock(x - 1, y, z)).GetSolid() || x == 0 && world->GetChunk(chunkX - 1, chunkZ) && BlockRegister::GetInstance().GetBlock(world->GetChunk(chunkX - 1, chunkZ)->GetBlock(15, y, z)).GetSolid()))
    {
        return false;
    }
    if (face == Face::right && (BlockRegister::GetInstance().GetBlock(GetBlock(x + 1, y, z)).GetSolid() || x == 15 && world->GetChunk(chunkX + 1, chunkZ) && BlockRegister::GetInstance().GetBlock(world->GetChunk(chunkX + 1, chunkZ)->GetBlock(0, y, z)).GetSolid()))
    {
        return false;
    }
    if (face == Face::top && BlockRegister::GetInstance().GetBlock(GetBlock(x, y + 1, z)).GetSolid())
    {
        return false;
    }
    if (face == Face::bottom && BlockRegister::GetInstance().GetBlock(GetBlock(x, y - 1, z)).GetSolid())
    {
        return false;
    }
    return true;
}
const std::vector<float> &Chunk::GetVertices()
{
    return vertices;
}
const std::vector<unsigned> &Chunk::GetIndices()
{
    return indices;
}