#pragma once

#include <unordered_map>

#include "chunk.h"
#include "core/sceneManager.h"
#include "worldGenerator.h"

class World : public SceneObject
{
public:
    World();
    World(const World&) = delete;
    World(World&&) = delete;
    World& operator=(const World&) = delete;
    World& operator=(World&&) = delete;
    ~World();
    int GetBlock(int x, int y, int z);
    void SetBlock(int x, int y, int z, int id);
    void SetBlockAndRefresh(int x, int y, int z, int id);
    Chunk* GetChunk(int x, int z);
    void CreateChunk(int x, int z);
    void InitChunk(int32_t i, int32_t j);

private:
    std::unordered_map<uint64_t, Chunk*> chunks;
    FastNoiseLite noise1, noise2;
    WorldGenerator worldGen;
};