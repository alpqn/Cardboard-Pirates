#pragma once

#include <SDL2/SDL.h>

#include "Texture.hpp"

class Tile : public Texture
{
public:
    Tile(float x, float y, const std::string& id);
    void rotateLeft() = delete;
    void rotateRight() = delete;
    void thrustForward() = delete;
private:
    const std::string m_id;
};
