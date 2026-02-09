#include "../include/Entity.hpp"
#include "../include/Coroutines.hpp"
#include "../include/Game.hpp"
#include "../include/Time.hpp"

#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtx/compatibility.hpp>

CoroutineResult SpriteMover(SpriteMover_Ctx& context)
{
    context.elapsed += Time::Get().deltaTime;

    float t = context.elapsed / context.duration;
    t = glm::saturate(1 - t);
    t *= t;
    t = 1 - t;

    glm::vec2 pos = glm::lerp(context.from, context.to, t);
    context.entity->spritePosition = pos;

    if (context.elapsed < context.duration) return CoroutineResult::Yield;
    else return CoroutineResult::Stop;
}

void Entity::SetCoords(glm::ivec2 coords)
{
    this->coords = coords;
    this->spritePosition.x = coords.x * Game::GRID_SIZE;
    this->spritePosition.y = coords.y * Game::GRID_SIZE;
}

void Entity::MoveTo(glm::ivec2 coords)
{
    if (this->coords == coords) return;
    this->coords = coords;

    glm::vec2 targetPos{ coords.x * Game::GRID_SIZE, coords.y * Game::GRID_SIZE };

    Coroutines::Get().StartCoroutine(SpriteMover, { .entity = this, .from = this->spritePosition, .to = targetPos, .duration = 0.25, .elapsed = 0 });
}