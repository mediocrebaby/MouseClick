#pragma once
#include <atomic>
#include <chrono>

class MouseClick
{
public:
    MouseClick() = default;
    ~MouseClick() = default;

    void start(int durationSeconds, int intervalMilliSeconds);
    void click();
    void stop() { isRunning = false; }

private:
    std::atomic<bool> isRunning{false};
};