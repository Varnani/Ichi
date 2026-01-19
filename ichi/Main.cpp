#include <cstdint>
#include <exception>
#include <vector>

#include <rgfw.hpp>

#include "Platform.hpp"

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

static RGFW_surface* UpdateSurface(RGFW_window* window)
{
    static std::vector<uint32_t> buffer{};
    static RGFW_monitor* monitor = RGFW_window_getMonitor(window);
    static RGFW_surface* surface = nullptr;

    if (monitor == nullptr) throw std::exception{ "can't query monitor!" };

    int width, height;
    RGFW_window_getSize(window, &width, &height);

    int surfaceWidth = monitor->pixelRatio * width;
    int surfaceHeight = monitor->pixelRatio * height;

    if (surface != nullptr)
    {
        if (surfaceWidth == surface->w && surfaceHeight == surface->h) return surface;
        RGFW_surface_free(surface);
    }

    std::printf("surface resized, w: %d, h: %d\n", surfaceWidth, surfaceHeight);

    buffer.resize(surfaceWidth * surfaceHeight);
    uint8_t* bufferPtr = reinterpret_cast<uint8_t*>(buffer.data());
    surface = RGFW_createSurface(bufferPtr, surfaceWidth, surfaceHeight, RGFW_formatRGBA8);

    return surface;
}

int AppEntry()
{
    RGFW_setKeyCallback(KeyCallback);
    RGFW_setMouseButtonCallback(MouseCallback);

    RGFW_window* window = RGFW_createWindow("Ichi", 0, 0, 1280, 960, RGFW_windowCenter | RGFW_windowFocusOnShow);
    RGFW_surface* surface = UpdateSurface(window);

    while (RGFW_window_shouldClose(window) == RGFW_FALSE)
    {
        RGFW_pollEvents();

        surface = UpdateSurface(window);

        /*for (uint32_t x = 0; x < 640; x++)
        {
            for (uint32_t y = 0; y < 480; y++)
            {
                float fx = float(x);
                float fy = float(y);

                uint8_t r = (uint32_t)((fx / 640) * 255);
                uint8_t g = (uint32_t)((fy / 480) * 255);
                uint8_t b = 0;
                uint8_t a = 255;

                uint32_t pixel = r + (g << 8) + (b << 16) + (a << 24);

                size_t index = (x + (y * 640));
                buffer[index] = pixel;
            }
        }

        RGFW_window_blitSurface(window, s_surface);*/
    }

    if (surface != nullptr) RGFW_surface_free(surface);
    RGFW_window_close(window);

    return 0;
}
