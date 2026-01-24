#pragma once

#include <glm/glm.hpp>
#include "Entity.hpp"
#include "Tile.hpp"

struct Game
{
    static constexpr int GRID_SIZE = 32;

    glm::ivec2 camera;
    std::vector<Entity> entities;
    std::vector<Tile> tiles;

    void Initialize();
    void Update();

    static Game& Get();
};