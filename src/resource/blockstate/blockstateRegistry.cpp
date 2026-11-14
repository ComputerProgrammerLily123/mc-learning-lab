#include "blockstateRegistry.h"

#include <limits>
#include <stdexcept>

namespace blockstate
{
BlockStateId BlockStateRegistry::Register(unsigned blockId, std::unordered_map<std::string, std::string> properties)
{
    if (states.size() >= std::numeric_limits<BlockStateId>::max())
        throw std::runtime_error("Too many block states.");
    const auto id = static_cast<BlockStateId>(states.size());
    states.push_back({blockId, std::move(properties)});
    return id;
}

void BlockStateRegistry::SetDefaultState(unsigned blockId, BlockStateId stateId)
{
    defaultStates.insert_or_assign(blockId, stateId);
}

BlockStateId BlockStateRegistry::GetDefaultState(unsigned blockId) const
{
    const auto found = defaultStates.find(blockId);

    if (found == defaultStates.end())
        throw std::out_of_range("Block has no default state.");

    return found->second;
}

const RuntimeBlockState& BlockStateRegistry::Get(BlockStateId id) const
{
    if (id >= states.size())
        throw std::out_of_range("Invalid block state id.");

    return states[id];
}
std::optional<BlockStateId> BlockStateRegistry::Find(unsigned blockId, const std::unordered_map<std::string, std::string>& properties) const
{
    for (std::size_t i = 0; i < states.size(); ++i)
    {
        const auto& state = states[i];

        if (state.blockId == blockId &&
            state.properties == properties)
        {
            return static_cast<BlockStateId>(i);
        }
    }
    return std::nullopt;
}
}