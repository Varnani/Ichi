#pragma once

#include <glm/glm.hpp>
#include <string>
#include <type_traits>
#include <vector>

#include "Pixel.hpp"

struct Image
{
    glm::uvec2 size;
    std::string path;
    std::vector<Pixel> pixels;

    void Load(std::string path);
    Pixel GetPixel(const uint32_t x, const uint32_t y) const;
    void SetPixel(uint32_t x, uint32_t y, Pixel pixel);
};