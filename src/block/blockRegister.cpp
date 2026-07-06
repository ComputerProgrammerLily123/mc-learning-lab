#include "blockRegister.h"
void BlockRegister::RegisterBlock(const std::string &name, const std::array<int, FACE_COUNT> &uvOffsets, const std::vector<std::string> &textures, bool solid)
{
    auto block = BlockBase();
    block.SetID(blockCount);
    block.SetName(name);
    block.SetUVOffsets(uvOffsets);
    block.SetTextures(textures);
    block.SetSolid(solid);
    blocks[blockCount] = block;
    blockCount++;
}
BlockBase BlockRegister::GetBlock(int id)
{
    return blocks[id];
}
unsigned BlockRegister::GetBlockCount() const
{
    return blocks.size();
}