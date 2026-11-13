#include "modelBaker.h"

#include <glm/gtc/matrix_transform.hpp>
#include "resource/resourceManager.h"

model::BakedModel ModelBaker::Bake(const model::Model &model)
{
    model::BakedModel result;
    result.ambientOcclusion = model.ambientOcclusion.value_or(true);
    result.display = model.display;
    if (!model.elements) return result;
    for (const auto& element : *model.elements)
    {
        BakeElement(element, result);
    }
    return result;
}

glm::vec4 ModelBaker::GetFaceUV(const model::Element& element, model::Direction direction, const model::Face& face)
{
    if (face.uv)
    {
        return *face.uv;
    }

    const glm::vec3& from = element.from;
    const glm::vec3& to = element.to;

    switch (direction)
    {
    case model::Direction::Down:
        return {from.x, 16.0f - to.z,
                to.x,   16.0f - from.z};

    case model::Direction::Up:
        return {from.x, from.z,
                to.x,   to.z};

    case model::Direction::North:
        return {16.0f - to.x,   16.0f - to.y,
                16.0f - from.x, 16.0f - from.y};

    case model::Direction::South:
        return {from.x, 16.0f - to.y,
                to.x,   16.0f - from.y};

    case model::Direction::West:
        return {from.z, 16.0f - to.y,
                to.z,   16.0f - from.y};

    case model::Direction::East:
        return {16.0f - to.z,   16.0f - to.y,
                16.0f - from.z, 16.0f - from.y};

    case model::Direction::Count:
        throw std::runtime_error("Cannot calculate UV for Direction::Count.");
    }

    throw std::runtime_error("Unknown face direction.");
}
std::array<glm::vec2, 4> ModelBaker::BakeFaceUV(const model::Element& element, model::Direction direction, const model::Face& face, const TextureRegion& region)
{
    const glm::vec4 uv = GetFaceUV(element, direction, face);

    const float regionWidth = region.GetU2() - region.GetU1();
    const float regionHeight = region.GetV2() - region.GetV1();

    const auto mapU = [&](float value)
    {
        return region.GetU1() + regionWidth * (value / 16.0f);
    };

    const auto mapV = [&](float value)
    {
        return region.GetV2() - regionHeight * (value / 16.0f);
    };

    const glm::vec2 topLeft     {mapU(uv.x), mapV(uv.y)};
    const glm::vec2 bottomLeft  {mapU(uv.x), mapV(uv.w)};
    const glm::vec2 bottomRight {mapU(uv.z), mapV(uv.w)};
    const glm::vec2 topRight    {mapU(uv.z), mapV(uv.y)};

    std::array<glm::vec2, 4> result {};

    if (direction == model::Direction::Up)
    {
        result = {topLeft, bottomLeft, bottomRight, topRight};
    }
    else
    {
        result = {bottomLeft, bottomRight, topRight, topLeft};
    }

    const std::size_t rotationSteps = static_cast<std::size_t>(face.rotation / 90);

    if (rotationSteps != 0)
    {
        const auto original = result;

        for (std::size_t i = 0; i < result.size(); ++i)
        {
            result[i] = original[(i + rotationSteps) % original.size()];
        }
    }

    return result;
}
void ModelBaker::BakeElement(const model::Element& element, model::BakedModel& result)
{
    for (std::size_t i = 0; i < model::DirectionToIndex(model::Direction::Count); ++i)
    {
        const auto& face = element.faces.at(i);
        if (!face) continue;
        const auto direction = static_cast<model::Direction>(i);
        model::BakedQuad quad = BakeFace(element, direction, *face);
        if (quad.cullFace)
        {
            result.faceQuads.at(model::DirectionToIndex(*quad.cullFace)).push_back(std::move(quad));
        }
        else
        {
            result.generalQuads.push_back(quad);
        }
    }
}
model::BakedQuad ModelBaker::BakeFace(const model::Element& element, model::Direction direction, const model::Face& face)
{
    model::BakedQuad result;

    result.direction = direction;
    result.cullFace = face.cullFace;
    result.tintIndex = face.tintIndex;
    result.shade = element.shade;

    const glm::vec3 min = element.from / 16.0f - glm::vec3(0.5f);

    const glm::vec3 max = element.to / 16.0f - glm::vec3(0.5f);

    const auto textureRegion = ResourceManager::GetInstance().GetTextureRegion(ResourceLocation(face.texture));

    if (!textureRegion)
    {
        throw std::runtime_error("Texture region not found: " + face.texture);
    }

    using Vertex = model::BakedVertex;
    using VertexArray = std::array<Vertex, 4>;

    switch (direction)
    {
    case model::Direction::South:
        result.vertices = VertexArray{{
            {{min.x, min.y, max.z}, {}, { 0,  0,  1}},
            {{max.x, min.y, max.z}, {}, { 0,  0,  1}},
            {{max.x, max.y, max.z}, {}, { 0,  0,  1}},
            {{min.x, max.y, max.z}, {}, { 0,  0,  1}}
        }};
        break;

    case model::Direction::North:
        result.vertices = VertexArray{{
            {{max.x, min.y, min.z}, {}, { 0,  0, -1}},
            {{min.x, min.y, min.z}, {}, { 0,  0, -1}},
            {{min.x, max.y, min.z}, {}, { 0,  0, -1}},
            {{max.x, max.y, min.z}, {}, { 0,  0, -1}}
        }};
        break;

    case model::Direction::West:
        result.vertices = VertexArray{{
            {{min.x, min.y, min.z}, {}, {-1,  0,  0}},
            {{min.x, min.y, max.z}, {}, {-1,  0,  0}},
            {{min.x, max.y, max.z}, {}, {-1,  0,  0}},
            {{min.x, max.y, min.z}, {}, {-1,  0,  0}}
        }};
        break;

    case model::Direction::East:
        result.vertices = VertexArray{{
            {{max.x, min.y, max.z}, {}, { 1,  0,  0}},
            {{max.x, min.y, min.z}, {}, { 1,  0,  0}},
            {{max.x, max.y, min.z}, {}, { 1,  0,  0}},
            {{max.x, max.y, max.z}, {}, { 1,  0,  0}}
        }};
        break;

    case model::Direction::Up:
        result.vertices = VertexArray{{
            {{min.x, max.y, min.z}, {}, { 0,  1,  0}},
            {{min.x, max.y, max.z}, {}, { 0,  1,  0}},
            {{max.x, max.y, max.z}, {}, { 0,  1,  0}},
            {{max.x, max.y, min.z}, {}, { 0,  1,  0}}
        }};
        break;

    case model::Direction::Down:
        result.vertices = VertexArray{{
            {{min.x, min.y, min.z}, {}, { 0, -1,  0}},
            {{max.x, min.y, min.z}, {}, { 0, -1,  0}},
            {{max.x, min.y, max.z}, {}, { 0, -1,  0}},
            {{min.x, min.y, max.z}, {}, { 0, -1,  0}}
        }};
        break;

    case model::Direction::Count:
        throw std::runtime_error("Direction::Count cannot be baked.");
    }
    const auto bakedUVs = BakeFaceUV(element, direction, face, *textureRegion);

    for (std::size_t i = 0; i < result.vertices.size(); ++i)
    {
        result.vertices[i].uv = bakedUVs[i];
    }
    if (element.rotation && element.rotation->singleRotation)
    {
        ApplyElementRotation(result.vertices, *element.rotation->singleRotation);
    }
    return result;
}
void ModelBaker::ApplyElementRotation(std::array<model::BakedVertex, 4>& vertices, const model::SingleRotation& rotation)
{
    glm::vec3 origin = rotation.origin / 16.0f - glm::vec3(0.5f);
    const auto& axisName = rotation.axis;
    glm::vec3 axis;
    glm::vec3 rescaleFactor(1.0f);
    if (rotation.axis == "x")
    {
        axis = {1.0f, 0.0f, 0.0f};
    }
    else if (rotation.axis == "y")
    {
        axis = {0.0f, 1.0f, 0.0f};
    }
    else if (rotation.axis == "z")
    {
        axis = {0.0f, 0.0f, 1.0f};
    }
    else
    {
        throw std::runtime_error("Invalid element rotation axis: " + rotation.axis);
    }
    if (rotation.rescale && rotation.angle != 0.0f)
    {
        const float scale = 1.0f / std::cos(glm::radians(std::abs(rotation.angle)));

        if (rotation.axis == "x")
            rescaleFactor = {1.0f, scale, scale};
        else if (rotation.axis == "y")
            rescaleFactor = {scale, 1.0f, scale};
        else
            rescaleFactor = {scale, scale, 1.0f};
    }
    const glm::mat4 rotationMatrix = glm::rotate(glm::mat4(1.0f), glm::radians(rotation.angle), axis);
    for (auto& vertex : vertices) 
    {
        glm::vec3 localPosition = vertex.position - origin;
        localPosition *= rescaleFactor;
        vertex.position = origin + glm::vec3(rotationMatrix * glm::vec4(localPosition, 1.0f));
    }
    glm::vec3 normal = glm::normalize(glm::cross(vertices[1].position - vertices[0].position, vertices[2].position - vertices[0].position));
    for (auto& vertex : vertices)
    {
        vertex.normal = normal;
    }
}