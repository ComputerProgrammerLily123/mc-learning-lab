#pragma once

#include <unordered_map>
#include <vector>
#include <string>
#include <cstdint>
#include <memory>
#include <optional>

#include "resource/model/bakedModel.h"
#include "resource/resourceLocation.h"
namespace blockstate
{
struct ModelVariant
{
    ResourceLocation model;
    int xRotation = 0;
    int yRotation = 0;
    bool uvLock = false;
    int weight = 1;
};

struct VariantEntry
{
    std::unordered_map<std::string, std::string> conditions;
    std::vector<ModelVariant> models;
};

struct PropertyCondition
{
    std::string name;
    std::vector<std::string> acceptedValues;
};

struct ConditionGroup
{
    // 组内条件是 AND
    std::vector<PropertyCondition> conditions;
};

struct MultipartCondition
{
    // 多个组之间是 OR
    std::vector<ConditionGroup> alternatives;
};

struct MultipartEntry
{
    // nullopt 表示无条件应用
    std::optional<MultipartCondition> condition;
    std::vector<ModelVariant> models;
};

struct BakedModelVariant
{
    std::shared_ptr<const model::BakedModel> model;
    int weight = 1;
};

struct BakedVariantEntry
{
    std::unordered_map<std::string, std::string> conditions;
    std::vector<BakedModelVariant> models;
};

struct BakedMultipartEntry
{
    std::optional<MultipartCondition> condition;
    std::vector<BakedModelVariant> models;
};

struct BlockStateDefinition
{
    std::vector<VariantEntry> variants;
    std::vector<MultipartEntry> multipart;
};


struct BakedBlockStateDefinition
{
    std::vector<BakedVariantEntry> variants;
    std::vector<BakedMultipartEntry> multipart;
};


using BlockStateId = std::uint16_t;

struct RuntimeBlockState
{
    unsigned blockId;
    std::unordered_map<std::string, std::string> properties;
};

inline bool Matches(const blockstate::VariantEntry& entry, const RuntimeBlockState& state)
{
    for (const auto& [name, expected] : entry.conditions)
    {
        const auto found = state.properties.find(name);

        if (found == state.properties.end() || found->second != expected)
        {
            return false;
        }
    }
    return true;
}
inline bool Matches(const blockstate::BakedVariantEntry& entry, const RuntimeBlockState& state)
{
    for (const auto& [name, expected] : entry.conditions)
    {
        const auto found = state.properties.find(name);

        if (found == state.properties.end() || found->second != expected)
        {
            return false;
        }
    }
    return true;
}
inline bool Matches(const PropertyCondition& condition, const RuntimeBlockState& state)
{
    const auto found = state.properties.find(condition.name);

    if (found == state.properties.end())
        return false;

    for (const auto& accepted : condition.acceptedValues)
    {
        if (found->second == accepted)
            return true;
    }

    return false;
}

inline bool Matches(const ConditionGroup& group, const RuntimeBlockState& state)
{
    for (const auto& condition : group.conditions)
    {
        if (!Matches(condition, state))
            return false;
    }

    return true;
}

inline bool Matches(const MultipartCondition& condition, const RuntimeBlockState& state)
{
    for (const auto& alternative : condition.alternatives)
    {
        if (Matches(alternative, state))
            return true;
    }

    return false;
}
}