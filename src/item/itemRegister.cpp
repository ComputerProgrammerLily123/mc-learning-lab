#include "itemRegister.h"
void ItemRegister::RegisterItem(int id, const std::string &name, const std::string &icon)
{
    auto item = ItemBase();
    item.SetID(id);
    item.SetName(name);
    item.SetIcon(icon);
    items[id] = item;
}
ItemBase ItemRegister::GetItem(int id)
{
    return items[id];
}