#pragma once

#include <array>
#include <optional>
#include <memory>
#include <vector>

#include <glm/glm.hpp>

#include "resource/textureRegion.h"
#include "modelDefinition.h"
#include "direction.h"

namespace model
{
struct BakedVertex
{
    glm::vec3 position;
    glm::vec2 uv;
    glm::vec3 normal;
};

struct BakedQuad
{
    std::array<BakedVertex, 4> vertices {};
    Direction direction {};
    std::optional<Direction> cullFace;
    int tintIndex = -1;
    bool shade = true;
};

struct BakedModel
{
    std::vector<BakedQuad> generalQuads;
    std::array<std::vector<BakedQuad>, static_cast<std::size_t>(Direction::Count)> faceQuads;
    bool ambientOcclusion = true;
    std::optional<Display> display;
};

struct BakedItemModel
{
    std::shared_ptr<const model::BakedModel> model;
    std::string guiLight = "side";
};
}