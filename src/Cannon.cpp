#include "Cannon.hpp"
#include "GameUtils.hpp"
#include "Sound.hpp"

Cannon::Cannon(float x, float y, float angle, SDL_Renderer*& renderer)
:Texture{ x, y, 10.0f, 10.0f, angle, "cannon.png", renderer }
,m_dX{ sinf(angle / 180.0f * g::PI) * g::bulletSpeed }
,m_dY{ -cosf(angle / 180.0f * g::PI) * g::bulletSpeed }
,m_collider{ x, y, 10.0f, 10.0f, angle }
,m_explosion{ "explosion.png", renderer }
{
    m_explosion.setScale(40.0f, 40.0f);
}

void Cannon::render(SDL_Renderer*& renderer)
{
    // If there is an explosion animation timer.
    if(m_sinceExplosion) { m_explosion.render(renderer); }
    else
    {
        SDL_RenderCopyExF(renderer, m_texture, NULL, &m_rect, m_angle, NULL, SDL_FLIP_NONE);
        setX(getX() + m_dX);
        setY(getY() + m_dY);
    }
}

bool Cannon::isColliding(const Collider& target)
{
    if(m_collider.isColliding(target))
    {
        m_explosion.setPos(getX(), getY());
        setX(-INFINITY); setY(-INFINITY); // Quasi destroy the cannon.
        static Sound explosionSound{ "explosion.wav" };
        explosionSound.play();
        m_sinceExplosion = SDL_GetTicks();
        return true;
    }
    return false;
}

bool Cannon::isExploding() const
{
    if(!m_sinceExplosion) return false;
    else if(SDL_GetTicks() - m_sinceExplosion < 250) return true;

    return false;
}
