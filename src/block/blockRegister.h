#pragma once

#include <string>
#include <vector>
#include <array>
#include <unordered_map>

#include "blockBase.h"
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
    BlockBase GetBlock(int id);
    unsigned GetBlockCount() const;
private:
    BlockRegister() = default;
    ~BlockRegister() = default;
    unsigned blockCount = 0;
    std::unordered_map<int,BlockBase> blocks;
};