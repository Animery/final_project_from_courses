#include "Timer.hpp"
#include "debug_level.hpp"

#ifdef DEBUG_LEVEL
#include <iostream>
#endif

Timer::Timer()
{
#ifdef DEBUG_LEVEL
    std::cout << "+++ ctor Timer" << std::endl;
#endif
}

Timer::~Timer()
{
#ifdef DEBUG_LEVEL
    std::cout << "--- destor Timer" << std::endl;
#endif
}

void Timer::update_timer(const float delta)
{
    if (isRunning)
    {
        timeLeft -= delta;
        if (timeLeft <= 0)
        {
            isRunning = false;
            callback();
        }
    }
}

void Timer::start(const float duration)
{
    timeLeft  = duration;
    isRunning = true;
}

void Timer::setCallback(std::function<void()> _callback)
{
    callback = _callback;
}