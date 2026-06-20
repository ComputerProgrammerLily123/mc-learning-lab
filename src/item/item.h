#pragma once
#include<string>
#include<unordered_map>
class Item
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
    Item GetItem(int id);

private:
    ItemRegister() = default;
    ~ItemRegister() = default;
    std::unordered_map<int,Item> items;
};