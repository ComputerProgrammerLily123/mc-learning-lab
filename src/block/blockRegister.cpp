#include "blockRegister.h"
void BlockRegister::RegisterAll()
{
    RegisterBlock("air", false);
    RegisterBlock("grass");
    RegisterBlock("dirt");
    RegisterBlock("stone");
    RegisterBlock("oak_log");
    RegisterBlock("iron_ore");
    RegisterBlock("lever", false);
    RegisterBlock("fence", false);
    RegisterBlock("blue_wool");
    RegisterBlock("sand");
    RegisterBlock("oak_leaves", false);
}
BlockBase BlockRegister::RegisterBlock(const std::string &name, bool isSolid)
{
    auto block = BlockBase();
    block.SetID(blockCount);
    block.SetName(name);
    block.SetSolid(isSolid);
    blocks[blockCount] = block;
    blockCount++;
    return block;
}
const BlockBase& BlockRegister::GetBlock(int id) const
{
    return blocks.at(id);
}
unsigned BlockRegister::GetBlockCount() const
{
    return blocks.size();
}