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

    Ship(Color color, float x, float y, float angle);

    void render() noexcept;
    void input(SDL_Scancode thrust, SDL_Scancode left, SDL_Scancode right, SDL_Scancode shoot) noexcept;
    void reset() noexcept;
    void shootForward() noexcept;
    void checkBoundriesAndUpdate(const Map& map) noexcept;
    static void checkCollisionAndUpdate(Ship& ship1, Ship& ship2) noexcept;
    const std::string getColorAsString() const noexcept;
    const SDL_Color getColorAsRGB() const noexcept;
    constexpr bool isAlive() const noexcept { return m_health > 0; }
    constexpr int getHealth() const noexcept { return m_health; }
    constexpr int getPoints() const noexcept { return m_points; }

    void setHealth(int health) noexcept;
    void setPoints(int points) noexcept;
    constexpr void setPos(float x, float y) noexcept { setX(x); setY(y); }
    constexpr void setX(float x) noexcept { Texture::setX(x); m_collider.setX(x); }
    constexpr void setY(float y) noexcept { Texture::setY(y); m_collider.setY(y); }
    static constexpr void setRenderUI(bool flag) noexcept { m_renderUI = flag; }
    static constexpr void setFontUI(TTF_Font* font) noexcept { m_fontUI = font; }
private:
    Color m_color;
    int m_health{ 100 };
    int m_points{ 0 };
    Uint32 m_shootCooldown;
    SoundEffect m_shootingSound{ "fire.wav" };
    Collider m_collider;
    std::vector<Cannon> m_cannons;
    static bool m_renderUI;
    static TTF_Font* m_fontUI;
    Texture m_healthUI;
    Texture m_pointsUI;
    Texture m_shieldUI;
};
