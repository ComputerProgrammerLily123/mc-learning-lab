#pragma once
class TimeSystem
{
public:
    TimeSystem(TimeSystem&&) = delete;
    TimeSystem& operator=(TimeSystem&&) = delete;
    TimeSystem(const TimeSystem&) = delete;
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
    TickSystem(TickSystem&&) = delete;
    TickSystem& operator=(TickSystem&&) = delete;
    TickSystem(const TickSystem&) = delete;
    TickSystem &operator=(const TickSystem &) = delete;
    static TickSystem &GetInstance()
    {
        static TickSystem instance;
        return instance;
    }
    void UpdateTimer();
    bool ShouldTick();
    void ConsumeTick();
    [[nodiscard]] float GetAlpha() const;

private:
    TickSystem() = default;
    ~TickSystem() = default;

    static constexpr double TICK_RATE = 20.0;
    static constexpr double TICK_TIME = 1.0 / TICK_RATE;

    double accumulator = 0.0;
    double lastTime = 0.0;
};