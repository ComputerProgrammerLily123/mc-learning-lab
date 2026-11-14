#include "blockstateBaker.h"
#include <stdexcept>

#include <glm/gtc/matrix_transform.hpp>
namespace blockstate 
{
BakedBlockStateDefinition BlockstateBaker::Bake(const BlockStateDefinition& definition, const ModelContainer& models)
{
    BakedBlockStateDefinition result;

    for (const auto& entry : definition.variants)
    {
        BakedVariantEntry bakedEntry;
        bakedEntry.conditions = entry.conditions;
        bakedEntry.models = BakeModels(entry.models, models);
        result.variants.push_back(std::move(bakedEntry));
    }
    for (const auto& entry : definition.multipart)
    {
        BakedMultipartEntry bakedEntry;
        bakedEntry.condition = entry.condition;
        bakedEntry.models = BakeModels(entry.models, models);
        result.multipart.push_back(std::move(bakedEntry));
    }
    return result;
}
std::vector<BakedModelVariant> BlockstateBaker::BakeModels(const std::vector<ModelVariant>& variants, const ModelContainer& models)
{
    std::vector<BakedModelVariant> bakedModels;
    for (const auto& variant : variants)
    {
        auto baseModel = models.GetModel(variant.model);

        if (!baseModel) throw std::runtime_error("Missing baked model.");

        std::shared_ptr<const model::BakedModel> transformed;

        if (variant.xRotation == 0 && variant.yRotation == 0)
        {
            transformed = baseModel;
        }
        else
        {
            transformed = std::make_shared<model::BakedModel>(RotateModel(*baseModel, variant.xRotation, variant.yRotation, variant.uvLock));
        }
        bakedModels.push_back({transformed, variant.weight});
    }
    return bakedModels;
}
model::BakedModel BlockstateBaker::RotateModel(const model::BakedModel& source, int xRotation, int yRotation, bool uvLock)
{
    const glm::mat4 xMatrix = glm::rotate(glm::mat4(1.0f),glm::radians(-static_cast<float>(xRotation)),glm::vec3(1.0f, 0.0f, 0.0f));
    const glm::mat4 yMatrix = glm::rotate(glm::mat4(1.0f),glm::radians(-static_cast<float>(yRotation)),glm::vec3(0.0f, 1.0f, 0.0f));
    const glm::mat4 rotation = yMatrix * xMatrix;
    model::BakedModel result;
    result.ambientOcclusion = source.ambientOcclusion;
    result.display = source.display;

    auto addQuad = [&](model::BakedQuad quad)
    {
        RotateQuad(quad, rotation, uvLock);

        if (quad.cullFace)
        {
            result.faceQuads[
                model::DirectionToIndex(*quad.cullFace)
            ].push_back(std::move(quad));
        }
        else
        {
            result.generalQuads.push_back(std::move(quad));
        }
    };
    for (const auto& quad : source.generalQuads)
    addQuad(quad);

    for (const auto& quads : source.faceQuads)
        for (const auto& quad : quads)
            addQuad(quad);
    return result;
}

void BlockstateBaker::RotateQuad(model::BakedQuad& quad, const glm::mat4& rotation, bool uvLock)
{
    const model::Direction sourceDirection = quad.direction;

    for (auto& vertex : quad.vertices)
    {
        vertex.position = glm::vec3(rotation * glm::vec4(vertex.position, 1.0f));
        vertex.normal = glm::normalize(glm::vec3(rotation * glm::vec4(vertex.normal, 0.0f)));
    }

    const model::Direction targetDirection = RotateDirection(sourceDirection, rotation);

    if (uvLock)
    {
        const std::size_t steps = GetUvLockRotation(sourceDirection, targetDirection, rotation);
        RotateUv(quad, steps);
    }

    quad.direction = targetDirection;

    if (quad.cullFace)
    {
        quad.cullFace = RotateDirection(*quad.cullFace, rotation);
    }
}

model::Direction BlockstateBaker::RotateDirection(model::Direction direction, const glm::mat4& rotation)
{
    glm::vec3 vector{0.0f};

    switch (direction)
    {
    case model::Direction::Down:  vector = { 0, -1,  0}; break;
    case model::Direction::Up:    vector = { 0,  1,  0}; break;
    case model::Direction::North: vector = { 0,  0, -1}; break;
    case model::Direction::South: vector = { 0,  0,  1}; break;
    case model::Direction::West:  vector = {-1,  0,  0}; break;
    case model::Direction::East:  vector = { 1,  0,  0}; break;
    case model::Direction::Count:
        throw std::runtime_error("Cannot rotate Direction::Count.");
    }

    const glm::vec3 rotated = glm::vec3(rotation * glm::vec4(vector, 0.0f));

    return model::VectorToDirection(rotated);
}

FaceBasis BlockstateBaker::GetFaceBasis(model::Direction direction)
{
    switch (direction)
    {
    case model::Direction::South:
        return {{ 1, 0, 0}, {0, 1,  0}};

    case model::Direction::North:
        return {{-1, 0, 0}, {0, 1,  0}};

    case model::Direction::West:
        return {{ 0, 0, 1}, {0, 1,  0}};

    case model::Direction::East:
        return {{ 0, 0,-1}, {0, 1,  0}};

    case model::Direction::Up:
        return {{ 1, 0, 0}, {0, 0, -1}};

    case model::Direction::Down:
        return {{ 1, 0, 0}, {0, 0,  1}};

    case model::Direction::Count:
        throw std::runtime_error("Direction::Count has no UV basis.");
    }

    throw std::runtime_error("Unknown direction.");
}

std::size_t BlockstateBaker::GetUvLockRotation(model::Direction sourceDirection, model::Direction targetDirection, const glm::mat4& rotation)
{
    const FaceBasis source = GetFaceBasis(sourceDirection);
    const FaceBasis target = GetFaceBasis(targetDirection);

    const glm::vec3 rotatedU = glm::normalize(glm::vec3(rotation * glm::vec4(source.u, 0.0f)));
    if (glm::dot(rotatedU, target.u) > 0.5f)
        return 0;

    if (glm::dot(rotatedU, target.v) > 0.5f)
        return 1;

    if (glm::dot(rotatedU, -target.u) > 0.5f)
        return 2;

    if (glm::dot(rotatedU, -target.v) > 0.5f)
        return 3;

    throw std::runtime_error("Unable to calculate uvlock rotation.");
}

void BlockstateBaker::RotateUv(model::BakedQuad& quad, std::size_t steps)
{
    if (steps == 0) return;

    std::array<glm::vec2, 4> original{};

    for (std::size_t i = 0; i < 4; ++i)
        original[i] = quad.vertices[i].uv;

    for (std::size_t i = 0; i < 4; ++i)
    {
        quad.vertices[i].uv = original[(i + steps) % 4];
    }
}

}