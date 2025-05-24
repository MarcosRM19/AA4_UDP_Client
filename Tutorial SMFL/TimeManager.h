#pragma once
#include <SFML/System.hpp>

#define TIME TimeManager::Instance()

class TimeManager
{
private:
    sf::Clock clock;
    const float turnDuration = 20.0f;

    TimeManager() = default;

    TimeManager(const TimeManager&) = delete;
    TimeManager& operator =(const TimeManager&) = delete;

public:
    inline static TimeManager& Instance()
    {
        static TimeManager tm;
        return tm;
    }

    void StartTurn();
    float GetElapsedTime() const;
    bool IsTurnTimeOver() const; 
};
