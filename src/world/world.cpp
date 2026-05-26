#include "world.h"

World::World(unsigned size) : renderRadian(size), worldGen(this)
{
}
World::~World()
{
    for (auto &pair : chunks)
    {
        delete pair.second;
    }
    chunks.clear();
}
int World::GetBlock(int x, int y, int z)
{
    int chunkX = x >= 0 ? x / 16 : (x - 15) / 16;
    int chunkZ = z >= 0 ? z / 16 : (z - 15) / 16;
    unsigned localX = x - chunkX * 16;
    unsigned localZ = z - chunkZ * 16;
    if (GetChunk(chunkX, chunkZ))
    {
        return GetChunk(chunkX, chunkZ)->GetBlock(localX, y, localZ);
    }
    else
    {
        return 0;
    }
}
void World::SetBlock(int x, int y, int z, int id)
{
    int chunkX = x >= 0 ? x / 16 : (x - 15) / 16;
    int chunkZ = z >= 0 ? z / 16 : (z - 15) / 16;
    unsigned localX = x - chunkX * 16;
    unsigned localZ = z - chunkZ * 16;
    if (GetChunk(chunkX, chunkZ))
    {
        GetChunk(chunkX, chunkZ)->SetBlock(localX, y, localZ,id);
    }
}
void World::SetBlockAndRefresh(int x, int y, int z, int id)
{
    int chunkX = x >= 0 ? x / 16 : (x - 15) / 16;
    int chunkZ = z >= 0 ? z / 16 : (z - 15) / 16;
    unsigned localX = x - chunkX * 16;
    unsigned localZ = z - chunkZ * 16;
    if (GetChunk(chunkX, chunkZ))
    {
        GetChunk(chunkX, chunkZ)->SetBlock(localX, y, localZ,id);
        GetChunk(chunkX, chunkZ)->UpdateMesh();
        if (localX == 15 && GetChunk(chunkX + 1, chunkZ))
        {
            GetChunk(chunkX + 1, chunkZ)->UpdateMesh();
        }
        if (localX == 0 && GetChunk(chunkX - 1, chunkZ))
        {
            GetChunk(chunkX - 1, chunkZ)->UpdateMesh();
        }
        if (localZ == 15 && GetChunk(chunkX, chunkZ + 1))
        {
            GetChunk(chunkX, chunkZ + 1)->UpdateMesh();
        }
        if (localZ == 0 && GetChunk(chunkX, chunkZ - 1))
        {
            GetChunk(chunkX, chunkZ - 1)->UpdateMesh();
        }
    }
}
Chunk *World::GetChunk(int x, int z)
{
    uint64_t key = ((uint64_t)x << 32) | (uint32_t)z;
    auto it = chunks.find(key);
    if (it != chunks.end())
    {
        return it->second;
    }
    return nullptr;
}
void World::CreateChunk(int x, int z)
{
    uint64_t key = ((uint64_t)x << 32) | (uint32_t)z;
    if (chunks.find(key) != chunks.end())
        return;
    chunks[key] = new Chunk(x, z, this);
}
void World::InitChunk(int32_t i, int32_t j)
{
    worldGen.GenerateChunk(i, j);
}