#include "../include/Entity.hpp"
#include "../include/Game.hpp"

void Entity::SetCoords(glm::ivec2 coords)
{
    this->coords = coords;
    this->spritePosition.x = coords.x * Game::GRID_SIZE;
    this->spritePosition.y = coords.y * Game::GRID_SIZE;
}

void Entity::MoveTo(glm::ivec2 coords) 
{
    if (this->coords == coords) return;
    SetCoords(coords);
}