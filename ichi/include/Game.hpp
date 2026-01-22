#pragma once

#include <glm/glm.hpp>

struct Game
{
    glm::ivec2 camera;

    void Update();

    static Game& Get();
};