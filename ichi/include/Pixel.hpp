#pragma once
#include <cstdint>

struct Pixel
{
    uint8_t r;
    uint8_t g;
    uint8_t b;
    uint8_t a;

    static Pixel FromUInt32(uint32_t source);
    static uint32_t ToUInt32(Pixel pixel);

    static const Pixel Red;
    static const Pixel Green;
    static const Pixel Blue;
    static const Pixel Clear;
    static const Pixel White;
    static const Pixel Black;
};
