#pragma once
#include <functional>

class Timer
{

public:
    Timer();
    ~Timer();

    void update(const float delta);
    void start(const float duration);
    void setCallback(std::function<void()> callback);

private:
    std::function<void()> callback;
    double                timeLeft  = 0;
    bool                  isRunning = false;
};