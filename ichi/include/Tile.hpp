#pragma once

#include "glm/glm.hpp"
#include "Sprite.hpp"

enum class TileType : uint8_t
{
    TestGround = 0,
    TestWall
};

struct Tile
{
    glm::ivec2 coords;
    TileType type;

    glm::vec2 GetPosition();
    Sprite GetSprite();
};