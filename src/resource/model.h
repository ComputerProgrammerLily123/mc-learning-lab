#pragma once
#include <glm/glm.hpp>
#include <string>
#include <unordered_map>
#include <vector>

struct Rotation
{
    glm::vec3 origin;
    bool rescale;

    // TODO:axis
};
struct Display
{
    glm::vec3 scale;
    glm::vec3 rotation;
    glm::vec3 translation;
};
struct Face
{
    std::string face;
    glm::vec4 uv;
    int rotation;
    int tintindex;
    std::string cullface;
};
struct ModelElement
{
    glm::vec3 from;
    glm::vec3 to;
    Rotation rotation;
    bool shade;
    int lightEmission;
    std::unordered_map<std::string, Face> faces;
};
struct ModelFormat
{
    std::string parent;
    bool ambientOcclusion;
    std::unordered_map<std::string, std::string> textures;
    std::vector<ModelElement> elements;
    Display display;
    std::string guiLight;
};
class Model
{
public:
    
private:
    ModelFormat modelFormat;
};