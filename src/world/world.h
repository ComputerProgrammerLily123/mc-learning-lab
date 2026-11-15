#pragma once

#include <unordered_map>
#include <deque>

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
    blockstate::BlockStateId GetBlockStateId(int x, int y, int z);
    void SetBlock(int x, int y, int z, int blockId);
    void SetBlockAndRefresh(int x, int y, int z, int id);
    void SetBlockState(int x, int y, int z, blockstate::BlockStateId stateId);
    void SetBlockStateAndRefresh(int x, int y, int z, blockstate::BlockStateId stateId);
    Chunk* GetChunk(int x, int z);
    void CreateChunk(int x, int z);
    void InitChunk(int32_t i, int32_t j);
    void RefreshChunkAndNeighbors(int chunkX, int chunkZ);

    void QueueChunkMeshUpdate(int chunkX, int chunkZ);
    void QueueBlockMeshUpdate(int x, int z);
    void ProcessMeshUpdates(std::size_t maxUpdates = 1);

    static constexpr int WorldToChunk(int coordinate)
    {
        return coordinate >= 0 ? coordinate / CHUNK_WIDTH : (coordinate - CHUNK_WIDTH + 1) / CHUNK_WIDTH;
    }

    static constexpr int WorldToLocal(int coordinate)
    {
        return coordinate - WorldToChunk(coordinate) * CHUNK_WIDTH;
    }

private:
    std::unordered_map<uint64_t, Chunk*> chunks;
    FastNoiseLite noise1, noise2;
    WorldGenerator worldGen;

    std::deque<Chunk*> meshUpdateQueue;

    bool CanFenceConnect(int x, int y, int z);
    bool UpdateFenceState(int x, int y, int z);
    void UpdateFenceConnectionsAround(int x, int y, int z);
    void UpdateFenceConnectionsAfterChunkLoad(int chunkX, int chunkZ);
    void UpdateFenceStatesInsideChunk(int chunkX, int chunkZ);
};