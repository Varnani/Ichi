#include "../include/Tile.hpp"
#include "../include/Game.hpp"

glm::vec2 Tile::GetPosition()
{
    return glm::vec2{this->coords.x * Game::GRID_SIZE, this->coords.y * Game::GRID_SIZE};
}