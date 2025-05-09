#include "Tile.hpp"
#include "GameUtils.hpp"

Tile::Tile(const float x, const float y, const std::string& id)
:Texture{ x, y, g::tileW, g::tileH, 0, "tile_" + id + ".png" }
{
}
