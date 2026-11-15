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
    static model::Model ResolveInheritance(const model::Model& rawModel);
    static model::Model Resolve(const model::Model& rawModel);

private:
    static model::Element ParseElement(const Json& rawJson);
    static model::Face ParseFace(const Json& rawJson);
    static std::array<std::optional<model::Face>, 6> ParseFaces(const Json& rawJson);
    static model::Rotation ParseRotation(const Json& rawJson);
    static model::Model ParseTexture(model::Model& rawModel);
    static std::optional<model::Display> ParseDisplay(const Json& json);
    static std::optional<model::DisplayMode> ParseDisplayMode(const Json& json, const char* name);
};