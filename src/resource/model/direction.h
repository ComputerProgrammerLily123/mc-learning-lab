#pragma once

#include <string>
#include <optional>

#include <glm/glm.hpp>

namespace model 
{
enum class Direction : unsigned char
{
    Down,
    Up,
    North,
    South,
    West,
    East,
    Count
};
constexpr std::size_t DirectionToIndex(Direction direction)
{
    return static_cast<std::size_t>(direction);
}
inline std::optional<Direction> StringToDirection(const std::string& directionName)
{
    if(directionName == "north")
    {
        return Direction::North;
    }
    if(directionName == "south")
    {
        return Direction::South;
    }
    if(directionName == "west")
    {
        return Direction::West;
    }
    if(directionName == "east")
    {
        return Direction::East;
    }
    if(directionName == "up")
    {
        return Direction::Up;
    }
    if(directionName == "down")
    {
        return Direction::Down;
    }
    return std::nullopt;
}
inline Direction VectorToDirection(const glm::vec3& value)
{
    const glm::vec3 absolute = glm::abs(value);

    if (absolute.x >= absolute.y && absolute.x >= absolute.z)
    {
        return value.x >= 0 ? Direction::East : Direction::West;
    }

    if (absolute.y >= absolute.z)
    {
        return value.y >= 0 ? Direction::Up : Direction::Down;
    }

    return value.z >= 0 ? Direction::South : Direction::North;
}
}