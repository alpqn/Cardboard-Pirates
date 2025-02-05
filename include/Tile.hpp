#pragma once

#include <SDL2/SDL.h>

#include "Texture.hpp"

class Tile : public Texture
{
private:
    std::string m_id;
public:
    Tile(float x, float y, std::string id, SDL_Renderer*& renderer);

    void rotateLeft() = delete;
    void rotateRight() = delete;
    void thrustForward() = delete;
};
