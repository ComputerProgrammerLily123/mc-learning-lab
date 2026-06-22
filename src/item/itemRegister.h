#pragma once
#include "itemBase.h"
#include <unordered_map>
class ItemRegister
{
public:
    static ItemRegister &GetInstance()
    {
        static ItemRegister instance;
        return instance;
    }
    ItemRegister(const ItemRegister &) = delete;
    ItemRegister &operator=(const ItemRegister &) = delete;
    void RegisterItem(int id, const std::string &name, const std::string &icon);
    ItemBase GetItem(int id);

private:
    ItemRegister() = default;
    ~ItemRegister() = default;
    std::unordered_map<int,ItemBase> items;
};