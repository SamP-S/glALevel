#pragma once

#include <chrono>

class Timer {
private:
    std::chrono::time_point<std::chrono::steady_clock> startTime;

public:
    void StartTimer() {
        startTime = std::chrono::steady_clock::now();
    }

    float GetTime() const {
        std::chrono::duration<float> duration = std::chrono::steady_clock::now() - startTime;
        return duration.count();
    }
};
