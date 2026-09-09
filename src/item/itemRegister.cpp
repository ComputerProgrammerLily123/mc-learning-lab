#include "itemRegister.h"
void ItemRegister::RegisterItem(const std::string &nameSpace, const std::string &name)
{
    items.emplace(ResourceLocation(nameSpace,name),ItemBase());
}
void ItemRegister::RegisterItem(const std::string &name)
{
    items.emplace(ResourceLocation(name),ItemBase());
}