#pragma once
#include <functional>

class Timer {

public:
    Timer();
    ~Timer();
   
    void update(const double delta);
    void start(const double duration);
    void setCallback(std::function<void()> callback);

private:
    std::function<void()> callback;
    double timeLeft;
    bool isRunning;
};