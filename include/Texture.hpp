#pragma once

#include <string>

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>

#include "GameUtils.hpp"

class Texture
{
protected:
    SDL_Texture* m_texture;
    SDL_FRect m_rect;
    float m_angle;
public:
    Texture(float x, float y, float w, float h, float angle, const std::string& file, SDL_Renderer*& renderer);
    Texture(const std::string& file, SDL_Renderer*& renderer);
    Texture(float x, float y, float w, float h, float angle);

    void render(SDL_Renderer*& renderer) const;
    void changeTexture(const std::string& file, SDL_Renderer*& renderer);
    void getTextureFromFont(TTF_Font* font, const std::string& text, SDL_Color color, SDL_Renderer*& renderer);
    void rotateLeft() { m_angle -= g::rotationMultiplier; }
    void rotateRight() { m_angle += g::rotationMultiplier; }
    void thrustForward();
    bool isColliding(Texture& texture) const;
    bool isClicked() const;

    void setAttrib(float x, float y, float w, float h) { setPos(x, y); setScale(w, h); }
    void setPos(float x, float y) { setX(x); setY(y); }
    void setScale(float w, float h) { setW(w), setH(h); }
    void setX(float x) { m_rect.x = x - m_rect.w/2; }
    void setY(float y) { m_rect.y = y - m_rect.h/2; }
    void setW(float w) { m_rect.w = w; }
    void setH(float h) { m_rect.h = h; }
    void setAngle(float angle) { m_angle = angle; }

    float getX() const { return m_rect.x + m_rect.w/2; }
    float getY() const { return m_rect.y + m_rect.h/2; }
    float getW() const { return m_rect.w; }
    float getH() const { return m_rect.h; }
    float getAngle() const { return m_angle; }
    SDL_Texture*& getTexture() { return m_texture; }
};
