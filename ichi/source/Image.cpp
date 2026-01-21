#include <stb_image.h>

#include "../include/Image.hpp"
#include "../include/Pixel.hpp"

void Image::Load(std::string path)
{
    int width, height, channels;
    uint8_t* dataPtr = stbi_load(path.c_str(), &width, &height, &channels, 4);

    size.x = width;
    size.y = height;

    pixels.resize(width * height);

    std::memcpy(pixels.data(), dataPtr, pixels.size() * sizeof(Pixel));

    stbi_image_free(dataPtr);
}

Pixel Image::GetPixel(uint32_t x, uint32_t y)
{
    return pixels[x + (y * size.x)];
}

void Image::SetPixel(uint32_t x, uint32_t y, Pixel pixel)
{
    pixels[x + (y * size.x)] = pixel;
}