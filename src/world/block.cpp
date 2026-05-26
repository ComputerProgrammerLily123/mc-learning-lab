#include "block.h"
Block::Block()
{
}
Block::~Block()
{
}
unsigned Block::GetID() const
{
    return id;
}
std::string Block::GetName() const
{
    return name;
}
const std::array<int, 6> Block::GetUVOffsets() const
{
    return uvOffsets;
}
const std::vector<std::string> Block::GetTextures() const
{
    return textures;
}
bool Block::GetSolid() const
{
    return isSolid;
}
void Block::SetID(unsigned id)
{
    this->id = id;
}
void Block::SetName(const std::string &name)
{
    this->name = name;
}
void Block::SetUVOffsets(const std::array<int, 6> &uvOffsets)
{
    this->uvOffsets = uvOffsets;
}
void Block::SetTextures(const std::vector<std::string> &textures)
{
    this->textures = textures;
}
void Block::SetSolid(bool isSolid)
{
    this->isSolid = isSolid;
}
void BlockRegister::RegisterBlock(const std::string &name, const std::array<int, 6> &uvOffsets, const std::vector<std::string> &textures, bool solid)
{
    auto block = Block();
    block.SetID(blockCount);
    block.SetName(name);
    block.SetUVOffsets(uvOffsets);
    block.SetTextures(textures);
    block.SetSolid(solid);
    blocks[blockCount] = block;
    blockCount++;
}
Block BlockRegister::GetBlock(int id)
{
    return blocks[id];
}
unsigned BlockRegister::GetBlockCount() const
{
    return blocks.size();
}