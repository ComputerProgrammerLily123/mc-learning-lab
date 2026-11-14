#include "world.h"

#include "resource/blockstate/blockstateRegistry.h"

World::World() : worldGen(this)
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
blockstate::BlockStateId World::GetBlockStateId(int x, int y, int z)
{
    if (y < 0 || y >= CHUNK_HEIGHT) return 0;
    int chunkX = WorldToChunk(x);
    int chunkZ = WorldToChunk(z);
    unsigned localX = WorldToLocal(x);
    unsigned localZ = WorldToLocal(z);
    if (GetChunk(chunkX, chunkZ))
    {
        return GetChunk(chunkX, chunkZ)->GetBlock(localX, y, localZ);
    }
    else
    {
        return 0;
    }
}
void World::SetBlock(int x, int y, int z, int blockId)
{
    const auto stateId = blockstate::BlockStateRegistry::GetInstance().GetDefaultState(blockId);
    SetBlockState(x, y, z, stateId);
}
void World::SetBlockAndRefresh(int x, int y, int z, int id)
{
    const auto stateId = blockstate::BlockStateRegistry::GetInstance().GetDefaultState(id);
    SetBlockStateAndRefresh(x, y, z, stateId);
}
void World::SetBlockState(int x, int y, int z, blockstate::BlockStateId stateId)
{
    if (y < 0 || y >= CHUNK_HEIGHT)
    return;
    int chunkX = WorldToChunk(x);
    int chunkZ = WorldToChunk(z);
    unsigned localX = WorldToLocal(x);
    unsigned localZ = WorldToLocal(z);
    if (Chunk* chunk = GetChunk(chunkX, chunkZ))
        chunk->SetBlock(localX, y, localZ, stateId);
}
void World::SetBlockStateAndRefresh(int x, int y, int z, blockstate::BlockStateId stateId)
{
    if (y < 0 || y >= CHUNK_HEIGHT)
        return;

    const int chunkX = WorldToChunk(x);
    const int chunkZ = WorldToChunk(z);

    if (!GetChunk(chunkX, chunkZ))
        return;

    SetBlockState(x, y, z, stateId);
    UpdateFenceConnectionsAround(x, y, z);
    RefreshChunkAndNeighbors(chunkX, chunkZ);
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
    UpdateFenceStatesInsideChunk(i, j);
    UpdateFenceConnectionsAfterChunkLoad(i, j);
    RefreshChunkAndNeighbors(i, j);
}

void World::RefreshChunkAndNeighbors(int chunkX, int chunkZ)
{
    static constexpr glm::ivec2 offsets[] =
    {
        { 0,  0},
        {-1,  0},
        { 1,  0},
        { 0, -1},
        { 0,  1}
    };

    for (const auto& offset : offsets)
    {
        if (Chunk* chunk = GetChunk(chunkX + offset.x, chunkZ + offset.y))
        {
            chunk->UpdateMesh();
        }
    }
}

bool World::CanFenceConnect(int x, int y, int z)
{
    if (y < 0 || y >= CHUNK_HEIGHT) return false;

    const auto stateId = GetBlockStateId(x, y, z);

    const auto& state = blockstate::BlockStateRegistry::GetInstance().Get(stateId);

    const BlockBase block = BlockRegister::GetInstance().GetBlock(state.blockId);

    return block.GetName() == "fence" || block.GetSolid();
}

bool World::UpdateFenceState(int x, int y, int z)
{
    if (y < 0 || y >= CHUNK_HEIGHT)
        return false;

    const auto currentStateId = GetBlockStateId(x, y, z);

    auto& registry = blockstate::BlockStateRegistry::GetInstance();

    const auto& currentState = registry.Get(currentStateId);

    const BlockBase currentBlock = BlockRegister::GetInstance().GetBlock(currentState.blockId);

    if (currentBlock.GetName() != "fence")
        return false;

    std::unordered_map<std::string, std::string> properties = {
        {
            "north",
            CanFenceConnect(x, y, z - 1) ? "true" : "false"
        },
        {
            "east",
            CanFenceConnect(x + 1, y, z) ? "true" : "false"
        },
        {
            "south",
            CanFenceConnect(x, y, z + 1) ? "true" : "false"
        },
        {
            "west",
            CanFenceConnect(x - 1, y, z) ? "true" : "false"
        }
    };

    const auto targetStateId = registry.Find(currentState.blockId, properties);

    if (!targetStateId)
    {
        throw std::runtime_error("Missing registered fence block state.");
    }

    if (*targetStateId == currentStateId)
        return false;

    SetBlockState(x, y, z, *targetStateId);

    return true;
}

void World::UpdateFenceConnectionsAround(int x, int y, int z)
{
    static constexpr glm::ivec2 offsets[] = {
        { 0,  0},
        { 0, -1},
        { 1,  0},
        { 0,  1},
        {-1,  0}
    };

    for (const auto& offset : offsets)
    {
        UpdateFenceState(x + offset.x, y, z + offset.y);
    }
}
void World::UpdateFenceConnectionsAfterChunkLoad(int chunkX, int chunkZ)
{
    const int baseX = chunkX * CHUNK_WIDTH;
    const int baseZ = chunkZ * CHUNK_WIDTH;

    const int minX = baseX;
    const int maxX = baseX + CHUNK_WIDTH - 1;
    const int minZ = baseZ;
    const int maxZ = baseZ + CHUNK_WIDTH - 1;

    for (int y = 0; y < CHUNK_HEIGHT; ++y)
    {
        for (int offset = 0;offset < CHUNK_WIDTH;++offset)
        {
            UpdateFenceState(baseX + offset, y, minZ);
            UpdateFenceState(baseX + offset, y, minZ - 1);
            UpdateFenceState(baseX + offset, y, maxZ);
            UpdateFenceState(baseX + offset, y, maxZ + 1);
            UpdateFenceState(minX, y, baseZ + offset);
            UpdateFenceState(minX - 1, y, baseZ + offset);
            UpdateFenceState(maxX, y, baseZ + offset);
            UpdateFenceState(maxX + 1, y, baseZ + offset);
        }
    }
}
void World::UpdateFenceStatesInsideChunk(int chunkX, int chunkZ)
{
    const int baseX = chunkX * CHUNK_WIDTH;
    const int baseZ = chunkZ * CHUNK_WIDTH;

    for (int y = 0; y < CHUNK_HEIGHT; ++y)
    {
        for (int localZ = 0;localZ < CHUNK_WIDTH;++localZ)
        {
            for (int localX = 0;localX < CHUNK_WIDTH;++localX)
            {
                UpdateFenceState(baseX + localX, y, baseZ + localZ);
            }
        }
    }
}