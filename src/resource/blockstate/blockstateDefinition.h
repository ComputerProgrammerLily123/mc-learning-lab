#pragma once

#include <unordered_map>
#include <vector>
#include <string>

#include "resource/resourceLocation.h"

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

struct BlockStateDefinition
{
    std::vector<VariantEntry> variants;
};