#pragma once
#include<string>
class ItemBase
{
public:
    unsigned GetID() const;
    std::string GetName() const;
    std::string GetIcon() const;

    void SetID(unsigned id);
    void SetName(std::string name);
    void SetIcon(std::string icon);
private:
    unsigned id;
    std::string name;
    std::string icon;
};