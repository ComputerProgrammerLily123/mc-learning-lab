#include "block.h"
Block::Block(std::string id)
{
    this->id = id;
}
Block::~Block()
{
}
std::string Block::GetID() const
{
    return id;
}
void BlockRegister::RegisterBlock(int id, BlockProperties prop)
{
    blocks[id] = prop;
}
struct BlockProperties BlockRegister::GetBlockProperty(int id)
{
    return blocks[id];
}