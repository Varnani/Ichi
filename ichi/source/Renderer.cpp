#include <cstdint>
#include <string.h>

#include <glm/glm.hpp>

#include "../include/Pixel.hpp"
#include "../include/Renderer.hpp"
#include "../include/Sprite.hpp"

Renderer& Renderer::Get()
{
    static Renderer renderer{};
    return renderer;
}

void Renderer::Resize(const uint32_t w, const uint32_t h)
{
    if (w == width && h == height) return;

    width = w;
    height = h;

    m_buffer.resize(width * height);
}

void Renderer::Clear(const Pixel color)
{
    size_t count = m_buffer.size();
    Pixel* ptr = m_buffer.data();

    while (count > 0)
    {
        *ptr = color;
        ptr++;
        count--;
    }
}

void Renderer::DrawRect(const glm::ivec2 position, const glm::ivec2 size, const Pixel color)
{
    for (int x = position.x; x < position.x + size.x; x++)
    {
        if (x >= width) continue;
        if (x < 0) continue;

        for (int y = position.y; y < position.y + size.y; y++)
        {
            if (y >= height) continue;
            if (y < 0) continue;

            size_t index = x + (y * width);
            m_buffer[index] = color;
        }
    }
}

void Renderer::DrawSprite(const glm::ivec2 position, const Sprite& sprite)
{
    for (int spriteX = 0; spriteX < sprite.width; spriteX++)
    {
        int bufX = spriteX + position.x;
        if (bufX >= width) continue;
        if (bufX < 0) continue;

        for (int spriteY = 0; spriteY < sprite.height; spriteY++)
        {
            int bufY = spriteY + position.y;
            if (bufY >= height) continue;
            if (bufY < 0) continue;

            size_t index = bufX + (bufY * width);

            Pixel spritePixel = sprite.GetPixel(spriteX, spriteY);
            if (spritePixel.a > 0) m_buffer[index] = spritePixel;
        }
    }
}

void Renderer::Present(uint8_t* target, const uint32_t targetWidth, const uint32_t targetHeight)
{
    memcpy((void*)target, (void*)m_buffer.data(), sizeof(Pixel) * m_buffer.size());
}