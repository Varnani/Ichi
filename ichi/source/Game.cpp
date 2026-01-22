#include "../include/Game.hpp"

Game& Game::Get()
{
    static Game game{};
    return game;
}

void Game::Update()
{
    ;
}