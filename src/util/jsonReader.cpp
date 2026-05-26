#include "jsonReader.h"
#include <fstream>
#include <sstream>
#include <iostream>
nlohmann::json JsonReader::ReadJson(const std::string &filePath)
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
    catch (std::ifstream::failure e)
    {
        std::cout << "ERROR::JSON::FILE_NOT_SUCCESS_READ" << std::endl;
    }
    return nlohmann::json::parse(rawJson);
}