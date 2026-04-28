#ifndef WORLD_H
#define WORLD_H

#include "chunk.h"
#include <unordered_map>
class World
{
public:
    World(unsigned size);
    ~World();
    int GetBlock(int x,int y,int z);
    void SetBlock(int x, int y, int z, int id);
    void SetBlockAndRefresh(int x, int y, int z, int id);
    Chunk* GetChunk(int x,int z);
    void CreateChunk(int x,int z);
    void InitChunk();
private:
    unsigned worldSize;
    std::unordered_map<uint64_t,Chunk*> chunks;
};
#endif