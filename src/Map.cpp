#include <iostream>
#include <fstream>

#include "Map.hpp"

Map::Map(const std::string& file)
{
    loadMapFromFile(file);
}

void Map::loadMapFromFile(const std::string& file) noexcept
{
    m_colliders.clear();
    m_tiles.clear();
    std::ifstream f{ g::mapsDirectory + file };
    int mapW{ (int)g::windowW / (int)g::tileW };
    int mapH{ (int)g::windowH / (int)g::tileH };
    std::vector<std::string> ids;
    auto concat = [](char a, char b){ return std::string(1, a) + std::string(1, b); };
    if(!f) { std::cerr << "Couldn't locate map: " << g::mapsDirectory + file << '\n'; exit(1); }

    std::string str;
    while(std::getline(f, str)) ids.push_back(str);

    for(size_t row{ 0 }; row < ids.size(); ++row)
    {
        for(size_t col{ 1 }; col < ids[row].size(); col += 2)
        {
            std::string id{ concat(ids[row][col - 1], ids[row][col]) };
            if(id == "00") continue;
            else
            {
                bool collision{ false };
                if(row > 0 && concat(ids[row - 1][col - 1], ids[row - 1][col]) == "00") collision = true;
                else if(row < mapH - 1 && concat(ids[row + 1][col - 1], ids[row + 1][col]) == "00") collision = true;
                else if(col > 2 && concat(ids[row][col - 3], ids[row][col - 2]) == "00")  collision = true;
                else if(col/2 < mapW - 2 && concat(ids[row][col + 1], ids[row][col + 2]) == "00") collision = true;

                m_tiles.emplace_back(Tile{ col/2 * g::tileW + g::tileW/2, row * g::tileH + g::tileH/2, id });
                if(collision) m_colliders.emplace_back(Collider{ col/2 * g::tileW + g::tileW/2, row * g::tileH + g::tileH/2, g::tileW, g::tileW, 0.0f });
            }
        }
    }
    for(auto& c: m_colliders) { c.update(); }
}

void Map::render() const noexcept
{
    for(const auto& t: m_tiles) { t.render(); }
}
