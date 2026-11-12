#pragma once

#include <glm/glm.hpp>
#include <optional>
#include <string>
#include <vector>

namespace model
{
//components
struct Face
{
    std::string texture;
    std::optional<glm::vec4> uv;
    std::optional<int> rotation {0};
    std::optional<int> tintIndex {-1};
    std::optional<std::string> cullFace;
};
struct SingleRotation
{
    glm::vec3 origin;
    std::optional<bool> rescale {false};
    float angle;
    std::string axis;
};
struct MultiRotation
{
    glm::vec3 origin;
    std::optional<bool> rescale {false};
    float x, y, z;
};
struct Rotation
{
    std::optional<SingleRotation> singleRotation;
    std::optional<MultiRotation> multiRotation;
};
struct Element
{
    glm::vec3 from;
    glm::vec3 to;
    std::optional<Rotation> rotation;
    std::optional<bool> shade {true};
    std::optional<int> light_emission;
    std::unordered_map<std::string, Face> faces;
};
struct DisplayMode
{
    std::optional<glm::vec3> scale {{1.0f, 1.0f, 1.0f}};
    std::optional<glm::vec3> rotation {{0.0f, 0.0f, 0.0f}};
    std::optional<glm::vec3> translation {{0.0f, 0.0f, 0.0f}};
};
struct Display
{
    std::optional<DisplayMode> gui;
    std::optional<DisplayMode> ground;
    std::optional<DisplayMode> fixed;
    std::optional<DisplayMode> thirdperson_righthand;
    std::optional<DisplayMode> firstperson_righthand;
    std::optional<DisplayMode> firstperson_lefthand;
};

//models
struct Model
{
    std::optional<std::string> parent;
    std::optional<bool> ambientOcclusion;
    std::optional<std::unordered_map<std::string, std::string>> textures;
    std::optional<std::vector<Element>> elements;
    std::optional<Display> display;
    std::optional<std::string> gui_light;
};

//converters
/* inline std::string DirectionToString(const Direction& direction)
{
    static const std::array<std::string, 6> tags = {"down", "up","north","south","west","east"};
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
} */
}
