#pragma once

#include "Image.hpp"
#include "Sprite.hpp"
#include "Tile.hpp"

struct Resources
{
    struct Images
    {
        Image ichiIdleSheet;
        Image testGround;
        Image testWall;
    };

    struct Sprites
    {
        Sprite ichiIdle;
        Sprite testGround;
        Sprite testWall;
    };

    Images images;
    Sprites sprites;

    void LoadFromDisk();
    Sprite GetTileSprite(TileType type);

    static Resources& Get();
};