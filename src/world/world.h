#pragma once

#include "chunk.h"
#include "worldGenerator.h"
#include <unordered_map>
class World
{
public:
    World();
    ~World();
    int GetBlock(int x,int y,int z);
    void SetBlock(int x, int y, int z, int id);
    void SetBlockAndRefresh(int x, int y, int z, int id);
    Chunk* GetChunk(int x,int z);
    void CreateChunk(int x,int z);
    void InitChunk(int32_t i, int32_t j);
private:
    std::unordered_map<uint64_t,Chunk*> chunks;
    FastNoiseLite noise1,noise2;
    WorldGenerator worldGen;
};