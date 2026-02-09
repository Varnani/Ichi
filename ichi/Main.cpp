#include <cstdint>
#include <exception>
#include <format>
#include <rgfw.hpp>
#include <stdexcept>
#include <vector>

#include "Platform.hpp"
#include "include/Game.hpp"
#include "include/Input.hpp"
#include "include/Pixel.hpp"
#include "include/Profiler.hpp"
#include "include/Renderer.hpp"
#include "include/Resources.hpp"
#include "include/Time.hpp"
#include "include/Entity.hpp"
#include "include/Coroutines.hpp"

static void RenderEntities(int screenWidth, int screenHeight);
static void RenderTiles(int screenWidth, int screenHeight);

static RGFW_surface* GetSurface(RGFW_monitor* monitor, int width, int height)
{
    static std::vector<uint32_t> buffer{};
    static RGFW_surface* surface = nullptr;

    int surfaceWidth = monitor->pixelRatio * width;
    int surfaceHeight = monitor->pixelRatio * height;

    if (surface != nullptr)
    {
        if (surfaceWidth == surface->w && surfaceHeight == surface->h) return surface;
        RGFW_surface_free(surface);
    }

    std::printf("surface resized, w: %d, h: %d\n", surfaceWidth, surfaceHeight);

    buffer.resize(surfaceWidth * surfaceHeight);
    u8* bufferPtr = reinterpret_cast<u8*>(buffer.data());
    surface = RGFW_createSurface(bufferPtr, surfaceWidth, surfaceHeight, RGFW_formatRGBA8);

    return surface;
}

int AppEntry()
{
    int width = 1280;
    int height = 960;

    RGFW_window* window = RGFW_createWindow("Ichi", 0, 0, width, height, RGFW_windowCenter | RGFW_windowFocusOnShow);
    RGFW_monitor* monitor = RGFW_window_getMonitor(window);
    RGFW_surface* surface = nullptr;

    if (monitor == nullptr) throw std::runtime_error{ "can't query monitor!" };

    Profiler& profiler = Profiler::Get();
    Renderer& renderer = Renderer::Get();
    Resources& resources = Resources::Get();
    Input& input = Input::Get();
    Time& time = Time::Get();
    Coroutines& coroutines = Coroutines::Get();
    Game& game = Game::Get();

    input.RegisterCallbacks();
    resources.LoadFromDisk();
    game.Initialize();

    while (RGFW_window_shouldClose(window) == RGFW_FALSE)
    {
        float dt = profiler.GetRootMarker().durationAsSeconds;
        time.Update(dt);

        profiler.BeginFrame();

        input.UpdateKeyStates();
        RGFW_pollEvents();

        coroutines.ProcessCoroutines();
        game.Update();

        RGFW_window_getSize(window, &width, &height);
        surface = GetSurface(monitor, width, height);

        profiler.BeginMarker("Rendering");
        {
            renderer.Resize(width, height);
            renderer.Clear(Pixel::Black);

            RenderTiles(width, height);
            RenderEntities(width, height);

            renderer.Present(surface->data, surface->w, surface->h);
        }
        profiler.EndMarker();

        profiler.BeginMarker("RGFW_window_blitSurface");
        {
            RGFW_window_blitSurface(window, surface);
        }
        profiler.EndMarker();

        RGFW_window_setName(window, std::format("Ichi | {:>3}fps {:>8.4f}ms", int(1.0f / time.deltaTime), time.deltaTime * 1000).c_str());

        profiler.EndFrame();

        if (input.IsKeyPressed(KeyCode::P))
        {
            std::printf("%s\n", profiler.GenerateReport().c_str());
        }
    }

    if (surface != nullptr) RGFW_surface_free(surface);
    RGFW_window_close(window);

    return 0;
}

static glm::ivec2 GameToScreenCoords(glm::vec2 halfScreen, glm::vec2 gamePos, glm::vec2 camera)
{
    glm::vec2 pos = gamePos - camera;
    pos.y = -pos.y;
    pos += halfScreen;
    return pos;
}

static void RenderEntities(int screenWidth, int screenHeight)
{
    Profiler::Get().BeginMarker("RenderEntities");

    Game& game = Game::Get();
    Renderer& renderer = Renderer::Get();

    glm::vec2 halfScreen{ (float)screenWidth / 2.0, (float)screenHeight / 2.0 };

    for (size_t i = 0; i < game.entities.size(); i++)
    {
        Entity& entity = game.entities[i];

        glm::ivec2 screenPos = GameToScreenCoords(halfScreen, entity.spritePosition, game.camera);

        renderer.DrawSprite(screenPos, entity.sprite);
    }

    Profiler::Get().EndMarker();
}

static void RenderTiles(int screenWidth, int screenHeight)
{
    Profiler::Get().BeginMarker("RenderTiles");

    Game& game = Game::Get();
    Renderer& renderer = Renderer::Get();

    glm::ivec2 halfScreen{ screenWidth / 2, screenHeight / 2 };

    for (size_t i = 0; i < game.tiles.size(); i++)
    {
        Tile& tile = game.tiles[i];

        glm::ivec2 screenPos = GameToScreenCoords(halfScreen, tile.GetPosition(), game.camera);

        renderer.DrawSprite(screenPos, tile.sprite);
    }

    Profiler::Get().EndMarker();
}
