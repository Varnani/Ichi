#include "../include/Resources.hpp"
#include "../include/Sprite.hpp"

Resources& Resources::Get()
{
    static Resources resources{};
    return resources;
}

void Resources::LoadFromDisk()
{
    images.ichiIdleSheet.Load("res/ichi-idle.png");
    images.testTile.Load("res/test-tile.png");

    sprites.ichiIdle = Sprite{ .image = &images.ichiIdleSheet, .startX = 0, .startY = 0, .width = 16, .height = 32 };
    sprites.testTile = Sprite{ .image = &images.testTile, .startX = 0, .startY = 0, .width = 32, .height = 32 };
}