#pragma once
#include <string>
#include <glm/glm.hpp>
#include <optional>

#include "nlohmann/json.hpp"

using Json = nlohmann::json;

class JsonReader
{
public:
    JsonReader() = delete;
    ~JsonReader() = delete;
    JsonReader(JsonReader&&) = delete;
    JsonReader& operator=(JsonReader&&) = delete;
    JsonReader(const JsonReader&) = delete;
    JsonReader& operator=(const JsonReader&) = delete;
    static Json ReadJson(const std::string& filePath);

    template <typename T>
    static std::optional<T> GetData(const Json &json, const std::string& key)
    {
        if (json.is_object())
        {
            auto iter = json.find(key);
            if (iter != json.end())
            {
                try
                {
                    return iter->get<T>();
                }
                catch (const Json::exception&)
                {
                    return std::nullopt;
                }
            }
        }
        return std::nullopt;
    }
};
template <> std::optional<glm::vec3> JsonReader::GetData<glm::vec3>(const Json &json, const std::string &key);
template <> std::optional<glm::vec4> JsonReader::GetData<glm::vec4>(const Json &json, const std::string &key);