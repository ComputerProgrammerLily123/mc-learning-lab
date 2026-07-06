#pragma once
#include <array>
#include <string>
#include <vector>

#define FACE_COUNT 6
enum class Face : unsigned char
{
    front,
    back,
    left,
    right,
    top,
    bottom
};
class BlockBase
{
public:
    [[nodiscard]] unsigned GetID() const;
    [[nodiscard]] const std::string GetName() const;
    [[nodiscard]] const std::array<int, FACE_COUNT> GetUVOffsets() const;
    [[nodiscard]] const std::vector<std::string> GetTextures() const;
    [[nodiscard]] bool GetSolid() const;

    void SetID(unsigned id);
    void SetName(const std::string& name);
    void SetUVOffsets(const std::array<int, FACE_COUNT>& uvOffsets);
    void SetTextures(const std::vector<std::string>& textures);
    void SetSolid(bool isSolid);

private:
    unsigned id;
    std::string name;
    std::array<int, FACE_COUNT> uvOffsets;
    std::vector<std::string> textures;
    bool isSolid;
};