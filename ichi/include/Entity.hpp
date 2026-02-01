#pragma once

#include "glm/glm.hpp"
#include "Sprite.hpp"

struct Entity
{
    glm::vec2 spritePosition;
    glm::ivec2 coords;
    Sprite sprite;

    void SetCoords(glm::ivec2 coords);
    void MoveTo(glm::ivec2 coords);
};

