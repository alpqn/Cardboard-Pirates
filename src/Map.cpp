#include <iostream>
#include <fstream>

#include "Map.hpp"

Map::Map(const std::string& file)
{
    loadMapFromFile(file);
}

void Map::loadMapFromFile(const std::string& file)
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

    for(size_t i{ 0 }; i < ids.size(); ++i)
    {
        for(size_t j{ 1 }; j < ids[i].size(); j += 2)
        {
            std::string id{ concat(ids[i][j - 1], ids[i][j]) };
            if(id == "00") continue;
            else
            {
                bool collision{ false };
                if(i > 0) { if(concat(ids[i - 1][j - 1], ids[i - 1][j]) == "00") collision = true; }
                if(!collision && i < mapH - 1) { if(concat(ids[i + 1][j - 1], ids[i + 1][j]) == "00") collision = true; }
                if(!collision && j > 2) { if(concat(ids[i][j - 3], ids[i][j - 2]) == "00") collision = true; }
                if(!collision && j/2 < mapW - 2) { if(concat(ids[i][j + 1], ids[i][j + 2]) == "00") collision = true;}

                m_tiles.push_back({ j/2 * g::tileW + g::tileW/2, i * g::tileH + g::tileH/2, id });
                if(collision) m_colliders.push_back({ j/2 * g::tileW + g::tileW/2, i * g::tileH + g::tileH/2, g::tileW, g::tileW, 0.0f });
            }
        }
    }
    for(auto& c: m_colliders) { c.update(); }
}

void Map::render() const
{
    for(const auto& t: m_tiles) { t.render(); }
}
