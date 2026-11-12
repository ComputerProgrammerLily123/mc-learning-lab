#include "jsonReader.h"

#include <fstream>
#include <iostream>
#include <sstream>

Json JsonReader::ReadJson(const std::string& filePath)
{
    std::string rawJson;
    std::ifstream jsonFile;

    jsonFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
    try
    {
        jsonFile.open(filePath);
        std::stringstream jsonFileStream;
        jsonFileStream << jsonFile.rdbuf();
        jsonFile.close();
        rawJson = jsonFileStream.str();
    }
    catch (const std::ifstream::failure& e)
    {
        std::cout << "ERROR::JSON::FILE_NOT_SUCCESS_READ:" << filePath << '\n';
        return Json{};
    }
    return Json::parse(rawJson);
}
template <>
std::optional<glm::vec3> JsonReader::GetData(const Json &json, const std::string &key)
{
    if (!json.is_object()) return std::nullopt;

    const auto iter = json.find(key);

    if (iter != json.end() && iter->is_array() && iter->size() == 3)
    {  
        for (const auto& member : *iter)
        {
            if (!member.is_number())
            {
                return std::nullopt;
            }
        }
        return glm::vec3{iter->at(0).get<float>(), iter->at(1).get<float>(), iter->at(2).get<float>()};
    }
    return std::nullopt;
}
template <>
std::optional<glm::vec4> JsonReader::GetData(const Json &json, const std::string &key)
{
    if (!json.is_object()) return std::nullopt;

    const auto iter = json.find(key);

    if (iter != json.end() && iter->is_array() && iter->size() == 4)
    {  
        for (const auto& member : *iter)
        {
            if (!member.is_number())
            {
                return std::nullopt;
            }
        }
        return glm::vec4{iter->at(0).get<float>(), iter->at(1).get<float>(), iter->at(2).get<float>(), iter->at(3).get<float>()};
    }
    return std::nullopt;
}