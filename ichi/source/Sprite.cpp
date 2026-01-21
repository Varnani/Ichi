#include "../include/Sprite.hpp"

Pixel Sprite::GetPixel(uint32_t x, uint32_t y)
{
    x += startX;
    y += startY;

    return image->GetPixel(x, y);
}