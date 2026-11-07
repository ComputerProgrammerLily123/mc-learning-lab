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
    East
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
    Fixed
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
    std::unordered_map<Direction, ModelFace> faces;
    std::optional<ElementRotation> rotation;
    bool shade{true};
};
struct DisplayTransform
{
    glm::vec3 scale{1.0f};
    glm::vec3 rotation{0.0f};
    glm::vec3 translation{0.0f};
};
struct RawModel
{
    std::optional<std::string> parent;
    std::optional<bool> ambientOcclusion;
    std::optional<std::unordered_map<std::string, std::string>> textures;
    std::optional<std::vector<ModelElement>> elements;
    std::optional<DisplayTransform> display;
    std::optional<std::string> guiLight;
};
struct ResolvedModel
{
    std::optional<bool> ambientOcclusion;
    std::optional<std::unordered_map<std::string, std::string>> textures;
    std::optional<std::vector<ModelElement>> elements;
    std::optional<DisplayTransform> display;
    std::optional<std::string> guiLight;
};
inline std::string DirectionToString(const Direction& direction)
{
    std::array<std::string, 6> tags = {"down", "up","north","south","west","east"};
    return tags.at(static_cast<int>(direction));
}
inline Direction StringToDirection(const std::string& tag)
{
    if(tag == "down"){
        return Direction::Down;
    }
    if(tag == "up"){
        return Direction::Up;
    }
    if(tag == "north"){
        return Direction::North;
    }
    if(tag == "south"){
        return Direction::South;
    }
    if(tag == "west"){
        return Direction::West;
    }
    if(tag == "east"){
        return Direction::East;
    }
    return static_cast<Direction>(-1);
}
inline Axis ParseRotationAxis(const std::string& value)
{
    if (value == "x") return Axis::X;
    if (value == "y") return Axis::Y;
    if (value == "z") return Axis::Z;

    return static_cast<Axis>(-1);
}
}  // namespace model
