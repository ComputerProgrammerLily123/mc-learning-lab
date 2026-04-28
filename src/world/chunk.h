#ifndef CHUNK_H
#define CHUNK_H

#include <glm/glm.hpp>

#include "block.h"
#include "renderer.h"
#include <vector>

#define CHUNK_WIDTH 16
#define CHUNK_HEIGHT 256

class Chunk{
public:
    Chunk(int x,int z,World* world);
    ~Chunk();
    void UpdateMesh();
    void DrawMesh();
    int GetBlock(glm::u8vec3 position);
    int GetBlock(unsigned x,unsigned y,unsigned z);
    void SetBlock(unsigned id,glm::u8vec3 position);
    void SetBlock(unsigned id,unsigned x,unsigned y,unsigned z);
private:
    int chunkX,chunkZ;
    unsigned blockIDs[CHUNK_WIDTH * CHUNK_WIDTH * CHUNK_HEIGHT];
    unsigned VAO,VBO,EBO;
    std::vector<float> vertices;
    std::vector<unsigned> indices;
    BlockRegister& blockRegister = BlockRegister::GetInstance();
    World* world;
    void InitMesh();
    void AddVertices(int x,int y,int z,int* uvOffset,float isGrass);
    bool ShouldRenderFace(int x,int y,int z, Face face);
};
#endif