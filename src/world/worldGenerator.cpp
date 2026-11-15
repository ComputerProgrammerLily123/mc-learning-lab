#include "worldGenerator.h"

#include "resource/blockstate/blockstateRegistry.h"
#include "world.h"


WorldGenerator::WorldGenerator(World* world) : world(world)
{
    surfaceNoise.SetNoiseType(FastNoiseLite::NoiseType_OpenSimplex2S);
    surfaceNoise.SetFrequency(0.001f);
    surfaceNoise.SetSeed(0);
    treeNoise.SetFrequency(0.1f);
    treeNoise.SetSeed(1000);
}
WorldGenerator::~WorldGenerator()
{
}
void WorldGenerator::GenerateChunk(int32_t i, int32_t j)
{
    NoiseGenerate(i, j);
    TreeGenerate(i, j);
}
void WorldGenerator::NoiseGenerate(int32_t i, int32_t j)
{
    auto grassId = blockstate::BlockStateRegistry::GetInstance().GetDefaultState(1);
    auto dirtId = blockstate::BlockStateRegistry::GetInstance().GetDefaultState(2);
    auto stoneId = blockstate::BlockStateRegistry::GetInstance().GetDefaultState(3);
    if (auto chunk = world->GetChunk(i, j))
    {
        for (int x = 0; x < CHUNK_WIDTH; x++)
        {
            for (int z = 0; z < CHUNK_WIDTH; z++)
            {
                int32_t blockX = x + i * CHUNK_WIDTH;
                int32_t blockZ = z + j * CHUNK_WIDTH;
                unsigned height = ((surfaceNoise.GetNoise(static_cast<float>(blockX), static_cast<float>(blockZ)) + 1) / 2) * 64 + ((surfaceNoise.GetNoise(static_cast<float>(blockX * 4), static_cast<float>(blockZ * 4)) + 1) / 2) * 32 + ((surfaceNoise.GetNoise(static_cast<float>(blockX * 16), static_cast<float>(blockZ * 16)) + 1) / 2) * 8 + 10;
                height_map.at(x * CHUNK_WIDTH + z) = height;
                for (unsigned y = 0; y < height - 3; y++)
                {
                    chunk->SetBlock(x, y, z, stoneId);
                }
                for (unsigned y = height - 3; y < height; y++)
                {
                    chunk->SetBlock(x, y, z, dirtId);
                }
                chunk->SetBlock(x, height, z, grassId);
            }
        }
    }
}
void WorldGenerator::TreeGenerate(int32_t i, int32_t j)
{
    auto oakLogId = blockstate::BlockStateRegistry::GetInstance().GetDefaultState(4);
    auto leaveId = blockstate::BlockStateRegistry::GetInstance().GetDefaultState(10);
    if (auto chunk = world->GetChunk(i, j))
    {
        for (int x = 2; x < CHUNK_WIDTH - 2; x++)
        {
            for (int z = 2; z < CHUNK_WIDTH - 2; z++)
            {
                int32_t blockX = x + i * CHUNK_WIDTH;
                int32_t blockZ = z + j * CHUNK_WIDTH;
                unsigned flag = ((treeNoise.GetNoise(static_cast<float>(blockX), static_cast<float>(blockZ)) + 1) / 2) * 32;
                if (flag < 30)
                    continue;
                unsigned surfaceHeight = height_map.at(x * CHUNK_WIDTH + z) + 1;
                if (!CheckTreeSpace(blockX, surfaceHeight, blockZ))
                    continue;

                chunk->SetBlock(x, surfaceHeight, z, oakLogId);
                chunk->SetBlock(x, surfaceHeight + 1, z, oakLogId);
                chunk->SetBlock(x, surfaceHeight + 2, z, oakLogId);
                for (int targetY = surfaceHeight + 2; targetY < surfaceHeight + 4; targetY++)
                {
                    for (int targetX = blockX - 2; targetX <= blockX + 2; targetX++)
                    {
                        for (int targetZ = blockZ - 2; targetZ <= blockZ + 2; targetZ++)
                        {
                            if (targetX == blockX && targetZ == blockZ)
                            {
                                world->SetBlock(targetX, targetY, targetZ, 4);
                            }
                            else
                            {
                                world->SetBlock(targetX, targetY, targetZ, 10);
                            }
                        }
                    }
                }
                for (int targetX = blockX - 1; targetX <= blockX + 1; targetX++)
                {
                    for (int targetZ = blockZ - 1; targetZ <= blockZ + 1; targetZ++)
                    {
                        world->SetBlock(targetX, surfaceHeight + 4, targetZ, 10);
                    }
                }
                chunk->SetBlock(x - 1, surfaceHeight + 5, z, leaveId);
                chunk->SetBlock(x + 1, surfaceHeight + 5, z, leaveId);
                chunk->SetBlock(x, surfaceHeight + 5, z - 1, leaveId);
                chunk->SetBlock(x, surfaceHeight + 5, z + 1, leaveId);
            }
        }
    }
}
bool WorldGenerator::CheckTreeSpace(int32_t x, int32_t y, int32_t z)
{
    for (int32_t m = y; m < y + 4; m++)
    {
        for (int32_t i = x - 2; i <= x + 2; i++)
        {
            for (int32_t j = z - 2; j <= z + 2; j++)
            {
                if (world->GetBlockStateId(i, m, j))
                    return false;
            }
        }
    }
    return true;
}