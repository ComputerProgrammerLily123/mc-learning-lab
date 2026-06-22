#pragma once
#include <unordered_map>
#include <vector>
#include <functional>
namespace GUI
{
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
        void Subscribe(Event event,std::function<void()> eventHandler);
        void UnSubscribe(Event event,unsigned long index);
    private:
        static unsigned long subscribeIndex;
        std::unordered_map<Event,std::vector<std::function<void()>>> subscribers;
    };
}