#include "chunk.h"
#include <glad/glad.h>

#include "world.h"
Chunk::Chunk(int x, int z, World* world) : chunkX(x),chunkZ(z),world(world),blockIDs{0}
{
    InitMesh();
}
Chunk::~Chunk()
{
    if (VAO)
    {
        glDeleteVertexArrays(1, &VAO);
        glDeleteBuffers(1, &VBO);
        glDeleteBuffers(1, &EBO);
    }
}
void Chunk::InitMesh()
{
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glGenBuffers(1, &EBO);

    glBindVertexArray(VAO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(float), vertices.data(), GL_DYNAMIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned), indices.data(), GL_DYNAMIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 9 * sizeof(float), (void *)0);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 9 * sizeof(float), (void *)(3 * sizeof(float)));
    glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 9 * sizeof(float), (void *)(5 * sizeof(float)));
    glVertexAttribPointer(3, 1, GL_FLOAT, GL_FALSE, 9 * sizeof(float), (void *)(8 * sizeof(float)));
    glEnableVertexAttribArray(0);
    glEnableVertexAttribArray(1);
    glEnableVertexAttribArray(2);
    glEnableVertexAttribArray(3);

    glBindVertexArray(0);
}
void Chunk::UpdateMesh()
{
    vertices.clear();
    indices.clear();

    for (unsigned i = 0; i < CHUNK_WIDTH * CHUNK_WIDTH * CHUNK_HEIGHT; i++)
    {
        if (blockIDs[i])
        {
            int x = i % CHUNK_WIDTH;
            int z = (i / CHUNK_WIDTH) % CHUNK_WIDTH;
            int y = i / (CHUNK_WIDTH * CHUNK_WIDTH);
            AddVertices(x, y, z,blockRegister.GetBlockProperty(blockIDs[i]).uvOffset,blockIDs[i] == 1);
        }
    }
}
void Chunk::DrawMesh(){
    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(float), vertices.data(), GL_DYNAMIC_DRAW);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned), indices.data(), GL_DYNAMIC_DRAW);
    glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, 0);
    glBindVertexArray(0);
}
int Chunk::GetBlock(glm::u8vec3 position)
{

    return blockIDs[(position.y * CHUNK_WIDTH + position.z) * CHUNK_WIDTH + position.x];
}
int Chunk::GetBlock(unsigned x, unsigned y, unsigned z)
{
    if(x > 15 || y > 255 || z > 15) return 0;
    return blockIDs[(y * CHUNK_WIDTH + z) * CHUNK_WIDTH + x];
}
void Chunk::SetBlock(unsigned id, glm::u8vec3 position)
{
    blockIDs[(position.y * CHUNK_WIDTH + position.z) * CHUNK_WIDTH + position.x] = id;
}
void Chunk::SetBlock(unsigned id, unsigned x, unsigned y, unsigned z)
{
    blockIDs[(y * CHUNK_WIDTH + z) * CHUNK_WIDTH + x] = id;
}
void Chunk::AddVertices(int x, int y, int z,int* uvOffset,float isGrass)
{
    float fx = static_cast<float>(x + chunkX * 16);
    float fy = static_cast<float>(y);
    float fz = static_cast<float>(z + chunkZ * 16);
    float u1 = 0.0f, v1 = 0.0f;
    float u2 = 0.5f, v2 = 0.5f;
    float cellSize = 0.5f;
    unsigned startIndex;
    
    if (ShouldRenderFace(x, y, z, front))
    {
        startIndex = vertices.size() / 9;
        vertices.insert(vertices.end(), {
            fx - 0.5f, fy - 0.5f, fz + 0.5f, u1 + uvOffset[front] % 2 * cellSize, v1 + uvOffset[front] / 2 * cellSize,0,0,1,isGrass,
            fx + 0.5f, fy - 0.5f, fz + 0.5f, u2 + uvOffset[front] % 2 * cellSize, v1 + uvOffset[front] / 2 * cellSize,0,0,1,isGrass,
            fx + 0.5f, fy + 0.5f, fz + 0.5f, u2 + uvOffset[front] % 2 * cellSize, v2 + uvOffset[front] / 2 * cellSize,0,0,1,isGrass,
            fx - 0.5f, fy + 0.5f, fz + 0.5f, u1 + uvOffset[front] % 2 * cellSize, v2 + uvOffset[front] / 2 * cellSize,0,0,1,isGrass
        });
        indices.insert(indices.end(), {startIndex, startIndex + 1, startIndex + 2,
                                       startIndex, startIndex + 2, startIndex + 3});
    }
    
    if (ShouldRenderFace(x, y, z, back))
    {
        startIndex = vertices.size() / 9;
        vertices.insert(vertices.end(), {
            fx + 0.5f, fy - 0.5f, fz - 0.5f, u1 + uvOffset[back] % 2 * cellSize, v1 + uvOffset[back] / 2 * cellSize,0,0,-1,isGrass,
            fx - 0.5f, fy - 0.5f, fz - 0.5f, u2 + uvOffset[back] % 2 * cellSize, v1 + uvOffset[back] / 2 * cellSize,0,0,-1,isGrass,
            fx - 0.5f, fy + 0.5f, fz - 0.5f, u2 + uvOffset[back] % 2 * cellSize, v2 + uvOffset[back] / 2 * cellSize,0,0,-1,isGrass,
            fx + 0.5f, fy + 0.5f, fz - 0.5f, u1 + uvOffset[back] % 2 * cellSize, v2 + uvOffset[back] / 2 * cellSize,0,0,-1,isGrass
        });
        indices.insert(indices.end(), {startIndex, startIndex + 1, startIndex + 2,
                                       startIndex, startIndex + 2, startIndex + 3});
    }
    
    if (ShouldRenderFace(x, y, z, left))
    {
        startIndex = vertices.size() / 9;
        vertices.insert(vertices.end(), {
            fx - 0.5f, fy - 0.5f, fz - 0.5f, u1 + uvOffset[left] % 2 * cellSize, v1 + uvOffset[left] / 2 * cellSize,-1,0,0,isGrass,
            fx - 0.5f, fy - 0.5f, fz + 0.5f, u2 + uvOffset[left] % 2 * cellSize, v1 + uvOffset[left] / 2 * cellSize,-1,0,0,isGrass,
            fx - 0.5f, fy + 0.5f, fz + 0.5f, u2 + uvOffset[left] % 2 * cellSize, v2 + uvOffset[left] / 2 * cellSize,-1,0,0,isGrass,
            fx - 0.5f, fy + 0.5f, fz - 0.5f, u1 + uvOffset[left] % 2 * cellSize, v2 + uvOffset[left] / 2 * cellSize,-1,0,0,isGrass
        });
        indices.insert(indices.end(), {startIndex, startIndex + 1, startIndex + 2,
                                       startIndex, startIndex + 2, startIndex + 3});
    }
    
    if (ShouldRenderFace(x, y, z, right))
    {
        startIndex = vertices.size() / 9;
        vertices.insert(vertices.end(), {
            fx + 0.5f, fy - 0.5f, fz + 0.5f, u1 + uvOffset[right] % 2 * cellSize, v1 + uvOffset[right] / 2 * cellSize,1,0,0,isGrass,
            fx + 0.5f, fy - 0.5f, fz - 0.5f, u2 + uvOffset[right] % 2 * cellSize, v1 + uvOffset[right] / 2 * cellSize,1,0,0,isGrass,
            fx + 0.5f, fy + 0.5f, fz - 0.5f, u2 + uvOffset[right] % 2 * cellSize, v2 + uvOffset[right] / 2 * cellSize,1,0,0,isGrass,
            fx + 0.5f, fy + 0.5f, fz + 0.5f, u1 + uvOffset[right] % 2 * cellSize, v2 + uvOffset[right] / 2 * cellSize,1,0,0,isGrass
        });
        indices.insert(indices.end(), {startIndex, startIndex + 1, startIndex + 2,
                                       startIndex, startIndex + 2, startIndex + 3});
    }
    
    if (ShouldRenderFace(x, y, z, top))
    {
        startIndex = vertices.size() / 9;
        vertices.insert(vertices.end(), {
            fx - 0.5f, fy + 0.5f, fz - 0.5f, u1 + uvOffset[top] % 2 * cellSize, v1 + uvOffset[top] / 2 * cellSize,0,1,0,isGrass,
            fx - 0.5f, fy + 0.5f, fz + 0.5f, u1 + uvOffset[top] % 2 * cellSize, v2 + uvOffset[top] / 2 * cellSize,0,1,0,isGrass,
            fx + 0.5f, fy + 0.5f, fz + 0.5f, u2 + uvOffset[top] % 2 * cellSize, v2 + uvOffset[top] / 2 * cellSize,0,1,0,isGrass,
            fx + 0.5f, fy + 0.5f, fz - 0.5f, u2 + uvOffset[top] % 2 * cellSize, v1 + uvOffset[top] / 2 * cellSize,0,1,0,isGrass
        });
        indices.insert(indices.end(), {startIndex, startIndex + 1, startIndex + 2,
                                       startIndex, startIndex + 2, startIndex + 3});
    }
    
    if (ShouldRenderFace(x, y, z, bottom))
    {
        startIndex = vertices.size() / 9;
        vertices.insert(vertices.end(), {
            fx - 0.5f, fy - 0.5f, fz - 0.5f, u1 + uvOffset[bottom] % 2 * cellSize, v1 + uvOffset[bottom] / 2 * cellSize,0,-1,0,isGrass,
            fx + 0.5f, fy - 0.5f, fz - 0.5f, u2 + uvOffset[bottom] % 2 * cellSize, v1 + uvOffset[bottom] / 2 * cellSize,0,-1,0,isGrass,
            fx + 0.5f, fy - 0.5f, fz + 0.5f, u2 + uvOffset[bottom] % 2 * cellSize, v2 + uvOffset[bottom] / 2 * cellSize,0,-1,0,isGrass,
            fx - 0.5f, fy - 0.5f, fz + 0.5f, u1 + uvOffset[bottom] % 2 * cellSize, v2 + uvOffset[bottom] / 2 * cellSize,0,-1,0,isGrass
        });
        indices.insert(indices.end(), {startIndex, startIndex + 1, startIndex + 2,
                                       startIndex, startIndex + 2, startIndex + 3});
    }
}
bool Chunk::ShouldRenderFace(int x, int y, int z, Face face)
{
    if (face == front && (GetBlock(x, y, z + 1) || z == 15 && world->GetChunk(chunkX,chunkZ+1) && world->GetChunk(chunkX,chunkZ+1)->GetBlock(x,y,0)))
    {
        return false;
    }
    if (face == back && (GetBlock(x, y, z - 1) || z == 0 && world->GetChunk(chunkX,chunkZ-1) && world->GetChunk(chunkX,chunkZ-1)->GetBlock(x,y,15)))
    {
        return false;
    }
    if (face == left && (GetBlock(x - 1, y, z)|| x == 0 && world->GetChunk(chunkX-1,chunkZ) && world->GetChunk(chunkX-1,chunkZ)->GetBlock(15,y,z)))
    {
        return false;
    }
    if (face == right && (GetBlock(x + 1, y, z)|| x == 15 && world->GetChunk(chunkX+1,chunkZ) && world->GetChunk(chunkX+1,chunkZ)->GetBlock(0,y,z)))
    {
        return false;
    }
    if (face == top && GetBlock(x, y + 1, z))
    {
        return false;
    }
    if (face == bottom && GetBlock(x, y - 1, z))
    {
        return false;
    }
    return true;
}