#include "../include/Coroutines.hpp"
#include "../include/Profiler.hpp"

Coroutines& Coroutines::Get()
{
    static Coroutines coroutines{};
    return coroutines;
}

template <typename CoroutineContext>
Coroutines::Coroutine<CoroutineContext>::Coroutine(CoroutineFunction<CoroutineContext> function, CoroutineContext context) 
    : function(function), context(std::move(context)) { }

template <typename CoroutineContext>
CoroutineResult Coroutines::Coroutine<CoroutineContext>::Evaluate()
{
    return function(context);
}

template <typename CoroutineContext>
void Coroutines::StartCoroutine(CoroutineFunction<CoroutineContext> function, CoroutineContext context)
{
    coroutines.emplace_back(std::make_unique<Coroutine<CoroutineContext>>(function, context));
}

void Coroutines::ProcessCoroutines()
{
    Profiler::Get().BeginMarker("Coroutines::ProcessCoroutines");

    for (size_t i = 0; i < coroutines.size();)
    {
        CoroutineBase* coroutine = coroutines[i].get();
        CoroutineResult status = coroutine->Evaluate();

        if (status == CoroutineResult::Stop) coroutines.erase(coroutines.begin() + i);
        else i++;
    }

    Profiler::Get().EndMarker();
}

// explicit instantiations

template void Coroutines::StartCoroutine<SpriteMover_Ctx>(CoroutineFunction<SpriteMover_Ctx>, SpriteMover_Ctx);