#include <print>
#include <stdio.h>

#include <glm/glm.hpp>
#include <rgfw.hpp>

void keyfunc(RGFW_window* win, RGFW_key key, u8 keyChar, RGFW_keymod keyMod, RGFW_bool repeat, RGFW_bool pressed)
{
    RGFW_UNUSED(repeat);
    if (key == RGFW_escape && pressed)
    {
        RGFW_window_setShouldClose(win, 1);
    }
}

int main()
{
    RGFW_setKeyCallback(keyfunc);

    RGFW_window* window = RGFW_createWindow("Ichi", 0, 0, 640, 480, RGFW_windowCenter | RGFW_windowFocusOnShow);
    RGFW_monitor* monitor = RGFW_window_getMonitor(window);

    if (monitor == nullptr)
    {
        std::printf("can't query monitor.\n");
        return 1;
    }

    // we need our surface to be much larger than window
    int surfaceWidth = monitor->pixelRatio * monitor->mode.w * 2;
    int surfaceHeight = monitor->pixelRatio * monitor->mode.h * 2;

    std::vector<uint32_t> buffer{};
    buffer.resize(surfaceWidth * surfaceHeight);

    uint8_t* bufferPtr = reinterpret_cast<uint8_t*>(buffer.data());

    RGFW_surface* surface = RGFW_createSurface(bufferPtr, surfaceWidth, surfaceHeight, RGFW_formatRGBA8);

    while (RGFW_window_shouldClose(window) == RGFW_FALSE)
    {
        RGFW_event event;
        while (RGFW_window_checkEvent(window, &event))
        {
            if (event.type == RGFW_quit) break;

            i32 mouseX, mouseY;
            RGFW_window_getMouse(window, &mouseX, &mouseY);

            if (event.type == RGFW_mouseButtonPressed && event.button.value == RGFW_mouseLeft)
            {
                printf("You clicked at x: %d, y: %d\n", mouseX, mouseY);
            }

            for (uint32_t x = 0; x < 640; x++)
            {
                for (uint32_t y = 0; y < 480; y++)
                {
                    float fx = float(x);
                    float fy = float(y);


                    uint32_t r = (uint32_t)((fx / 640) * 255);
                    uint32_t g = (uint32_t)((fy / 480) * 255);
                    uint32_t b = 0;
                    uint32_t a = 255;

                    uint32_t pixel = r + (g << 8) + (b << 16) + (a << 24);

                    size_t index = (x + (y * surfaceWidth));
                    buffer[index] = pixel;
                }
            }

            RGFW_window_blitSurface(window, surface);
        }
    }

    RGFW_surface_free(surface);
    RGFW_window_close(window);
}
