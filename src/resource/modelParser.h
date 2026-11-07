#pragma once
#include "model.h"
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
    
    static RawModel Parse(const Json& rawJson);
    static ResolvedModel Resolve(const RawModel& rawModel);

private:
    static ModelElement ParseElement(const Json& rawJson);
    static ModelFace ParseFace(const Json& rawJson);
    static std::unordered_map<Direction, ModelFace> ParseFaces(const Json& rawJson);
    static glm::vec3 ParseVector3(const Json& rawJson);
    static glm::vec4 ParseVector4(const Json& rawJson);
    static ElementRotation ParseElementRotation(const Json& rawJson);

    static ResolvedModel ResolveTexture(ResolvedModel& resolvedModel);
};