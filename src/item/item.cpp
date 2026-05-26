#include "item.h"
unsigned Item::GetID() const
{
    return id;
}
std::string Item::GetName() const
{
    return name;
}
std::string Item::GetIcon() const
{
    return icon;
}
void Item::SetID(unsigned id)
{
    this->id = id;
}
void Item::SetName(std::string name)
{
    this->name = name;
}
void Item::SetIcon(std::string icon)
{
    this->icon = icon;
}
void ItemRegister::RegisterItem(int id, const std::string &name, const std::string &icon)
{
    auto item = Item();
    item.SetID(id);
    item.SetName(name);
    item.SetIcon(icon);
    items[id] = item;
}
Item ItemRegister::GetItem(int id)
{
    return items[id];
}