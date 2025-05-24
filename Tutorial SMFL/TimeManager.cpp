#include "TimeManager.h"

void TimeManager::StartTurn()
{
    clock.restart();
}

float TimeManager::GetElapsedTime() const
{
    return clock.getElapsedTime().asSeconds();
}

bool TimeManager::IsTurnTimeOver() const
{
    return clock.getElapsedTime().asSeconds() >= turnDuration;
}
