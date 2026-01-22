#pragma once
#include <cstdint>
#include <vector>

#include <glm/glm.hpp>

#include "Pixel.hpp"
#include "Sprite.hpp"

class Renderer
{
public:
    uint32_t width;
    uint32_t height;

    void Resize(const uint32_t width, const uint32_t height);
    void Clear(const Pixel color);
    void DrawRect(const glm::ivec2 position, const glm::ivec2 size, const Pixel color);
    void DrawSprite(const glm::ivec2 position, const Sprite& sprite);
    void Present(uint8_t* target, const uint32_t targetWidth, const uint32_t targetHeight);

    static Renderer& Get();

private:
    std::vector<Pixel> m_buffer{};
};