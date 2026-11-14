#pragma once

#include "blockstateDefinition.h"
#include "util/jsonReader.h"
class BlockstateParser
{
public:
    BlockstateParser() = delete;
    ~BlockstateParser() = delete;
    BlockstateParser(const BlockstateParser&) = delete;
    BlockstateParser(BlockstateParser&&) = delete;
    BlockstateParser& operator=(const BlockstateParser&) = delete;
    BlockstateParser& operator=(BlockstateParser&&) = delete;

    static blockstate::BlockStateDefinition Parse(const Json& rawJson);

private:

    static blockstate::VariantEntry ParseVariantEntry(const std::string& key, const Json& value);

    static std::unordered_map<std::string, std::string> ParseVariantConditions(const std::string& key);

    static std::vector<blockstate::ModelVariant> ParseModelVariants(const Json& json);

    static blockstate::ModelVariant ParseModelVariant(const Json& json);

    static std::vector<blockstate::MultipartEntry> ParseMultipart(const Json& json);

    static blockstate::MultipartEntry ParseMultipartEntry(const Json& json);

    static blockstate::MultipartCondition ParseMultipartCondition(const Json& json);

    static std::vector<std::string> SplitValues(const std::string& value);

    static std::string ParseConditionValue(const Json& value);

    static blockstate::ConditionGroup ParseConditionGroup(const Json& json);

};