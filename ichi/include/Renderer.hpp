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

    void Resize(uint32_t width, uint32_t height);
    void Clear(Pixel color);
    void DrawRect(glm::vec2 position, glm::vec2 size, Pixel color);
    void DrawSprite(glm::uvec2 position, Sprite& sprite);
    void Present(uint8_t* target, uint32_t targetWidth, uint32_t targetHeight);

private:
    std::vector<Pixel> m_buffer{};
};