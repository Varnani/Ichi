#include <cstdint>

#include "../include/Pixel.hpp"
#include "../include/Sprite.hpp"

Pixel Sprite::GetPixel(uint32_t x, uint32_t y) const
{
    x += startX;
    y += startY;

    return image->GetPixel(x, y);
}