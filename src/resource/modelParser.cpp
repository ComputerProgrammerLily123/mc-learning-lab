#include "modelParser.h"

#include <unordered_set>

RawModel ModelParser::Parse(const Json& rawJson)
{
    RawModel model;
    
    if(const auto& iter = rawJson.find("parent"); iter != rawJson.end())
    {
        if(!iter->is_string())
        {
            throw std::runtime_error("Model parent must be a string.");
        }
        model.parent = iter->get<std::string>();
    }
    if(const auto& iter = rawJson.find("ambientocclusion"); iter != rawJson.end())
    {
        if(!iter->is_boolean())
        {
            throw std::runtime_error("Model ambientocclusion must be a boolean.");
        }
        model.ambientOcclusion = iter->get<bool>();
    }
    if (const auto& iter = rawJson.find("textures"); iter != rawJson.end())
    {
        if (!iter->is_object())
        {
            throw std::runtime_error("Model textures must be an object.");
        }
        for (const auto& [key, value] : iter->items())
        {
            if (!value.is_string())
            {
                throw std::runtime_error("Model texture value must be a string: " + key);
            }
            if(!model.textures.has_value())
            {
                model.textures.emplace();
            }
            model.textures->emplace(key, value.get<std::string>());
        }
    }
    if (const auto& iter = rawJson.find("elements"); iter != rawJson.end())
    {
        if (!iter->is_array())
        {
            throw std::runtime_error("Model elements must be an array.");
        }
        std::vector<ModelElement> result;
        result.reserve(iter->size());
        for (const Json& elementJson : *iter)
        {
            result.push_back(ParseElement(elementJson));
        }
        model.elements = std::move(result);
    }
    return model;
}
ResolvedModel ModelParser::Resolve(const RawModel& rawModel)
{
    ResolvedModel result;
    if(rawModel.parent.has_value())
    {
        result = Resolve(Parse(JsonReader::ReadJson("assets/minecraft/models/" + *rawModel.parent + ".json")));
    }
    if(rawModel.ambientOcclusion.has_value())
    {
        result.ambientOcclusion = rawModel.ambientOcclusion;
    }
    if(rawModel.textures.has_value())
    {
        if(!result.textures.has_value())
        {
            result.textures.emplace();
        }
        result.textures->reserve(result.textures->size() + rawModel.textures->size());
        for(const auto& [key, value] : *rawModel.textures)
        {
            result.textures->insert_or_assign(key, value);
        }
    }
    if (rawModel.elements.has_value() && !rawModel.elements->empty())
    {
        result.elements = *rawModel.elements;
    }
    if(rawModel.display.has_value())
    {
        result.display = rawModel.display;
    }
    if(rawModel.guiLight.has_value())
    {
        result.guiLight = rawModel.guiLight;
    }
    result = ResolveTexture(result);
    return result;
}
ModelElement ModelParser::ParseElement(const Json& rawJson)
{
    if (!rawJson.is_object())
    {
        throw std::runtime_error("Model element must be an object.");
    }

    ModelElement element;

    const auto& from = rawJson.find("from");
    const auto& to = rawJson.find("to");
    const auto& faces = rawJson.find("faces");

    if (from == rawJson.end())
    {
        throw std::runtime_error("Model element requires from.");
    }

    if (to == rawJson.end())
    {
        throw std::runtime_error("Model element requires to.");
    }

    if (faces == rawJson.end())
    {
        throw std::runtime_error("Model element requires faces.");
    }

    element.from = ParseVector3(*from);
    element.to = ParseVector3(*to);

    element.faces = ParseFaces(*faces);

    if (const auto& rotation = rawJson.find("rotation"); rotation != rawJson.end())
    {
        element.rotation = ParseElementRotation(*rotation);
    }

    if (const auto& shade = rawJson.find("shade"); shade != rawJson.end())
    {
        if (!shade->is_boolean())
        {
            throw std::runtime_error("Element shade must be boolean.");
        }
        element.shade = shade->get<bool>();
    }

    return element;
}
ModelFace ModelParser::ParseFace(const Json& rawJson)
{
    if (!rawJson.is_object())
    {
        throw std::runtime_error("Face must be an object.");
    }

    ModelFace face;

    const auto& texture = rawJson.find("texture");

    if (texture == rawJson.end() || !texture->is_string())
    {
        throw std::runtime_error("Face requires a string texture.");
    }

    face.texture = texture->get<std::string>();

    if (face.texture.empty())
    {
        throw std::runtime_error("Face texture cannot be empty.");
    }

    if (const auto& uv = rawJson.find("uv"); uv != rawJson.end())
    {
        face.uv = ParseVector4(*uv);
    }

    if (const auto& cullFace = rawJson.find("cullface"); cullFace != rawJson.end())
    {
        if (!cullFace->is_string())
        {
            throw std::runtime_error("Face cullface must be a string.");
        }

        face.cullFace = cullFace->get<std::string>();
    }

    if (const auto& rotation = rawJson.find("rotation"); rotation != rawJson.end())
    {
        if (!rotation->is_number_integer())
        {
            throw std::runtime_error("Face rotation must be an integer.");
        }

        face.rotation = rotation->get<int>();

        if (face.rotation != 0 && face.rotation != 90 && face.rotation != 180 && face.rotation != 270)
        {
            throw std::runtime_error("Face rotation must be 0, 90, 180 or 270.");
        }
    }

    if (const auto& tintIndex = rawJson.find("tintindex"); tintIndex != rawJson.end())
    {
        if (!tintIndex->is_number_integer())
        {
            throw std::runtime_error("Face tintindex must be an integer.");
        }

        face.tintIndex = tintIndex->get<int>();
    }

    return face;
}
std::unordered_map<Direction, ModelFace> ModelParser::ParseFaces(const Json& rawJson)
{
    if (!rawJson.is_object())
    {
        throw std::runtime_error("Element faces must be an object.");
    }

    if (rawJson.empty())
    {
        throw std::runtime_error("Element faces cannot be empty.");
    }

    std::unordered_map<Direction, ModelFace> faces;

    for (const auto& [directionName, faceJson] : rawJson.items())
    {
        const Direction direction = StringToDirection(directionName);

        const auto& [iterator, inserted] = faces.emplace(direction,ParseFace(faceJson));

        if (!inserted)
        {
            throw std::runtime_error("Duplicate face direction: " + directionName);
        }
    }
    return faces;
}
glm::vec3 ModelParser::ParseVector3(const Json& rawJson)
{
    if (!rawJson.is_array() || rawJson.size() != 3)
    {
        throw std::runtime_error("Vector3 must be an array containing 3 numbers");
    }

    for (const auto& member : rawJson)
    {
        if (!member.is_number())
        {
            throw std::runtime_error("Vector3 must contain only numbers");
        }
    }
    return glm::vec3{rawJson[0].get<float>(), rawJson[1].get<float>(), rawJson[2].get<float>()};
}
glm::vec4 ModelParser::ParseVector4(const Json& rawJson)
{
    if (!rawJson.is_array() || rawJson.size() != 4)
    {
        throw std::runtime_error("Vector4 must be an array containing 4 numbers");
    }

    for (const auto& member : rawJson)
    {
        if (!member.is_number())
        {
            throw std::runtime_error("Vector4 must contain only numbers");
        }
    }
    return glm::vec4{rawJson[0].get<float>(), rawJson[1].get<float>(), rawJson[2].get<float>(), rawJson[3].get<float>()};
}
ElementRotation ModelParser::ParseElementRotation(const Json& rawJson)
{
    if (!rawJson.is_object())
    {
        throw std::runtime_error("Element rotation must be an object.");
    }

    ElementRotation rotation;

    const auto& origin = rawJson.find("origin");

    if (origin == rawJson.end())
    {
        throw std::runtime_error("Element rotation requires origin.");
    }

    rotation.origin = ParseVector3(*origin);

    const auto& axis = rawJson.find("axis");

    if (axis == rawJson.end() || !axis->is_string())
    {
        throw std::runtime_error("Element rotation requires a string axis.");
    }

    rotation.axis = ParseRotationAxis(axis->get<std::string>());

    const auto& angle = rawJson.find("angle");

    if (angle == rawJson.end() || !angle->is_number())
    {
        throw std::runtime_error("Element rotation requires a numeric angle");
    }

    rotation.angle = angle->get<float>();

    if (rotation.angle != -45.0f && rotation.angle != -22.5f && rotation.angle != 0.0f && rotation.angle != 22.5f && rotation.angle != 45.0f)
    {
        throw std::runtime_error("Element rotation angle must be -45, -22.5, 0, 22.5 or 45.");
    }

    if (const auto& rescale = rawJson.find("rescale"); rescale != rawJson.end())
    {
        if (!rescale->is_boolean())
        {
            throw std::runtime_error("Element rotation rescale must be boolean.");
        }

        rotation.rescale = rescale->get<bool>();
    }

    return rotation;
}

ResolvedModel ModelParser::ResolveTexture(ResolvedModel& resolvedModel)
{
    if (!resolvedModel.textures)
    {
        return resolvedModel;
    }

    const auto resolveReference =[&resolvedModel](std::string& reference) -> std::optional<std::string>
    {
        std::unordered_set<std::string> visited;

        while (!reference.empty() && reference.front() == '#')
        {
            const std::string variable(reference.substr(1));

            if (variable.empty())
            {
                return std::nullopt;
            }

            if (!visited.insert(variable).second)
            {
                throw std::runtime_error("Circular texture reference: #" +variable);
            }

            const auto iterator = resolvedModel.textures->find(variable);

            if (iterator == resolvedModel.textures->end())
            {
                return std::nullopt;
            }

            reference = iterator->second;
        }

        if (reference.empty())
        {
            return std::nullopt;
        }

        return std::string(reference);
    };

    for (auto& [name, reference] : *resolvedModel.textures)
    {
        if (const auto resolved = resolveReference(reference))
        {
            reference = *resolved;
        }
    }

    if (resolvedModel.elements)
    {
        for (ModelElement& element : *resolvedModel.elements)
        {
            for (auto& [direction, face] : element.faces)
            {
                if (const auto resolved = resolveReference(face.texture))
                {
                    face.texture = *resolved;
                }
            }
        }
    }
    return resolvedModel;
}
