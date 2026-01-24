#pragma once

#include "Image.hpp"
#include "Sprite.hpp"

struct Resources
{
    struct Images
    {
        Image ichiIdleSheet;
        Image testTile;
    };

    struct Sprites
    {
        Sprite ichiIdle;
        Sprite testTile;
    };

    Images images;
    Sprites sprites;

    void LoadFromDisk();

    static Resources& Get();
};