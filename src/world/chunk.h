#pragma once

#include <glm/glm.hpp>

#include "block.h"
#include <vector>

#define CHUNK_WIDTH 16
#define CHUNK_HEIGHT 256
class World;
class Chunk{
public:
    Chunk(int x,int z,World* world);
    ~Chunk();
    bool isDirty = true;
    void UpdateMesh();
    int GetBlock(glm::u8vec3 position);
    int GetBlock(unsigned x,unsigned y,unsigned z);
    void SetBlock(glm::u8vec3 position,unsigned id);
    void SetBlock(unsigned x,unsigned y,unsigned z,unsigned id);
    const std::vector<float>& GetVertices();
    const std::vector<unsigned>& GetIndices();
private:
    int chunkX,chunkZ;
    unsigned blockIDs[CHUNK_WIDTH * CHUNK_WIDTH * CHUNK_HEIGHT] = {0};
    std::vector<float> vertices;
    std::vector<unsigned> indices;
    BlockRegister& blockRegister = BlockRegister::GetInstance();
    World* world;
    void AddVertices(int x,int y,int z,const int* const uvOffset, float blockId);
    bool ShouldRenderFace(int x,int y,int z, Face face);
};