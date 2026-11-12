#pragma once
#include "modelDefinition.h"
#include "util/jsonReader.h"

using namespace model;
class ModelParser
{
public:
    ModelParser() = delete;
    ~ModelParser() = delete;
    ModelParser(const ModelParser&) = delete;
    ModelParser& operator=(const ModelParser&) = delete;
    ModelParser(ModelParser&&) = delete;
    ModelParser& operator=(ModelParser&&) = delete;
    
    static model::Model Parse(const Json& rawJson);
    static model::Model Bake(const model::Model& rawModel);

private:
    static model::Element ParseElement(const Json& rawJson);
    static model::Face ParseFace(const Json& rawJson);
    static std::unordered_map<std::string, model::Face> ParseFaces(const Json& rawJson);
    static model::Rotation ParseRotation(const Json& rawJson);
    static model::Model ParseTexture(model::Model& rawModel);
};