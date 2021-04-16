#include "Timer.hpp"
# include <iostream>

Timer::Timer()
{
    std::cout << "+++ ctor Timer" << std::endl;
}

Timer::~Timer()
{
    std::cout << "--- destor Timer" << std::endl;
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