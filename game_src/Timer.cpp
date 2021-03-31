#include "Timer.hpp"
# include <iostream>

Timer::Timer()
    : isRunning(false)
    , timeLeft(0)
{
    std::cout << "+++ ctor Timer" << std::endl;
}

Timer::~Timer()
{
    std::cout << "--- destor Timer" << std::endl;
}

void Timer::update(const double delta)
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

void Timer::start(const double duration)
{
    timeLeft  = duration;
    isRunning = true;
}

void Timer::setCallback(std::function<void()> _callback)
{
    callback = _callback;
}