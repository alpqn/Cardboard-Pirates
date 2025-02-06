#pragma once

#include <vector>
#include <string>

#include <SDL2/SDL.h>

#include "Tile.hpp"
#include "Collider.hpp"

class Map
{
private:
    std::vector<Tile> m_tiles;
    std::vector<Collider> m_colliders;
public:
    Map(const std::string& file);
    void loadMapFromFile(const std::string& file);
    void render() const ;
    const std::vector<Collider>& getColliders() const { return m_colliders; };
};
