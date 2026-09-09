#pragma once

#include <cstdint>
#include <glm/glm.hpp>
#include <optional>
#include <string>
#include <vector>

namespace model
{
enum class Direction : std::uint8_t
{
    Down,
    Up,
    North,
    South,
    West,
    East,
    Count
};

enum class Axis : std::uint8_t
{
    X,
    Y,
    Z
};

enum class DisplayContext : std::uint8_t
{
    ThirdPersonRightHand,
    ThirdPersonLeftHand,
    FirstPersonRightHand,
    FirstPersonLeftHand,
    Head,
    Gui,
    Ground,
    Fixed,
    Count
};
struct ModelFace
{
    std::string texture;
    std::optional<glm::vec4> uv;
    std::optional<int> rotation{0};
    std::optional<int> tintIndex{-1};
    std::optional<std::string> cullFace;
};
struct ElementRotation
{
    glm::vec3 origin{};
    Axis axis = Axis::Y;
    float angle = 0.0f;
    bool rescale = false;
};
struct ModelElement
{
    glm::vec3 from{};
    glm::vec3 to{};
    std::optional<ElementRotation> rotation;
    bool shade{true};
    ModelFace faces;
};
struct DisplayTransform
{
    glm::vec3 scale{1.0f};
    glm::vec3 rotation{0.0f};
    glm::vec3 translation{0.0f};
};
struct Texture
{
};
struct RawModel
{
    std::optional<std::string> parent;
    std::optional<bool> ambientOcclusion;
    std::optional<Texture> textures;
    std::optional<std::vector<ModelElement>> elements;
    std::optional<DisplayTransform> display;
    std::optional<std::string> guiLight;
};
}  // namespace model
