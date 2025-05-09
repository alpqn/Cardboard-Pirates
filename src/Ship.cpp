#include <algorithm>
#include <format>

#include "Ship.hpp"
#include "Cannon.hpp"
#include "Sound.hpp"
#include "GameUtils.hpp"

bool Ship::m_renderUI{ false };
TTF_Font* Ship::m_fontUI{ nullptr };

Ship::Ship(const Color& color, const float x, const float y, const float angle)
:m_color{ color }
,Texture{ x, y, 66.0f, 113.0f, angle }
,m_collider{ x, y, 52.0f, 100.0f, angle }
,m_healthUI{ 0.0f, 0.0f, 50.0f, 40.0f, 0.0f }
,m_pointsUI{ 0.0f, 0.0f, 20.0f, 40.0f, 0.0f }
,m_shieldUI{ 0.0f, 0.0f, 32.0f, 32.0f, 0.0f, "shield.png" }
{
    switch(color)
    {
        case Color::RED :
            m_healthUI.setPos(100.0f, 32.0f);
            m_pointsUI.setPos(20.0f, 32.0f);
            m_shieldUI.setPos(52.0f, 32.0f);
            break;
        case Color::BLUE :
            m_healthUI.setPos(g::windowW - 100.0f, 32.0f);
            m_pointsUI.setPos(g::windowW - 20.0f, 32.0f);
            m_shieldUI.setPos(g::windowW - 52.0f, 32.0f);
            break;
    }

    m_healthUI.getTextureFromFont(m_fontUI, std::format("{:<3}", std::to_string(getHealth())), getColorAsRGB());
    m_pointsUI.getTextureFromFont(m_fontUI, std::to_string(getPoints()), getColorAsRGB());
    changeTexture("ship" + getColorAsString() + "1.png");
}

void Ship::render() noexcept
{
    // Remove cannons that are out of bounds or/and destroyed
    if(m_cannons.size())
    {
        auto f = std::remove_if
        (
            m_cannons.begin(), m_cannons.end(), [](const Cannon& c)
            {
                return !c.isExploding() && (c.getX() < 1 || c.getY() < 1 || c.getX() >= g::windowW || c.getY() >= g::windowH);
            }
        );
        if(f != m_cannons.end()) { m_cannons.erase(f); }
    }

    for(auto& c : m_cannons) { c.render(); }
    if(m_health < 30) changeTexture("ship" + getColorAsString() + "3.png");
    else if(m_health < 90) changeTexture("ship" + getColorAsString() + "2.png");
    else if(90 < m_health) changeTexture("ship" + getColorAsString() + "1.png");
    if(m_health > 0)
    {
        Texture::render();
    }
    if(m_renderUI) { m_healthUI.render(); m_pointsUI.render(); m_shieldUI.render(); }
}

void Ship::input(const SDL_Scancode& thrust, const SDL_Scancode& left, const SDL_Scancode& right, const SDL_Scancode& shoot) noexcept
{
    const Uint8* state{ SDL_GetKeyboardState(NULL) };
    if(state[thrust]) { thrustForward(); m_collider.thrustForward();  }
    if(state[right]) { rotateRight(); m_collider.rotateRight(); }
    if(state[left]) { rotateLeft(); m_collider.rotateLeft(); }
    if(state[shoot]) { shootForward(); }
}

std::string Ship::getColorAsString() const noexcept
{
    switch(m_color)
    {
        case Color::RED: return "RED";
        case Color::BLUE: return "BLUE";
    }
    return "";
}

void Ship::shootForward() noexcept
{
    // 0.5 seconds cooldown
    if(SDL_GetTicks() - m_shootCooldown > 500)
    {
        m_cannons.push_back({ getX(), getY(), m_angle });
        static SoundEffect shootingSound{ "fire.wav" }; shootingSound.play();
        m_shootCooldown = SDL_GetTicks();
    }
}

void Ship::checkBoundariesAndUpdate(const Map& map) noexcept
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


void Ship::checkCollisionAndUpdate(Ship& ship1, Ship& ship2) noexcept
{
    for(auto& c : ship1.m_cannons) if(c.isColliding(ship2.m_collider)) ship2.setHealth(ship2.getHealth() - g::cannonDamage);
    for(auto& c : ship2.m_cannons) if(c.isColliding(ship1.m_collider)) ship1.setHealth(ship1.getHealth() - g::cannonDamage);
    static Uint32 sinceLastCollision{ 0 };

    if(ship1.m_collider.checkCollisionAndResolve(ship2.m_collider) && SDL_GetTicks() - sinceLastCollision > 200)
    {
        static SoundEffect collisionSound{ "ship-collision.wav" }; collisionSound.play();
        ship1.setHealth(ship1.getHealth() - 1);
        ship2.setHealth(ship2.getHealth() - 1);

        ship1.setX(ship1.m_collider.getX());
    	ship1.setY(ship1.m_collider.getY());

    	ship2.setX(ship2.m_collider.getX());
        ship2.setY(ship2.m_collider.getY());

        sinceLastCollision = SDL_GetTicks();
    }
}

void Ship::reset() noexcept
{
    if(getColorAsString() == "BLUE") { setPos(1216.0f, 384.0f); }
    else if(getColorAsString() == "RED") { setPos(64.0f, 384.0f); }

    setHealth(100);
    m_cannons.clear();
}

SDL_Color Ship::getColorAsRGB() const noexcept
{
    if(getColorAsString() == "BLUE") return { 0, 0, 255 };
    else if(getColorAsString() == "RED") return { 255, 0, 0 };

    return { 0, 0, 0 };
}

void Ship::setHealth(const int health) noexcept
{
    m_health = health;
    m_healthUI.getTextureFromFont(m_fontUI, std::format("{:<3}", std::to_string(getHealth())), getColorAsRGB());
}

void Ship::setPoints(const int points) noexcept
{
    m_points = points;
    m_pointsUI.getTextureFromFont(m_fontUI, std::to_string(getPoints()), getColorAsRGB());
}
