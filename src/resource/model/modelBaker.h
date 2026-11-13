#pragma once

#include "bakedModel.h"

class ModelBaker
{
public:
    ModelBaker() = delete;
    ~ModelBaker() = delete;
    ModelBaker(const ModelBaker&) = delete;
    ModelBaker(ModelBaker&&) = delete;
    ModelBaker& operator=(const ModelBaker&) = delete;
    ModelBaker& operator=(ModelBaker&&) = delete;

    static model::BakedModel Bake(const model::Model& model);
    static glm::vec4 GetFaceUV(const model::Element& element, model::Direction direction, const model::Face& face);
    static std::array<glm::vec2, 4> BakeFaceUV(const model::Element& element, model::Direction direction, const model::Face& face, const TextureRegion& region
);

private:
    static void BakeElement(const model::Element& element, model::BakedModel& result);
    static model::BakedQuad BakeFace(const model::Element& element, model::Direction direction, const model::Face& face);
    static void ApplyElementRotation(std::array<model::BakedVertex, 4>& vertices, const model::SingleRotation& rotation);
};