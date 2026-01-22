#pragma once

#include "Image.hpp"

struct Resources
{
    Image ichiIdleSheet;

    void LoadFromDisk();

    static Resources& Get();
};