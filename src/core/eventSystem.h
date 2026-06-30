#pragma once
#include <unordered_map>
#include <vector>
#include <functional>

enum class Event
{
    OnPointerEnter,
    OnPointerStay,
    OnPointerExit,
    OnPointerClick
};
class EventSystem
{
public:
    EventSystem(const EventSystem &) = delete;
    EventSystem &operator=(const EventSystem &) = delete;
    static EventSystem &GetInstance()
    {
        static EventSystem instance;
        return instance;
    }
    void Subscribe(Event event, std::function<void()> eventHandler);
    void UnSubscribe(Event event, unsigned long index);

private:
    EventSystem() = default;
    ~EventSystem() = default;
    static unsigned long subscribeIndex;
    std::unordered_map<Event, std::vector<std::function<void()>>> subscribers;
};
