#pragma once

#include <string>
#include <unordered_map>

#include "blockBase.h"

class BlockRegister
{
public:
    static BlockRegister& GetInstance()
    {
        static BlockRegister instance;
        return instance;
    }
    BlockRegister(const BlockRegister&) = delete;
    BlockRegister& operator=(const BlockRegister&) = delete;
    BlockRegister(BlockRegister&&) = delete;
    BlockRegister& operator=(BlockRegister&&) = delete;
    void RegisterAll();
    BlockBase GetBlock(int id);
    unsigned GetBlockCount() const;
    
private:
    BlockRegister() = default;
    ~BlockRegister() = default;
    unsigned blockCount = 0;
    std::unordered_map<unsigned, BlockBase> blocks;
    BlockBase RegisterBlock(const std::string& name, bool isSolid = true);
};