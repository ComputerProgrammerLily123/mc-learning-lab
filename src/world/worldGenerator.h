#pragma once
#include <fastNoiseLite/FastNoiseLite.h>
#include <cstdint>
class World;
class WorldGenerator
{
public:
    WorldGenerator(World *world);
    ~WorldGenerator();
    void GenerateChunk(int32_t i, int32_t j);

private:
    World *world;
    FastNoiseLite surfaceNoise, treeNoise;

    void NoiseGenerate(int32_t i, int32_t j);
    void TreeGenerate(int32_t i, int32_t j);
    bool CheckTreeSpace(int32_t x, int32_t y, int32_t z);
};