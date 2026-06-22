#pragma once
#include <string>
#include <unordered_map>
class ItemModelBase
{
public:
    virtual std::string GetTexture(const std::string& layer = "layer0") const;
protected:
    std::string parent;
    std::unordered_map<std::string,std::string> textures;
};