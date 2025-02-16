#pragma once

#include <vector>
#include <string>

#include <SDL2/SDL.h>

#include "Tile.hpp"
#include "Collider.hpp"

class Map
{
public:
    explicit Map(const std::string& file);
    void loadMapFromFile(const std::string& file) noexcept;
    void render() const noexcept;
    constexpr const std::vector<Collider>& getColliders() const noexcept { return m_colliders; };
private:
    std::vector<Tile> m_tiles;
    std::vector<Collider> m_colliders;
};
