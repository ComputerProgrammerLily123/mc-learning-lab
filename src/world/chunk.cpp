#include "chunk.h"

#include <glad/glad.h>

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
    auto fx = static_cast<float>(x + chunkX * CHUNK_WIDTH);
    auto fy = static_cast<float>(y);
    auto fz = static_cast<float>(z + chunkZ * CHUNK_WIDTH);
    unsigned startIndex = 0;
    auto& faces = resourceManager.modelContainer.GetModel(ResourceLocation("block/" + blockRegister.GetBlock(blockId).GetName()))->elements->front().faces;
    if (ShouldRenderFace(x, y, z, "front"))
    {
        startIndex = vertices.size() / 9;
        const auto& textureRegion = resourceManager.GetTextureRegion(ResourceLocation(faces["south"].texture));
        if(textureRegion != nullptr)
        {
            const auto& u1 = textureRegion->GetU1();
            const auto& v1 = textureRegion->GetV1();
            const auto& u2 = textureRegion->GetU2();
            const auto& v2 = textureRegion->GetV2();
            vertices.insert(vertices.end(), {fx - 0.5f, fy - 0.5f, fz + 0.5f, u1, v1, 0, 0, 1, blockId,
                                         fx + 0.5f, fy - 0.5f, fz + 0.5f, u2, v1, 0, 0, 1, blockId,
                                         fx + 0.5f, fy + 0.5f, fz + 0.5f, u2, v2, 0, 0, 1, blockId,
                                         fx - 0.5f, fy + 0.5f, fz + 0.5f, u1, v2, 0, 0, 1, blockId});
            indices.insert(indices.end(), {startIndex, startIndex + 1, startIndex + 2,
                                       startIndex, startIndex + 2, startIndex + 3});
        }
    }

    if (ShouldRenderFace(x, y, z, "back"))
    {
        startIndex = vertices.size() / 9;
        const auto& textureRegion = resourceManager.GetTextureRegion(ResourceLocation(faces["north"].texture));
        if(textureRegion != nullptr)
        {
            const auto& u1 = textureRegion->GetU1();
            const auto& v1 = textureRegion->GetV1();
            const auto& u2 = textureRegion->GetU2();
            const auto& v2 = textureRegion->GetV2();
            vertices.insert(vertices.end(), {fx + 0.5f, fy - 0.5f, fz - 0.5f, u1, v1, 0, 0, -1, blockId,
                                         fx - 0.5f, fy - 0.5f, fz - 0.5f, u2, v1, 0, 0, -1, blockId,
                                         fx - 0.5f, fy + 0.5f, fz - 0.5f, u2, v2, 0, 0, -1, blockId,
                                         fx + 0.5f, fy + 0.5f, fz - 0.5f, u1, v2, 0, 0, -1, blockId});
            indices.insert(indices.end(), {startIndex, startIndex + 1, startIndex + 2,
                                       startIndex, startIndex + 2, startIndex + 3});
        }
    }

    if (ShouldRenderFace(x, y, z, "left"))
    {
        startIndex = vertices.size() / 9;
        const auto& textureRegion = resourceManager.GetTextureRegion(ResourceLocation(faces["west"].texture));
        if(textureRegion != nullptr)
        {
            const auto& u1 = textureRegion->GetU1();
            const auto& v1 = textureRegion->GetV1();
            const auto& u2 = textureRegion->GetU2();
            const auto& v2 = textureRegion->GetV2();
            vertices.insert(vertices.end(), {fx - 0.5f, fy - 0.5f, fz - 0.5f, u1, v1, -1, 0, 0, blockId,
                                         fx - 0.5f, fy - 0.5f, fz + 0.5f, u2, v1, -1, 0, 0, blockId,
                                         fx - 0.5f, fy + 0.5f, fz + 0.5f, u2, v2, -1, 0, 0, blockId,
                                         fx - 0.5f, fy + 0.5f, fz - 0.5f, u1, v2, -1, 0, 0, blockId});
            indices.insert(indices.end(), {startIndex, startIndex + 1, startIndex + 2,
                                       startIndex, startIndex + 2, startIndex + 3});
        }
    }

    if (ShouldRenderFace(x, y, z, "right"))
    {
        startIndex = vertices.size() / 9;
        const auto& textureRegion = resourceManager.GetTextureRegion(ResourceLocation(faces["east"].texture));
        if(textureRegion != nullptr)
        {
            const auto& u1 = textureRegion->GetU1();
            const auto& v1 = textureRegion->GetV1();
            const auto& u2 = textureRegion->GetU2();
            const auto& v2 = textureRegion->GetV2();
            vertices.insert(vertices.end(), {fx + 0.5f, fy - 0.5f, fz + 0.5f, u1, v1, 1, 0, 0, blockId,
                                         fx + 0.5f, fy - 0.5f, fz - 0.5f, u2, v1, 1, 0, 0, blockId,
                                         fx + 0.5f, fy + 0.5f, fz - 0.5f, u2, v2, 1, 0, 0, blockId,
                                         fx + 0.5f, fy + 0.5f, fz + 0.5f, u1, v2, 1, 0, 0, blockId});
            indices.insert(indices.end(), {startIndex, startIndex + 1, startIndex + 2,
                                       startIndex, startIndex + 2, startIndex + 3});
        }
    }

    if (ShouldRenderFace(x, y, z, "top"))
    {
        startIndex = vertices.size() / 9;
        const auto& textureRegion = resourceManager.GetTextureRegion(ResourceLocation(faces["up"].texture));
        if(textureRegion != nullptr)
        {
            const auto& u1 = textureRegion->GetU1();
            const auto& v1 = textureRegion->GetV1();
            const auto& u2 = textureRegion->GetU2();
            const auto& v2 = textureRegion->GetV2();
            vertices.insert(vertices.end(), {fx - 0.5f, fy + 0.5f, fz - 0.5f, u1, v1, 0, 1, 0, blockId,
                                         fx - 0.5f, fy + 0.5f, fz + 0.5f, u1, v2, 0, 1, 0, blockId,
                                         fx + 0.5f, fy + 0.5f, fz + 0.5f, u2, v2, 0, 1, 0, blockId,
                                         fx + 0.5f, fy + 0.5f, fz - 0.5f, u2, v1, 0, 1, 0, blockId});
            indices.insert(indices.end(), {startIndex, startIndex + 1, startIndex + 2,
                                       startIndex, startIndex + 2, startIndex + 3});
            }
    }

    if (ShouldRenderFace(x, y, z, "bottom"))
    {
        startIndex = vertices.size() / 9;
        const auto& textureRegion = resourceManager.GetTextureRegion(ResourceLocation(faces["down"].texture));
        if(textureRegion != nullptr)
        {
            const auto& u1 = textureRegion->GetU1();
            const auto& v1 = textureRegion->GetV1();
            const auto& u2 = textureRegion->GetU2();
            const auto& v2 = textureRegion->GetV2();
            vertices.insert(vertices.end(), {fx - 0.5f, fy - 0.5f, fz - 0.5f, u1, v1, 0, -1, 0, blockId,
                                         fx + 0.5f, fy - 0.5f, fz - 0.5f, u2, v1, 0, -1, 0, blockId,
                                         fx + 0.5f, fy - 0.5f, fz + 0.5f, u2, v2, 0, -1, 0, blockId,
                                         fx - 0.5f, fy - 0.5f, fz + 0.5f, u1, v2, 0, -1, 0, blockId});
            indices.insert(indices.end(), {startIndex, startIndex + 1, startIndex + 2,
                                       startIndex, startIndex + 2, startIndex + 3});
        }
    }
}
bool Chunk::ShouldRenderFace(int x, int y, int z, const std::string& face)
{
    if (face == "front" && (BlockRegister::GetInstance().GetBlock(GetBlock(x, y, z + 1)).GetSolid() || z == 15 && world->GetChunk(chunkX, chunkZ + 1) && BlockRegister::GetInstance().GetBlock(world->GetChunk(chunkX, chunkZ + 1)->GetBlock(x, y, 0)).GetSolid()))
    {
        return false;
    }
    if (face == "back" && (BlockRegister::GetInstance().GetBlock(GetBlock(x, y, z - 1)).GetSolid() || z == 0 && world->GetChunk(chunkX, chunkZ - 1) && BlockRegister::GetInstance().GetBlock(world->GetChunk(chunkX, chunkZ - 1)->GetBlock(x, y, 15)).GetSolid()))
    {
        return false;
    }
    if (face == "left" && (BlockRegister::GetInstance().GetBlock(GetBlock(x - 1, y, z)).GetSolid() || x == 0 && world->GetChunk(chunkX - 1, chunkZ) && BlockRegister::GetInstance().GetBlock(world->GetChunk(chunkX - 1, chunkZ)->GetBlock(15, y, z)).GetSolid()))
    {
        return false;
    }
    if (face == "right" && (BlockRegister::GetInstance().GetBlock(GetBlock(x + 1, y, z)).GetSolid() || x == 15 && world->GetChunk(chunkX + 1, chunkZ) && BlockRegister::GetInstance().GetBlock(world->GetChunk(chunkX + 1, chunkZ)->GetBlock(0, y, z)).GetSolid()))
    {
        return false;
    }
    if (face == "top" && BlockRegister::GetInstance().GetBlock(GetBlock(x, y + 1, z)).GetSolid())
    {
        return false;
    }
    if (face == "bottom" && BlockRegister::GetInstance().GetBlock(GetBlock(x, y - 1, z)).GetSolid())
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