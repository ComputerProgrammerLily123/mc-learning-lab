#ifndef BLOCK_H
#define BLOCK_H

#include <string>
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
struct BlockProperties
{
    int uvOffset[6];
};
class Block
{
public:
    Block(std::string id);
    ~Block();
    std::string GetID() const;
private:
    std::string id;
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
    void RegisterBlock(int id,BlockProperties prop);
    struct BlockProperties GetBlockProperty(int id);
private:
    BlockRegister() = default;
    ~BlockRegister() = default;
    std::unordered_map<int, BlockProperties> blocks;
};
#endif