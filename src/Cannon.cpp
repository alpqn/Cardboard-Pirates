#include <cmath>

#include "Cannon.hpp"
#include "GameUtils.hpp"
#include "Sound.hpp"

Cannon::Cannon(float x, float y, float angle)
:Texture{ x, y, 10.0f, 10.0f, angle, "cannon.png" }
,m_dX{ sinf(angle / 180.0f * g::PI) * g::bulletSpeed }
,m_dY{ -cosf(angle / 180.0f * g::PI) * g::bulletSpeed }
,m_collider{ x, y, 10.0f, 10.0f, angle }
,m_explosion{ "explosion.png" }
{
    m_explosion.setScale(40.0f, 40.0f);
}

void Cannon::render() noexcept
{
    // If there is an explosion animation timer.
    if(m_sinceExplosion) { m_explosion.render(); }
    else
    {
        Texture::render();
        setX(getX() + m_dX);
        setY(getY() + m_dY);
    }
}

bool Cannon::isColliding(const Collider& target) noexcept
{
    if(m_collider.isColliding(target))
    {
        m_explosion.setPos(getX(), getY());
        setX(-INFINITY); setY(-INFINITY); // Quasi destroy the cannon.
        static SoundEffect explosionSound{ "explosion.wav" };
        explosionSound.play();
        m_sinceExplosion = SDL_GetTicks();
        return true;
    }
    return false;
}

bool Cannon::isExploding() const noexcept
{
    if(!m_sinceExplosion) return false;
    else if(SDL_GetTicks() - m_sinceExplosion < 250) return true;

    return false;
}
