#pragma once

#include "glm/glm.hpp"
#include "Sprite.hpp"

struct Tile
{
    glm::ivec2 coords;
    Sprite sprite;

    glm::vec2 GetPosition();
};