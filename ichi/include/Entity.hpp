#pragma once

#include "glm/glm.hpp"
#include "Sprite.hpp"

struct Entity
{
    glm::ivec2 position;
    Sprite sprite;
};