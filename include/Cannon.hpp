#pragma once

#include "Texture.hpp"
#include "Collider.hpp"

class Cannon : public Texture
{
private:
    float m_dX;
    float m_dY;
    Uint32 m_sinceExplosion;
    Collider m_collider;
    Texture m_explosion;
public:
    Cannon(float x, float y, float angle, SDL_Renderer*& renderer);

    void render(SDL_Renderer*& renderer);
    bool isColliding(const Collider& target);
    bool isExploding() const;

    void setX(float x) { Texture::setX(x); m_collider.setX(x); }
    void setY(float y) { Texture::setY(y); m_collider.setY(y); }
};
