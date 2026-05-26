#ifndef SHADER_H
#define SHADER_H

#include <glm/glm.hpp>
#include <string>

class Shader
{
public:
    unsigned ID;
    Shader(const std::string &vertexPath, const std::string &fragmentPath);

    void Use();
    void setValue(const std::string &name, bool value) const;
    void setValue(const std::string &name, int value) const;
    void setValue(const std::string &name, float value) const;
    void setValue(const std::string &name, const glm::vec2 &value) const;
    void setValue(const std::string &name, float x, float y) const;
    void setValue(const std::string &name, const glm::vec3 &value) const;
    void setValue(const std::string &name, float x, float y, float z) const;
    void setValue(const std::string &name, const glm::vec4 &value) const;
    void setValue(const std::string &name, float x, float y, float z, float w) const;
    void setValue(const std::string &name, const glm::mat2 &mat) const;
    void setValue(const std::string &name, const glm::mat3 &mat) const;
    void setValue(const std::string &name, const glm::mat4 &mat) const;
};

#endif