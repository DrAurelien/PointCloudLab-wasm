#pragma once

#include <chrono>

namespace Tools
{
class Timer
{
public:
    Timer() : m_StartTime(std::chrono::high_resolution_clock::now()) {}

    void Start() { m_StartTime = std::chrono::high_resolution_clock::now(); }

    template<typename durationtype = std::chrono::milliseconds>
    auto Elapsed() const
    {
        auto endTime = std::chrono::high_resolution_clock::now();
        durationtype duration = std::chrono::duration_cast<durationtype>(endTime - m_StartTime);
        return duration.count();
    }

private:
    std::chrono::time_point<std::chrono::high_resolution_clock> m_StartTime;
};
}