#include "../include/Pixel.hpp"

const Pixel Pixel::Red{ 255, 0, 0, 255 };
const Pixel Pixel::Green{ 0, 255, 0, 255 };
const Pixel Pixel::Blue{ 0, 0, 255, 255 };
const Pixel Pixel::Clear{ 0, 0, 0, 0 };
const Pixel Pixel::White{ 255, 255, 255, 255 };
const Pixel Pixel::Black{ 0, 0, 0, 255 };

Pixel Pixel::FromUInt32(uint32_t source)
{
    Pixel pixel{};

    uint32_t* ptr = reinterpret_cast<uint32_t*>(&pixel);
    *ptr = source;

    return pixel;
}

uint32_t Pixel::ToUInt32(Pixel pixel)
{
    uint32_t num = 0;

    uint32_t* ptr = reinterpret_cast<uint32_t*>(&pixel);
    num = *ptr;

    return num;
}