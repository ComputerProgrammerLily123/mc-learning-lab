#include "modelParser.h"

#include <unordered_set>
#include "resource/resourceManager.h"

model::Model ModelParser::Parse(const Json& rawJson)
{
    model::Model model;
    model.parent = JsonReader::GetData<std::string>(rawJson, "parent");
    model.ambientOcclusion = JsonReader::GetData<bool>(rawJson, "ambientocclusion");

    if (auto iter = rawJson.find("textures"); iter != rawJson.end())
    {
        if (!iter->is_object()) throw std::runtime_error("Model textures must be an object.");

        std::unordered_map<std::string, std::string> textures;
        for (const auto& [key, value] : iter->items())
        {
            if (!value.is_string())
                throw std::runtime_error("Model texture value must be a string: " + key);
            textures.emplace(key, value.get<std::string>());
        }
        model.textures = std::move(textures);
    }
    if (const auto& iter = rawJson.find("elements"); iter != rawJson.end())
    {
        if (!iter->is_array()) throw std::runtime_error("Model elements must be an array.");
        
        std::vector<Element> result;
        result.reserve(iter->size());
        for (const Json& elementJson : *iter)
        {
            result.push_back(ParseElement(elementJson));
        }
        model.elements = std::move(result);
    }
    return model;
}
model::Model ModelParser::Bake(const model::Model& rawModel)
{
    model::Model result;
    if(rawModel.parent.has_value())
    {
        ResourceLocation location(*rawModel.parent);
        result = Bake(Parse(JsonReader::ReadJson(ResourceManager::GetInstance().GetModelPath(location).string())));
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
    if(rawModel.gui_light.has_value())
    {
        result.gui_light = rawModel.gui_light;
    }
    result = ParseTexture(result);
    return result;
}
model::Element ModelParser::ParseElement(const Json& rawJson)
{
    if (!rawJson.is_object())
    {
        throw std::runtime_error("Model element must be an object.");
    }

    model::Element element;

    if (auto from = JsonReader::GetData<glm::vec3>(rawJson, "from"))
    {
        element.from = *from;
    }
    else
    {
        throw std::runtime_error("Model element requires from.");
    }
    if (auto to = JsonReader::GetData<glm::vec3>(rawJson, "to"))
    {
        element.to = *to;
    }
    else
    {
        throw std::runtime_error("Model element requires to.");
    }

    if (const auto& faces = rawJson.find("faces"); faces != rawJson.end())
    {
        element.faces = ParseFaces(*faces);
    }
    else
    {
        throw std::runtime_error("Model element requires faces.");
    }
    if (const auto& rotation = rawJson.find("rotation"); rotation != rawJson.end())
    {
        element.rotation = ParseRotation(*rotation);
    }

    element.shade = JsonReader::GetData<bool>(rawJson, "shade");

    return element;
}
model::Face ModelParser::ParseFace(const Json& rawJson)
{
    model::Face face;

    if(auto texture = JsonReader::GetData<std::string>(rawJson, "texture"))
    {
        face.texture = *texture;
    }
    else
    {
        throw std::runtime_error("Face requires a texture.");
    }
    face.uv = JsonReader::GetData<glm::vec4>(rawJson, "uv");
    face.cullFace = JsonReader::GetData<std::string>(rawJson, "cullface");
    if(auto rotation = JsonReader::GetData<int>(rawJson, "rotation"))
    {
        if (face.rotation != 0 && face.rotation != 90 && face.rotation != 180 && face.rotation != 270)
        {
            throw std::runtime_error("Face rotation must be 0, 90, 180 or 270.");
        }
        face.rotation = rotation;
    }
    face.tintIndex = JsonReader::GetData<int>(rawJson, "tintindex");

    return face;
}
std::unordered_map<std::string, model::Face> ModelParser::ParseFaces(const Json& rawJson)
{

    std::unordered_map<std::string, model::Face> faces;

    for (const auto& [directionName, faceJson] : rawJson.items())
    {
        const auto& [iterator, inserted] = faces.emplace(directionName,ParseFace(faceJson));

        if (!inserted)
        {
            throw std::runtime_error("Duplicate face direction: " + directionName);
        }
    }
    return faces;
}
model::Rotation ModelParser::ParseRotation(const Json& rawJson)
{
    model::Rotation rotation;

    const auto& axis = JsonReader::GetData<std::string>(rawJson, "axis");
    const auto& x = JsonReader::GetData<float>(rawJson, "x");
    const auto& y = JsonReader::GetData<float>(rawJson, "y");
    const auto& z = JsonReader::GetData<float>(rawJson, "z");

    if (axis)
    {
        model::SingleRotation singleRotation;
        if(auto origin = JsonReader::GetData<glm::vec3>(rawJson, "origin"))
        {
            singleRotation.origin = *origin;
        }
        else
        {
            throw std::runtime_error("Element rotation requires origin.");
        }
        singleRotation.rescale = JsonReader::GetData<bool>(rawJson, "rescale");
        singleRotation.axis = *axis;
        if(auto angle = JsonReader::GetData<float>(rawJson, "angle"))
        {
            if (angle != -45.0f && angle != -22.5f && angle != 0.0f && angle != 22.5f && angle != 45.0f)
            {
                throw std::runtime_error("Element rotation angle must be -45, -22.5, 0, 22.5 or 45.");
            }
            singleRotation.angle = *angle;
        }
        else
        {
            throw std::runtime_error("Element rotation requires angle.");
        }
        rotation.singleRotation = singleRotation;
    }
    if (x && y && z)
    {
        model::MultiRotation multiRotation;
        if(auto origin = JsonReader::GetData<glm::vec3>(rawJson, "origin"))
        {
            multiRotation.origin = *origin;
        }
        else
        {
            throw std::runtime_error("Element rotation requires origin.");
        }
        multiRotation.rescale = JsonReader::GetData<bool>(rawJson, "rescale");
        multiRotation.x = *x;
        multiRotation.y = *y;
        multiRotation.z = *z;
        rotation.multiRotation = multiRotation;
    }

    return rotation;
}

model::Model ModelParser::ParseTexture(model::Model& rawModel)
{
    if (!rawModel.textures)
    {
        return rawModel;
    }

    const auto resolveReference =[&rawModel](std::string& reference) -> std::optional<std::string>
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

            const auto iterator = rawModel.textures->find(variable);

            if (iterator == rawModel.textures->end())
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

    for (auto& [name, reference] : *rawModel.textures)
    {
        if (const auto resolved = resolveReference(reference))
        {
            reference = *resolved;
        }
    }

    if (rawModel.elements)
    {
        for (model::Element& element : *rawModel.elements)
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
    return rawModel;
}
