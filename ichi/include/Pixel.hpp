#pragma once

#include <cstdint>
#include <type_traits>

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

namespace Pixel_Asserts
{
    static_assert(sizeof(Pixel) == 4, "sizeof(Pixel) should be 4.");
    static_assert(std::is_trivially_copyable_v<Pixel>, "Pixel should be trivially copyable.");
    static_assert(std::is_standard_layout_v<Pixel>, "Pixel should have standard layout.");
    static_assert(std::is_aggregate_v<Pixel>, "Pixel should be an aggregate.");
} // namespace Pixel_Asserts
