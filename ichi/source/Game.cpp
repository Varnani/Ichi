#include "../include/Game.hpp"
#include "../include/Entity.hpp"
#include "../include/Input.hpp"
#include "../include/KeyCodes.hpp"
#include "../include/Profiler.hpp"
#include "../include/Resources.hpp"
#include "../include/Tile.hpp"
#include "../include/Time.hpp"

void AnimatePlayer(Sprite& playerSprite)
{
    static float passedTime = 0;
    static const float frameTime = 0.3;

    passedTime += Time::Get().deltaTime;

    if (passedTime < frameTime) return;

    passedTime -= frameTime;

    playerSprite.startX += playerSprite.width;
    playerSprite.startX %= playerSprite.image->size.x;
};

Game& Game::Get()
{
    static Game game{};
    return game;
}

size_t playerIndex;
void Game::Initialize()
{
    Entity player{};
    player.sprite = Resources::Get().sprites.ichiIdle;

    entities.push_back(player);
    playerIndex = entities.size() - 1;

    for (int x = 0; x < 20; x++)
    {
        for (int y = 0; y < 20; y++)
        {
            Tile tile{};
            tile.coords.x = x - 10;
            tile.coords.y = y - 10;

            if (x == 0 || x == 19) tile.type = TileType::TestWall;
            else if (y == 0 || y == 19) tile.type = TileType::TestWall;
            else tile.type = TileType::TestGround;

            tiles.push_back(tile);
        }
    }
}

void Game::Update()
{
    Profiler::Get().BeginMarker("Game::Update");

    Input& input = Input::Get();
    Entity& player = entities[playerIndex];

    if (input.IsKeyDown(KeyCode::ShiftL))
    {
        if (input.IsKeyDown(KeyCode::Right)) camera.x += 1;
        if (input.IsKeyDown(KeyCode::Left)) camera.x -= 1;
        if (input.IsKeyDown(KeyCode::Up)) camera.y += 1;
        if (input.IsKeyDown(KeyCode::Down)) camera.y -= 1;
    }

    else
    {
        glm::ivec2 playerPos = player.coords;

        if (input.IsKeyPressed(KeyCode::Right)) playerPos.x += 1;
        if (input.IsKeyPressed(KeyCode::Left)) playerPos.x -= 1;
        if (input.IsKeyPressed(KeyCode::Up)) playerPos.y += 1;
        if (input.IsKeyPressed(KeyCode::Down)) playerPos.y -= 1;

        player.MoveTo(playerPos);
    }

    AnimatePlayer(player.sprite);

    Profiler::Get().EndMarker();
}