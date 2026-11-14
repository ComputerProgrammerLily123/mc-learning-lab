#pragma once
#include "blockstateDefinition.h"
#include "resource/model/modelContainer.h"
namespace blockstate 
{
struct FaceBasis
{
    glm::vec3 u;
    glm::vec3 v;
};
class BlockstateBaker
{
public:
    BlockstateBaker() = delete;
    ~BlockstateBaker() = delete;
    BlockstateBaker(const BlockstateBaker&) = delete;
    BlockstateBaker(BlockstateBaker&&) = delete;
    BlockstateBaker& operator=(const BlockstateBaker&) = delete;
    BlockstateBaker& operator=(BlockstateBaker&&) = delete;
    static blockstate::BakedBlockStateDefinition Bake(const blockstate::BlockStateDefinition& definition, const ModelContainer& models);

private:

    static std::vector<BakedModelVariant> BakeModels(const std::vector<ModelVariant>& variants, const ModelContainer& models);
    
    static model::BakedModel RotateModel(const model::BakedModel& source, int xRotation, int yRotation, bool uvLock);

    static void RotateQuad(model::BakedQuad& quad, const glm::mat4& rotation, bool uvLock);

    static model::Direction RotateDirection(model::Direction direction, const glm::mat4& rotation);

    static FaceBasis GetFaceBasis(model::Direction direction);

    static std::size_t GetUvLockRotation(model::Direction sourceDirection, model::Direction targetDirection, const glm::mat4& rotation);

    static void RotateUv(model::BakedQuad& quad, std::size_t steps);
};
}