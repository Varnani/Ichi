#include "../include/Resources.hpp"

Resources& Resources::Get()
{
    static Resources resources{};
    return resources;
}

void Resources::LoadFromDisk()
{
    ichiIdleSheet.Load("res/ichi-idle.png");
}