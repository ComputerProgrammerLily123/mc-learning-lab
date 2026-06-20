#pragma once
#include<string>
#include "nlohmann/json.hpp"
class JsonReader{
public:
    JsonReader() = delete;
    ~JsonReader() = delete;
    JsonReader(const JsonReader&) = delete;
    JsonReader& operator=(const JsonReader&) = delete;
    static nlohmann::json ReadJson(const std::string& filePath);
private:
};