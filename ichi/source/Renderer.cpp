#include <cassert>
#include <cstdint>
#include <string.h>

#include <glm/glm.hpp>

#include "../include/Pixel.hpp"
#include "../include/Profiler.hpp"
#include "../include/Renderer.hpp"
#include "../include/Sprite.hpp"

static size_t CalculateIndex(size_t x, size_t y, uint32_t width)
{
    return x + (y * width);
}

Renderer& Renderer::Get()
{
    static Renderer renderer{};
    return renderer;
}

/// <summary>
/// Resizes the internal buffer if needed, no-op otherwise. Does not clear.
/// </summary>
/// <param name="w">New width</param>
/// <param name="h">New height</param>
void Renderer::Resize(const uint32_t w, const uint32_t h)
{
    Profiler::Get().BeginMarker("Renderer::Resize");

    if (w == width && h == height)
    {
        Profiler::Get().EndMarker();
        return;
    }

    width = w;
    height = h;

    m_buffer.resize(width * height);

    Profiler::Get().EndMarker();
}

/// <summary>
/// Clears the buffer with given pixel value.
/// </summary>
/// <param name="color">Color to fill.</param>
void Renderer::Clear(const Pixel color)
{
    Profiler::Get().BeginMarker("Renderer::Clear");

    size_t count = m_buffer.size();
    Pixel* ptr = m_buffer.data();

    while (count > 0)
    {
        *ptr = color;
        ptr++;
        count--;
    }

    Profiler::Get().EndMarker();
}

/// <summary>
/// Draws a colored, filled rect to the screen.
/// </summary>
/// <param name="position">Top left corner of the rect.</param>
/// <param name="size">Size of the rect.</param>
/// <param name="color">Color of the rect.</param>
void Renderer::DrawRect(const glm::ivec2 position, const glm::ivec2 size, const Pixel color)
{
    for (int y = position.y; y < position.y + size.y; y++)
    {
        if (y >= height) continue;
        if (y < 0) continue;

        for (int x = position.x; x < position.x + size.x; x++)
        {
            if (x >= width) continue;
            if (x < 0) continue;

            size_t index = CalculateIndex(x, y, width);
            m_buffer[index] = color;
        }
    }
}

/// <summary>
/// Draws a sprite, centered in a given screen position.
/// </summary>
/// <param name="position">Screen position. Note that sprite will be centered.</param>
/// <param name="sprite">Sprite to draw. If it's Image* is null, it will be skipped.</param>
void Renderer::DrawSprite(const glm::ivec2 position, const Sprite& sprite)
{
    if (sprite.image == nullptr) return;

    int xOffset = sprite.width / 2;
    int yOffset = sprite.height / 2;

    for (int spriteY = 0; spriteY < sprite.height; spriteY++)
    {
        int bufY = spriteY + position.y - yOffset;
        if (bufY >= height) continue;
        if (bufY < 0) continue;

        for (int spriteX = 0; spriteX < sprite.width; spriteX++)
        {
            int bufX = spriteX + position.x - xOffset;
            if (bufX >= width) continue;
            if (bufX < 0) continue;

            size_t index = CalculateIndex(bufX, bufY, width);

            Pixel spritePixel = sprite.GetPixel(spriteX, spriteY);
            if (spritePixel.a > 0) m_buffer[index] = spritePixel;
        }
    }
}

/// <summary>
/// Blits the internal buffer to given target.
/// If sizes match, does a memcpy.
/// If not, does a nearest-neighbor resize by 2x (retina screens).
/// </summary>
/// <param name="target">Target buffer pointer</param>
/// <param name="targetWidth">Target width</param>
/// <param name="targetHeight">Target height</param>
void Renderer::Present(uint8_t* target, const uint32_t targetWidth, const uint32_t targetHeight)
{
    Profiler::Get().BeginMarker("Renderer::Present");

    if (targetWidth == width && targetHeight == height)
    {
        std::memcpy((void*)target, (void*)m_buffer.data(), sizeof(Pixel) * m_buffer.size());
        Profiler::Get().EndMarker();
        return;
    }

    assert(targetWidth == 2 * width && targetHeight == 2 * height);

    Pixel* sourceBuffer = m_buffer.data();
    Pixel* targetBuffer = reinterpret_cast<Pixel*>(target);

    for (size_t y = 0; y < height; y++)
    {
        size_t targetY_1 = y * 2;

        for (size_t x = 0; x < width; x++)
        {
            size_t index = CalculateIndex(x, y, width);
            Pixel pix = sourceBuffer[index];

            size_t targetX_1 = x * 2;
            size_t targetX_2 = targetX_1 + 1;

            size_t idx1 = CalculateIndex(targetX_1, targetY_1, targetWidth);
            size_t idx2 = CalculateIndex(targetX_2, targetY_1, targetWidth);

            targetBuffer[idx1] = pix;
            targetBuffer[idx2] = pix;
        }

        size_t from = CalculateIndex(0, targetY_1, targetWidth);
        size_t to = CalculateIndex(0, targetY_1 + 1, targetWidth);

        Pixel* src = targetBuffer + from;
        Pixel* dst = targetBuffer + to;

        std::memcpy((void*)(dst), (void*)(src), sizeof(Pixel) * targetWidth);
    }

    Profiler::Get().EndMarker();
}
