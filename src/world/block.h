#ifndef BLOCK_H
#define BLOCK_H

#include <string>
#include <vector>
#include <array>
#include <unordered_map>
enum Face
{
    front,
    back,
    left,
    right,
    top,
    bottom
};
class Block
{
public:
    Block();
    ~Block();
    unsigned GetID() const;
    std::string GetName() const;
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
class BlockRegister
{
public:
    static BlockRegister &GetInstance()
    {
        static BlockRegister instance;
        return instance;
    }
    BlockRegister(const BlockRegister &) = delete;
    BlockRegister &operator=(const BlockRegister &) = delete;
    void RegisterBlock(const std::string &name, const std::array<int,6>& uvOffsets, const std::vector<std::string>& textures, bool solid);
    Block GetBlock(int id);
    unsigned GetBlockCount() const;
private:
    BlockRegister() = default;
    ~BlockRegister() = default;
    unsigned blockCount = 0;
    std::unordered_map<int,Block> blocks;
};
#endif