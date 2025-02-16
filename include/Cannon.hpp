#pragma once

#include "Texture.hpp"
#include "Collider.hpp"

class Cannon : public Texture
{
public:
    Cannon(float x, float y, float angle);

    void render() noexcept;
    bool isColliding(const Collider& target) noexcept;
    bool isExploding() const noexcept;

    constexpr void setX(float x) noexcept { Texture::setX(x); m_collider.setX(x); }
    constexpr void setY(float y) noexcept { Texture::setY(y); m_collider.setY(y); }
private:
    float m_dX;
    float m_dY;
    Uint32 m_sinceExplosion{};
    Collider m_collider;
    Texture m_explosion;
};
