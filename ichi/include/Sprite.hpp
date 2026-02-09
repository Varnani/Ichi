#pragma once

#include <glm/glm.hpp>
#include <string>

#include "Image.hpp"
#include "Pixel.hpp"

struct Sprite
{
    Image* image;

    uint8_t startX;
    uint8_t startY;
    uint8_t width;
    uint8_t height;

    Pixel GetPixel(uint32_t x, uint32_t y) const;
};