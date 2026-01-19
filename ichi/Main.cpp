#include <cstdint>
#include <exception>
#include <vector>

#include <rgfw.hpp>

#include "Platform.hpp"
#include "include/Pixel.hpp"
#include "include/Renderer.hpp"

static void KeyCallback(RGFW_window* window, RGFW_key key, u8 keyChar, RGFW_keymod keyMod, RGFW_bool repeat, RGFW_bool pressed)
{
    if (key == RGFW_escape && pressed) RGFW_window_setShouldClose(window, 1);
}

static void MouseCallback(RGFW_window* window, RGFW_mouseButton button, RGFW_bool pressed)
{
    if (pressed == RGFW_FALSE) return;
    int mouseX, mouseY;
    RGFW_window_getMouse(window, &mouseX, &mouseY);
    std::printf("You clicked at x: %d, y: %d\n", mouseX, mouseY);
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

    Renderer renderer{};

    while (RGFW_window_shouldClose(window) == RGFW_FALSE)
    {
        RGFW_pollEvents();
        RGFW_window_getSize(window, &width, &height);

        surface = GetSurface(monitor, width, height);

        renderer.Resize(width, height);
        renderer.Clear(Pixel::Red);
        renderer.DrawRect({ 100, 100 }, { 250, 250 }, Pixel::Blue);
        renderer.Present(surface->data, surface->w, surface->h);

        RGFW_window_blitSurface(window, surface);
    }

    if (surface != nullptr) RGFW_surface_free(surface);
    RGFW_window_close(window);

    return 0;
}
