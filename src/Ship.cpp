#include <algorithm>

#include "Ship.hpp"
#include "Cannon.hpp"
#include "Sound.hpp"
#include "GameUtils.hpp"

Ship::Ship(Color color, float x, float y, float angle, SDL_Renderer*& renderer)
:m_color{ color }
,Texture{ x, y, 66.0f, 113.0f, angle }
,m_collider{ x, y, 52.0f, 100.0f, angle }
{
    changeTexture("ship" + getColorAsString() + "1.png", renderer);
}

void Ship::render(SDL_Renderer*& renderer)
{
    // Remove cannons that are out of bounds or/and destroyed
    if(m_cannons.size())
    {
        auto f = std::remove_if
        (
            m_cannons.begin(), m_cannons.end(), [&](Cannon c)
            {
                return !c.isExploding() && (c.getX() < 1 || c.getY() < 1 || c.getX() >= g::windowW || c.getY() >= g::windowH);
            }
        );
        if(f != m_cannons.end()) { m_cannons.erase(f); }
    }

    for(auto& c : m_cannons) { c.render(renderer); }
    if(m_health < 30) changeTexture("ship" + getColorAsString() + "3.png", renderer);
    else if(m_health < 90) changeTexture("ship" + getColorAsString() + "2.png", renderer);
    else if(90 < m_health) changeTexture("ship" + getColorAsString() + "1.png", renderer);
    if(m_health > 0)
    {
        SDL_RenderCopyExF(renderer, m_texture, NULL, &m_rect, m_angle, NULL, SDL_FLIP_NONE);
    }
}

void Ship::input(SDL_Scancode thrust, SDL_Scancode left, SDL_Scancode right, SDL_Scancode shoot, SDL_Renderer*& renderer)
{
    const Uint8* state{ SDL_GetKeyboardState(NULL) };
    if(state[thrust]) { thrustForward(); m_collider.thrustForward();  }
    if(state[right]) { rotateRight(); m_collider.rotateRight(); }
    if(state[left]) { rotateLeft(); m_collider.rotateLeft(); }
    if(state[shoot]) { shootForward(renderer); }
}

const std::string Ship::getColorAsString() const
{
    switch(m_color)
    {
        case Color::RED: return "RED";
        case Color::BLUE: return "BLUE";
    }
    return "";
}

void Ship::shootForward(SDL_Renderer*& renderer)
{
    // 0.5 seconds cooldown
    if(SDL_GetTicks() - m_shootCooldown > 500)
    {
        m_cannons.push_back({ getX(), getY(), m_angle, renderer });
        static Sound shootingSound{ "fire.wav" }; shootingSound.play();
        m_shootCooldown = SDL_GetTicks();
    }
}

void Ship::checkBoundriesAndUpdate(const Map& map)
{
    for(const auto& c : m_collider.getTransformed())
    {
        if(c.x <= 0) { m_collider.setX(m_collider.getX() - c.x); setX(m_collider.getX()); }
        else if(c.x >= g::windowW) { m_collider.setX(m_collider.getX() - (c.x - g::windowW)); setX(m_collider.getX()); }
        if(c.y <= 0) { m_collider.setY(m_collider.getY() - c.y); setY(m_collider.getY()); }
        else if(c.y >= g::windowH) { m_collider.setY(m_collider.getY() - (c.y - g::windowH)); setY(m_collider.getY()); }
    }
    for(const auto& t : map.getColliders())
    {
        m_collider.checkCollisionAndResolve(t);
        setX(m_collider.getX());
        setY(m_collider.getY());
    }
    for(const auto& t : map.getColliders()) for(auto& c : m_cannons) c.isColliding(t);
}


void Ship::checkCollisionAndUpdate(Ship& ship1, Ship& ship2)
{
    for(auto& c : ship1.m_cannons) if(c.isColliding(ship2.m_collider)) ship2.setHealth(ship2.getHealth() - g::cannonDamage);
    for(auto& c : ship2.m_cannons) if(c.isColliding(ship1.m_collider)) ship1.setHealth(ship1.getHealth() - g::cannonDamage);
    static Uint32 sinceLastCollision{ 0 };

    if(ship1.m_collider.checkCollisionAndResolve(ship2.m_collider) && SDL_GetTicks() - sinceLastCollision > 200)
    {
        static Sound collisionSound{ "ship-collision.wav" }; collisionSound.play();
        ship1.setHealth(ship1.getHealth() - 1);
        ship2.setHealth(ship2.getHealth() - 1);

        ship1.setX(ship1.m_collider.getX());
    	ship1.setY(ship1.m_collider.getY());

    	ship2.setX(ship2.m_collider.getX());
        ship2.setY(ship2.m_collider.getY());

        sinceLastCollision = SDL_GetTicks();
    }
}

void Ship::reset()
{
    if(getColorAsString() == "BLUE") { setPos(1216.0f, 384.0f); }
    else if(getColorAsString() == "RED") { setPos(64.0f, 384.0f); }

    setHealth(100);
    m_cannons.clear();
}

const SDL_Color Ship::getColorAsRGB() const
{
    if(getColorAsString() == "BLUE") return { 0, 0, 255 };
    else if(getColorAsString() == "RED") return { 255, 0, 0 };

    return { 0, 0, 0 };
}
