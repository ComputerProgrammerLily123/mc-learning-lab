#pragma once

#include <array>
#include <string>
#include <unordered_map>
#include <vector>

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
    void RegisterBlock(const std::string& name, const std::array<int, FACE_COUNT>& uvOffsets, const std::vector<std::string>& textures, bool solid);
    BlockBase GetBlock(int id);
    unsigned GetBlockCount() const;

private:
    BlockRegister() = default;
    ~BlockRegister() = default;
    unsigned blockCount = 0;
    std::unordered_map<unsigned, BlockBase> blocks;
};