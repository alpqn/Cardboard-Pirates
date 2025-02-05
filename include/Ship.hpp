#pragma once

#include <string>
#include <vector>

#include <SDL2/SDL.h>

#include "Texture.hpp"
#include "Sound.hpp"
#include "Cannon.hpp"
#include "Map.hpp"

class Ship : public Texture
{
public:
    enum class Color { RED, BLUE };
private:
    Color m_color;
    int m_health{ 100 };
    int m_points{ 0 };
    Uint32 m_shootCooldown;
    Sound m_shootingSound{ "fire.wav" };
    Collider m_collider;
    std::vector<Cannon> m_cannons;
public:
    Ship(Color color, float x, float y, float angle, SDL_Renderer*& renderer);

    void render(SDL_Renderer*& renderer);
    void input(SDL_Scancode thrust, SDL_Scancode left, SDL_Scancode right, SDL_Scancode shoot, SDL_Renderer*& renderer);
    void reset();
    void shootForward(SDL_Renderer*& renderer);
    void checkBoundriesAndUpdate(const Map& map);
    static void checkCollisionAndUpdate(Ship& ship1, Ship& ship2);

    const std::string getColorAsString() const;
    const SDL_Color getColorAsRGB() const;
    int getHealth() const { return m_health; }
    int getPoints() const { return m_points; }

    void setPos(float x, float y) { setX(x); setY(y); }
    void setX(float x) { Texture::setX(x); m_collider.setX(x); }
    void setY(float y) { Texture::setY(y); m_collider.setY(y); }
    void setHealth(int health) { m_health = health; }
    void setPoints(int points) { m_points = points; }
};
