#include "blockstateParser.h"
blockstate::BlockStateDefinition BlockstateParser::Parse(const Json& rawJson)
{
    if (!rawJson.is_object())
        throw std::runtime_error("Blockstate root must be an object.");

    const auto variants = rawJson.find("variants");
    const auto multipart = rawJson.find("multipart");

    if (variants == rawJson.end() && multipart == rawJson.end())
    {
        throw std::runtime_error("Blockstate requires variants or multipart.");
    }

    blockstate::BlockStateDefinition result;

    if (variants != rawJson.end())
    {
        if (!variants->is_object())
            throw std::runtime_error("Blockstate variants must be an object.");

        result.variants.reserve(variants->size());

        for (const auto& [key, value] : variants->items())
            result.variants.push_back(ParseVariantEntry(key, value));
    }

    if (multipart != rawJson.end())
        result.multipart = ParseMultipart(*multipart);

    return result;
}

blockstate::VariantEntry BlockstateParser::ParseVariantEntry(const std::string& key, const Json& value)
{
    blockstate::VariantEntry result;
    result.conditions = ParseVariantConditions(key);
    result.models = ParseModelVariants(value);
    return result;
}

std::unordered_map<std::string, std::string> BlockstateParser::ParseVariantConditions(const std::string& key)
{
    std::unordered_map<std::string, std::string> result;

    if (key.empty() || key == "normal") return result;

    std::size_t begin = 0;

    while (begin < key.size())
    {
        const std::size_t comma = key.find(',', begin);
        const std::size_t end = comma == std::string::npos ? key.size() : comma;

        const std::string condition = key.substr(begin, end - begin);

        const std::size_t equals = condition.find('=');

        if (equals == std::string::npos || equals == 0 || equals + 1 == condition.size())
        {
            throw std::runtime_error("Invalid variant condition: " + condition);
        }

        const std::string property = condition.substr(0, equals);

        const std::string value = condition.substr(equals + 1);

        if (!result.emplace(property, value).second)
        {
            throw std::runtime_error("Duplicate variant property: " + property);
        }

        if (comma == std::string::npos)
            break;

        begin = comma + 1;
    }

    return result;
}

std::vector<blockstate::ModelVariant> BlockstateParser::ParseModelVariants(const Json& json)
{
    std::vector<blockstate::ModelVariant> result;

    if (json.is_object())
    {
        result.push_back(ParseModelVariant(json));
        return result;
    }

    if (!json.is_array() || json.empty())
    {
        throw std::runtime_error("Variant value must be a model object or non-empty array.");
    }

    result.reserve(json.size());

    for (const Json& item : json)
    {
        result.push_back(ParseModelVariant(item));
    }

    return result;
}

blockstate::ModelVariant BlockstateParser::ParseModelVariant(const Json& json)
{
    if (!json.is_object()) throw std::runtime_error("Model variant must be an object.");

    const auto modelName = JsonReader::GetData<std::string>(json, "model");

    if (!modelName) throw std::runtime_error("Model variant requires model.");

    const int x = JsonReader::GetDataOrDefault<int>(json, "x", 0);
    const int y = JsonReader::GetDataOrDefault<int>(json, "y", 0);
    const int weight = JsonReader::GetDataOrDefault<int>(json, "weight", 1);
    const bool uvLock = JsonReader::GetDataOrDefault<bool>(json, "uvlock", false);

    const auto validRotation = [](int angle)
    {
        return angle == 0 || angle == 90 || angle == 180 || angle == 270;
    };

    if (!validRotation(x) || !validRotation(y))
    {
        throw std::runtime_error("Blockstate rotation must be 0, 90, 180 or 270.");
    }

    if (weight <= 0) throw std::runtime_error("Model weight must be positive.");

    ResourceLocation source(*modelName);

    const std::string& path = source.GetPath();

    const bool hasBlockPrefix = path.rfind("block/", 0) == 0;

    const std::string modelPath = hasBlockPrefix ? path : "block/" + path;

    ResourceLocation modelLocation(source.GetNamespace(), modelPath);

    return {std::move(modelLocation), x, y, uvLock, weight};
}
std::vector<blockstate::MultipartEntry> BlockstateParser::ParseMultipart(const Json& json)
{
    if (!json.is_array() || json.empty())
        throw std::runtime_error("Blockstate multipart must be a non-empty array.");

    std::vector<blockstate::MultipartEntry> result;
    result.reserve(json.size());

    for (const auto& entry : json)
        result.push_back(ParseMultipartEntry(entry));

    return result;
}

blockstate::MultipartEntry BlockstateParser::ParseMultipartEntry(const Json& json)
{
    if (!json.is_object())
        throw std::runtime_error("Multipart entry must be an object.");

    const auto apply = json.find("apply");

    if (apply == json.end())
        throw std::runtime_error("Multipart entry requires apply.");

    blockstate::MultipartEntry result;
    result.models = ParseModelVariants(*apply);

    const auto when = json.find("when");

    if (when != json.end())
        result.condition = ParseMultipartCondition(*when);

    return result;
}

blockstate::MultipartCondition BlockstateParser::ParseMultipartCondition(const Json& json)
{
    if (!json.is_object() || json.empty())
        throw std::runtime_error("Multipart when must be a non-empty object.");

    blockstate::MultipartCondition result;
    const auto orCondition = json.find("OR");

    if (orCondition == json.end())
    {
        result.alternatives.push_back(ParseConditionGroup(json));
        return result;
    }

    if (json.size() != 1)
        throw std::runtime_error("Multipart OR cannot be mixed with direct properties.");

    if (!orCondition->is_array() || orCondition->empty())
        throw std::runtime_error("Multipart OR must be a non-empty array.");

    result.alternatives.reserve(orCondition->size());

    for (const auto& alternative : *orCondition)
        result.alternatives.push_back(
            ParseConditionGroup(alternative));

    return result;
}
std::vector<std::string> BlockstateParser::SplitValues(const std::string& value)
{
    std::vector<std::string> result;
    std::size_t begin = 0;

    while (begin <= value.size())
    {
        const std::size_t separator = value.find('|', begin);
        const std::size_t end = separator == std::string::npos ? value.size() : separator;

        std::string part = value.substr(begin, end - begin);

        if (part.empty())
            throw std::runtime_error("Empty multipart property value.");

        result.push_back(std::move(part));

        if (separator == std::string::npos)
            break;

        begin = separator + 1;
    }

    return result;
}

std::string BlockstateParser::ParseConditionValue(const Json& value)
{
    if (value.is_string())
        return value.get<std::string>();

    if (value.is_boolean() || value.is_number_integer() || value.is_number_unsigned())
    {
        return value.dump();
    }

    throw std::runtime_error("Multipart property value must be a string, boolean or integer.");
}

blockstate::ConditionGroup BlockstateParser::ParseConditionGroup(const Json& json)
{
    if (!json.is_object() || json.empty())
        throw std::runtime_error("Multipart condition group must be a non-empty object.");

    blockstate::ConditionGroup group;
    group.conditions.reserve(json.size());

    for (const auto& [name, value] : json.items())
    {
        if (name == "OR")
            throw std::runtime_error("Nested multipart OR is not supported.");

        blockstate::PropertyCondition condition;
        condition.name = name;
        condition.acceptedValues = SplitValues(ParseConditionValue(value));

        group.conditions.push_back(std::move(condition));
    }

    return group;
}