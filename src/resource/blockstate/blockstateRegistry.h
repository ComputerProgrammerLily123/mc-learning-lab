#pragma once
#include <vector>

#include "blockstateDefinition.h"
namespace blockstate 
{
class BlockStateRegistry
{
public:

    BlockStateRegistry(const BlockStateRegistry&) = delete;
    BlockStateRegistry(BlockStateRegistry&&) = delete;
    BlockStateRegistry& operator=(const BlockStateRegistry&) = delete;
    BlockStateRegistry& operator=(BlockStateRegistry&&) = delete;

    static BlockStateRegistry& GetInstance()
    {
        static BlockStateRegistry instance;
        return instance;
    }
    BlockStateId Register(unsigned blockId, std::unordered_map<std::string, std::string> properties = {});
    void SetDefaultState(unsigned blockId, BlockStateId stateId);
    [[nodiscard]] BlockStateId GetDefaultState(unsigned blockId) const;
    [[nodiscard]] const RuntimeBlockState& Get(BlockStateId id) const;
    std::optional<BlockStateId> Find(unsigned blockId, const std::unordered_map<std::string, std::string>& properties) const;

private:
    BlockStateRegistry() = default;
    ~BlockStateRegistry() = default;
    std::vector<RuntimeBlockState> states;
    std::unordered_map<unsigned, BlockStateId> defaultStates;
};
}