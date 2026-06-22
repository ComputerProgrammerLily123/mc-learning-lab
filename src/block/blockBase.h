#pragma once
#include <string>
#include <array>
#include <vector>
enum Face
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
    BlockBase();
    ~BlockBase();
    unsigned GetID() const;
    const std::string GetName() const;
    const std::array<int,6> GetUVOffsets() const;
    const std::vector<std::string> GetTextures() const;
    bool GetSolid() const;

    void SetID(unsigned id);
    void SetName(const std::string &name);
    void SetUVOffsets(const std::array<int,6>& uvOffsets);
    void SetTextures(const std::vector<std::string>& textures);
    void SetSolid(bool isSolid);

private:
    unsigned id;
    std::string name;
    std::array<int, 6> uvOffsets;
    std::vector<std::string> textures;
    bool isSolid;
};