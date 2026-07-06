#include "worldGenerator.h"
#include "world.h"
WorldGenerator::WorldGenerator(World *world) : world(world)
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
    for (int x = 0; x < CHUNK_WIDTH; x++)
    {
        for (int z = 0; z < CHUNK_WIDTH; z++)
        {
            if (world->GetChunk(i, j))
            {
                int32_t blockX = x + i * 16;
                int32_t blockZ = z + j * 16;
                unsigned height = ((surfaceNoise.GetNoise(static_cast<float>(blockX), static_cast<float>(blockZ)) + 1) / 2) * 64 + ((surfaceNoise.GetNoise(static_cast<float>(blockX * 4), static_cast<float>(blockZ * 4)) + 1) / 2) * 32 + ((surfaceNoise.GetNoise(static_cast<float>(blockX * 16), static_cast<float>(blockZ * 16)) + 1) / 2) * 8 + 10;
                for (unsigned y = 0; y < height - 3; y++)
                {
                    world->SetBlock(blockX, y, blockZ,3);
                }
                for (unsigned y = height - 3; y < height; y++)
                {
                    world->SetBlock(blockX, y, blockZ,2);
                }
                world->SetBlock(blockX, height, blockZ,1);
            }
        }
    }
}
void WorldGenerator::TreeGenerate(int32_t i, int32_t j)
{
    for (int x = 2; x < CHUNK_WIDTH - 2; x++)
    {
        for (int z = 2; z < CHUNK_WIDTH - 2; z++)
        {
            int32_t blockX = x + i * 16;
            int32_t blockZ = z + j * 16;
            unsigned flag = ((treeNoise.GetNoise(static_cast<float>(blockX), static_cast<float>(blockZ)) + 1) / 2) * 32;
            if (flag < 30)
                continue;
            unsigned surfaceHeight = 0;
            if (world->GetChunk(i, j))
            {
                for (int y = 0; y < CHUNK_HEIGHT; y++)
                {
                    if (!world->GetBlock(blockX, y, blockZ))
                    {
                        surfaceHeight = y;
                        break;
                    }
                }
                if (!CheckTreeSpace(blockX, surfaceHeight, blockZ))
                    continue;
                world->SetBlock(blockX, surfaceHeight, blockZ, 4);
                world->SetBlock(blockX, surfaceHeight + 1, blockZ, 4);
                world->SetBlock(blockX, surfaceHeight + 2, blockZ, 4);
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
                world->SetBlock(blockX - 1, surfaceHeight + 5, blockZ, 10);
                world->SetBlock(blockX + 1, surfaceHeight + 5, blockZ, 10);
                world->SetBlock(blockX, surfaceHeight + 5, blockZ - 1, 10);
                world->SetBlock(blockX, surfaceHeight + 5, blockZ + 1, 10);
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
                if (world->GetBlock(i, m, j))
                    return false;
            }
        }
    }
    return true;
}