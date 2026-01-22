#include <chrono>
#include <cstdint>
#include <exception>
#include <format>
#include <vector>

#include <rgfw.hpp>

#include "Platform.hpp"
#include "include/Game.hpp"
#include "include/Input.hpp"
#include "include/Pixel.hpp"
#include "include/Profiler.hpp"
#include "include/Renderer.hpp"
#include "include/Resources.hpp"
#include "include/Time.hpp"

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

    if (monitor == nullptr) throw std::exception{ "can't query monitor!" };

    Profiler& profiler = Profiler::Get();
    Renderer& renderer = Renderer::Get();
    Resources& resources = Resources::Get();
    Input& input = Input::Get();
    Time& time = Time::Get();
    Game& game = Game::Get();

    input.RegisterCallbacks();
    resources.LoadFromDisk();

    while (RGFW_window_shouldClose(window) == RGFW_FALSE)
    {
        float dt = profiler.GetRootMarker().durationAsSeconds;
        time.Update(dt);

        profiler.BeginFrame();

        input.UpdateKeyStates();
        RGFW_pollEvents();

        game.Update();

        RGFW_window_getSize(window, &width, &height);
        surface = GetSurface(monitor, width, height);

        renderer.Resize(width, height);
        renderer.Clear(Pixel::Black);
        renderer.Present(surface->data, surface->w, surface->h);

        RGFW_window_blitSurface(window, surface);
        RGFW_window_setName(window, std::format("Ichi | {:>3}fps {:>8.4f}ms", int(1.0f / time.deltaTime), time.deltaTime).c_str());

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
