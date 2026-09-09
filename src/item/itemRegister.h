#pragma once
#include "itemBase.h"
#include "resource/resourceLocation.h"
#include <unordered_map>
class ItemRegister
{
public:
    static ItemRegister &GetInstance()
    {
        static ItemRegister instance;
        return instance;
    }
    ItemRegister(ItemRegister&&) = delete;
    ItemRegister& operator=(ItemRegister&&) = delete;
    ItemRegister(const ItemRegister&) = delete;
    ItemRegister &operator=(const ItemRegister &) = delete;
    void RegisterItem(const std::string &nameSpace, const std::string &name);
    void RegisterItem(const std::string &name);

private:
    ItemRegister() = default;
    ~ItemRegister() = default;
    std::unordered_map<ResourceLocation, ItemBase> items;
};