#include "itemBase.h"
unsigned ItemBase::GetID() const
{
    return id;
}
std::string ItemBase::GetName() const
{
    return name;
}
std::string ItemBase::GetIcon() const
{
    return icon;
}
void ItemBase::SetID(unsigned id)
{
    this->id = id;
}
void ItemBase::SetName(std::string name)
{
    this->name = name;
}
void ItemBase::SetIcon(std::string icon)
{
    this->icon = icon;
}