#include "world.h"

#include <fastNoiseLite/FastNoiseLite.h>
World::World(unsigned size) : worldSize(size)
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
        GetChunk(chunkX, chunkZ)->SetBlock(id, localX, y, localZ);
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
        GetChunk(chunkX, chunkZ)->SetBlock(id, localX, y, localZ);
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
void World::InitChunk()
{
    FastNoiseLite noise;
    noise.SetNoiseType(FastNoiseLite::NoiseType_Perlin);
    noise.SetFrequency(0.05f);
    for (unsigned i = 0; i < worldSize; i++)
    {
        for (unsigned j = 0; j < worldSize; j++)
        {
            for (unsigned x = 0; x < CHUNK_WIDTH; x++)
            {
                for (unsigned z = 0; z < CHUNK_WIDTH; z++)
                {
                    if (GetChunk(i, j))
                    {
                        unsigned height = ((noise.GetNoise((float)(x + i * 16), (float)(z + j * 16)) + 1) / 2) * 10 + 10;
                        for (unsigned y = 0; y < height - 3; y++)
                        {
                            GetChunk(i, j)->SetBlock(3, x, y, z);
                        }
                        for (unsigned y = height - 3; y < height; y++)
                        {
                            GetChunk(i, j)->SetBlock(2, x, y, z);
                        }
                        GetChunk(i, j)->SetBlock(1, x, height, z);
                    }
                }
            }
        }
    }
    for (unsigned i = 0; i < worldSize; i++)
    {
        for (unsigned j = 0; j < worldSize; j++)
        {
            if (GetChunk(i, j))
                GetChunk(i, j)->UpdateMesh();
        }
    }
}