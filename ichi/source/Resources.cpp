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
    images.testGround.Load("res/test-ground.png");
    images.testWall.Load("res/test-wall.png");

    sprites.ichiIdle = Sprite{ .image = &images.ichiIdleSheet, .startX = 0, .startY = 0, .width = 16, .height = 32 };
    sprites.testGround = Sprite{ .image = &images.testGround, .startX = 0, .startY = 0, .width = 32, .height = 32 };
    sprites.testWall = Sprite{ .image = &images.testWall, .startX = 0, .startY = 0, .width = 32, .height = 32 };
}

Sprite Resources::GetTileSprite(TileType type)
{
    switch (type)
    {
    case TileType::TestGround:
        return sprites.testGround;
    case TileType::TestWall:
        return sprites.testWall;
    }

    return Sprite{};
}