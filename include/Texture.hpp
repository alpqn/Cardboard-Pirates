#pragma once

#include <string>

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>

#include "GameUtils.hpp"

class Texture
{
public:
    Texture(float x, float y, float w, float h, float angle, const std::string& file);
    Texture(float x, float y, float w, float h, float angle);
    explicit Texture(const std::string& file);

    void render() const noexcept;
    void changeTexture(const std::string& file) noexcept;
    void getTextureFromFont(TTF_Font* font, const std::string& text, const SDL_Color& color) noexcept;
    void thrustForward() noexcept;
    bool isClicked() const noexcept;
    constexpr void rotateLeft() noexcept { m_angle -= g::rotationMultiplier; }
    constexpr void rotateRight() noexcept { m_angle += g::rotationMultiplier; }

    constexpr void setAttrib(const float x, const float y, const float w, const float h) noexcept { setPos(x, y); setScale(w, h); }
    constexpr void setPos(const float x, const float y) noexcept { setX(x); setY(y); }
    constexpr void setScale(const float w, const float h) noexcept { setW(w), setH(h); }
    constexpr void setX(const float x) noexcept { m_rect.x = x - m_rect.w/2; }
    constexpr void setY(const float y) noexcept { m_rect.y = y - m_rect.h/2; }
    constexpr void setW(const float w) noexcept { m_rect.w = w; }
    constexpr void setH(const float h) noexcept { m_rect.h = h; }
    constexpr void setAngle(const float angle) noexcept { m_angle = angle; }

    constexpr float getX() const noexcept { return m_rect.x + m_rect.w/2; }
    constexpr float getY() const noexcept { return m_rect.y + m_rect.h/2; }
    constexpr float getW() const noexcept { return m_rect.w; }
    constexpr float getH() const noexcept { return m_rect.h; }
    constexpr float getAngle() const noexcept { return m_angle; }
    constexpr SDL_Texture*& getTexture() noexcept { return m_texture; }
protected:
    SDL_Texture* m_texture;
    SDL_FRect m_rect;
    float m_angle;
};
