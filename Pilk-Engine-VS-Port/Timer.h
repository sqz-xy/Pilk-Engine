#pragma once
#include "PilkEngineCommon.h"

/// <summary>
/// Original Author: Thomas Beet, followed tutorial at: https://www.youtube.com/watch?v=9kEBbKIsmz0
/// Timer class for cooldowns, etc...
/// </summary>
class Timer {
public:
    Timer() 
    {
        m_startTime = m_currentTime = m_lastDeltaTick = std::chrono::system_clock::now();
        m_deltaTime = std::chrono::duration<float>(0);
    }

    // restarts the start time to now, which basically resets the elapsed time count
    void Restart() 
    {
        m_startTime = m_currentTime = m_lastDeltaTick = std::chrono::system_clock::now();
    }

    // returns the amount of time since the last call to this function
    float GetDeltaTime() 
    {
        m_currentTime = std::chrono::system_clock::now();
        m_deltaTime = m_currentTime - m_lastDeltaTick;
        m_lastDeltaTick = m_currentTime;
        return m_deltaTime.count();
    }

    // returns the amount of time since last Restart
    float GetElapsedTime() 
    {
        m_currentTime = std::chrono::system_clock::now();
        std::chrono::duration<float> elapsed_time = m_currentTime - m_startTime;
        return elapsed_time.count();
    }
private:
    std::chrono::system_clock::time_point m_startTime, m_currentTime, m_lastDeltaTick;
    std::chrono::duration<float> m_deltaTime;
};
