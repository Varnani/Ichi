#include <chrono>
#include <cstdint>
#include <exception>
#include <format>
#include <vector>

#include <rgfw.hpp>

#include "Platform.hpp"
#include "include/Image.hpp"
#include "include/Pixel.hpp"
#include "include/Profiler.hpp"
#include "include/Renderer.hpp"

glm::uvec2 spritePos{ 0, 0 };

static void KeyCallback(RGFW_window* window, RGFW_key key, u8 keyChar, RGFW_keymod keyMod, RGFW_bool repeat, RGFW_bool pressed)
{
    if (pressed == RGFW_FALSE) return;

    if (key == RGFW_escape) RGFW_window_setShouldClose(window, 1);
    if (key == RGFW_p) std::printf("%s", Profiler::Get().GenerateReport().c_str());

    if (key == RGFW_up) spritePos.y += 32;
    if (key == RGFW_down) spritePos.y -= 32;
    if (key == RGFW_right) spritePos.x += 32;
    if (key == RGFW_left) spritePos.x -= 32;
}

static void MouseCallback(RGFW_window* window, RGFW_mouseButton button, RGFW_bool pressed)
{
    ;
}

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
    RGFW_setKeyCallback(KeyCallback);
    RGFW_setMouseButtonCallback(MouseCallback);

    int width = 1280;
    int height = 960;

    RGFW_window* window = RGFW_createWindow("Ichi", 0, 0, width, height, RGFW_windowCenter | RGFW_windowFocusOnShow);
    RGFW_monitor* monitor = RGFW_window_getMonitor(window);
    RGFW_surface* surface = nullptr;

    if (monitor == nullptr) throw std::exception{ "can't query monitor!" };

    Image ichiSheet{};
    ichiSheet.Load("C:\\Users\\ozang\\source\\repos\\Ichi\\res\\ichi-idle.png");

    Sprite ichiFrame{};

    ichiFrame.image = &ichiSheet;
    ichiFrame.startX = 0;
    ichiFrame.startY = 0;
    ichiFrame.width = 16;
    ichiFrame.height = 32;

    Renderer renderer{};

    float deltaTime = 0;
    float passedTime = 0;

    while (RGFW_window_shouldClose(window) == RGFW_FALSE)
    {
        deltaTime = Profiler::Get().GetRootMarker().durationAsSeconds;
        passedTime += deltaTime;

        Profiler::Get().BeginFrame();

        RGFW_pollEvents();
        RGFW_window_getSize(window, &width, &height);

        const float animTime = 0.3;
        if (passedTime > animTime)
        {
            passedTime -= animTime;
            ichiFrame.startX = (ichiFrame.startX + 16) % ichiSheet.size.x;
        }

        surface = GetSurface(monitor, width, height);

        Profiler::Get().BeginMarker("Renderer::Resize");
        {
            renderer.Resize(width, height);
        }
        Profiler::Get().EndMarker();

        Profiler::Get().BeginMarker("Renderer::Clear");
        {
            renderer.Clear(Pixel::Black);
        }
        Profiler::Get().EndMarker();

        Profiler::Get().BeginMarker("Renderer::DrawSprite");
        {
            renderer.DrawSprite(spritePos, ichiFrame);
        }
        Profiler::Get().EndMarker();

        Profiler::Get().BeginMarker("Renderer::Present");
        {
            renderer.Present(surface->data, surface->w, surface->h);
        }
        Profiler::Get().EndMarker();

        Profiler::Get().BeginMarker("RGFW_window_blitSurface");
        {
            RGFW_window_blitSurface(window, surface);
        }
        Profiler::Get().EndMarker();

        Profiler::Get().BeginMarker("RGFW_window_setName");
        {
            RGFW_window_setName(window, std::format("Ichi | {:>3}fps {:>8.4f}ms", int(1.0f / deltaTime), deltaTime).c_str());
        }
        Profiler::Get().EndMarker();

        Profiler::Get().EndFrame();
    }

    if (surface != nullptr) RGFW_surface_free(surface);
    RGFW_window_close(window);

    return 0;
}
