#include "blockRegister.h"
void BlockRegister::RegisterAll()
{
    RegisterBlock("air", false);
    RegisterBlock("grass");
    RegisterBlock("dirt");
    RegisterBlock("stone");
    RegisterBlock("fence", false);
    RegisterBlock("oak_log");
    RegisterBlock("stone_button", false);
    RegisterBlock("lever", false);
    RegisterBlock("beacon");
    RegisterBlock("birch_log");
    RegisterBlock("blue_wool");
    RegisterBlock("clay");
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
BlockBase BlockRegister::GetBlock(int id)
{
    return blocks[id];
}
unsigned BlockRegister::GetBlockCount() const
{
    return blocks.size();
}