#pragma once

#include <cstdint>

struct Time
{
    float totalTime;
    float deltaTime;
    uint64_t frameCounter;

    void Update(float dt);

    static Time& Get();
};