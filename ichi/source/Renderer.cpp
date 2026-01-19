#include <cstdint>
#include <string.h>

#include <glm/glm.hpp>

#include "../include/Pixel.hpp"
#include "../include/Renderer.hpp"

size_t CalculateIndex(size_t x, size_t y, uint32_t width)
{
    return x + (y * width);
}

void Renderer::Resize(uint32_t w, uint32_t h)
{
    if (w == width && h == height) return;

    width = w;
    height = h;

    m_buffer.resize(width * height);
}

void Renderer::Clear(Pixel color)
{
    for (size_t i = 0; i < m_buffer.size(); i++)
    {
        m_buffer[i] = color;
    }
}

void Renderer::DrawRect(glm::vec2 position, glm::vec2 size, Pixel color)
{
    for (size_t x = position.x; x < position.x + size.x; x++)
    {
        if (x >= width) continue;

        for (size_t y = position.y; y < position.y + size.y; y++)
        {
            if (y >= height) continue;

            size_t index = CalculateIndex(x, y, width);
            m_buffer[index] = color;
        }
    }
}

void Renderer::Present(uint8_t* target, uint32_t targetWidth, uint32_t targetHeight)
{
    memcpy((void*)target, (void*)m_buffer.data(), sizeof(Pixel) * m_buffer.size());
}