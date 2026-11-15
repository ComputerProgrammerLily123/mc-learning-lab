#include "timeSystem.h"

#include <algorithm>
#include <cmath>
#include "GLFW/glfw3.h"
void TimeSystem::UpdateTime()
{
    lastFrameTime = currentFrameTime;
    currentFrameTime = GetCurrentTime();
}
double TimeSystem::GetCurrentTime()
{
    return glfwGetTime();
}
double TimeSystem::GetDeltaTime()
{
    return currentFrameTime - lastFrameTime;
}

void TickSystem::UpdateTimer()
{
    const double currentTime = glfwGetTime();
    if (!initialized)
    {
        lastTime = currentTime;
        initialized = true;
        return;
    }
    double deltaTime = currentTime - lastTime;
    if (deltaTime > 0.25)
        deltaTime = 0.25;
    accumulator += deltaTime;
    lastTime = currentTime;
}
bool TickSystem::ShouldTick()
{
    return accumulator >= TICK_TIME;
}
void TickSystem::ConsumeTick()
{
    accumulator -= TICK_TIME;
}
void TickSystem::DiscardBacklog()
{
    accumulator = std::fmod(accumulator, TICK_TIME);
}
float TickSystem::GetAlpha() const
{
     return std::clamp(static_cast<float>(accumulator / TICK_TIME), 0.0f, 1.0f);
}