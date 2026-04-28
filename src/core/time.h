#ifndef TIME_H
#define TIME_H

class TimeSystem
{
public:
    TimeSystem(const TimeSystem &) = delete;
    TimeSystem &operator=(const TimeSystem &) = delete;
    static TimeSystem &GetInstance()
    {
        static TimeSystem instance;
        return instance;
    }
    void UpdateTime();
    double GetCurrentTime();
    double GetDeltaTime();

private:
    TimeSystem() = default;
    ~TimeSystem() = default;
    double lastFrameTime;
    double currentFrameTime;
};
class TickSystem
{
public:
    TickSystem(const TickSystem &) = delete;
    TickSystem &operator=(const TickSystem &) = delete;
    static TickSystem &GetInstance()
    {
        static TickSystem instance;
        return instance;
    }
    void UpdateTimer();
    bool ShouldTick();
    void ConsumeTick();
    float GetAlpha() const;

private:
    TickSystem() = default;
    ~TickSystem() = default;

    static constexpr double TICK_RATE = 20.0;
    static constexpr double TICK_TIME = 1.0 / TICK_RATE;

    float accumulator = 0.0f;
    float lastTime = 0.0f;
};
#endif