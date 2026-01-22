#include "../include/Time.hpp"

Time& Time::Get()
{
    static Time time{};
    return time;
}

void Time::Update(float dt)
{
    deltaTime = dt;
    totalTime += dt;
    frameCounter++;
}
