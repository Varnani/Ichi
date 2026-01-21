#include "../include/Pixel.hpp"
#include <bit>

const Pixel Pixel::Red{ 255, 0, 0, 255 };
const Pixel Pixel::Green{ 0, 255, 0, 255 };
const Pixel Pixel::Blue{ 0, 0, 255, 255 };
const Pixel Pixel::Clear{ 0, 0, 0, 0 };
const Pixel Pixel::White{ 255, 255, 255, 255 };
const Pixel Pixel::Black{ 0, 0, 0, 255 };

Pixel Pixel::FromUInt32(uint32_t source)
{
    Pixel pixel = std::bit_cast<Pixel>(source);
    return pixel;
}

uint32_t Pixel::ToUInt32(Pixel pixel)
{
    uint32_t num = std::bit_cast<uint32_t>(pixel);
    return num;
}