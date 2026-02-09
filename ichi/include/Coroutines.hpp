#pragma once
#include <cstdint>
#include <memory>
#include <vector>

#include <glm/glm.hpp>

#include "Entity.hpp"

enum class CoroutineResult : uint8_t
{
    Yield = 0,
    Stop = 1,
};

template <typename CoroutineContext>
using CoroutineFunction = CoroutineResult(*)(CoroutineContext&);

class Coroutines
{
    class CoroutineBase
    {
    public:
        virtual CoroutineResult Evaluate() = 0;
        virtual ~CoroutineBase() = default;
    };

    template <typename CoroutineContext>
    class Coroutine : public CoroutineBase
    {
    public:
        CoroutineFunction<CoroutineContext> function;
        CoroutineContext context;

        Coroutine(CoroutineFunction<CoroutineContext> function, CoroutineContext context);

        CoroutineResult Evaluate() override;
    };

public:
    static Coroutines& Get();

    template <typename CoroutineContext>
    void StartCoroutine(CoroutineFunction<CoroutineContext> coroutine, CoroutineContext context);

    void ProcessCoroutines();

private:
    std::vector<std::unique_ptr<CoroutineBase>> coroutines;
};

// coroutine contexts

struct SpriteMover_Ctx
{
    Entity* entity;
    glm::vec2 from;
    glm::vec2 to;
    float duration;
    float elapsed;
};