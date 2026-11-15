#pragma once

#include <glm/glm.hpp>

#include <vector>
#include <array>

#include "block/blockRegister.h"
#include "resource/resourceManager.h"
#include "resource/blockstate/blockstateDefinition.h"

#define CHUNK_WIDTH 16
#define CHUNK_HEIGHT 256
class World;
class Chunk{
public:
    Chunk(const Chunk&) = default;
    Chunk(Chunk&&) = delete;
    Chunk& operator=(const Chunk&) = delete;
    Chunk& operator=(Chunk&&) = delete;
    Chunk(int x, int z, World* world);
    ~Chunk();
    bool isDirty = true;
    bool meshQueued = false;
    void UpdateMesh();
    int GetBlock(glm::u8vec3 position);
    int GetBlock(unsigned x,unsigned y,unsigned z);
    void SetBlock(glm::u8vec3 position, blockstate::BlockStateId stateId);
    void SetBlock(unsigned x,unsigned y,unsigned z, blockstate::BlockStateId stateId);
    const std::vector<float>& GetVertices();
    const std::vector<unsigned>& GetIndices();
private:
    int chunkX,chunkZ;
    std::array<blockstate::BlockStateId, CHUNK_WIDTH * CHUNK_WIDTH * CHUNK_HEIGHT> blockstateIDs {};
    std::vector<float> vertices;
    std::vector<unsigned> indices;
    std::vector<const model::BakedModel*> selectedModels;
    BlockRegister& blockRegister = BlockRegister::GetInstance();
    ResourceManager& resourceManager = ResourceManager::GetInstance();
    World* world;
    void AddVertices(int x,int y,int z, blockstate::BlockStateId blockId);
    void EmitQuad(const model::BakedQuad& quad, const glm::i32vec3 blockPosition, int stateId);
    bool ShouldRenderFace(int x,int y,int z, model::Direction face);
};