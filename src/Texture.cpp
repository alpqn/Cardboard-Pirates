#include <cmath>

#include <SDL2/SDL_image.h>

#include "Texture.hpp"
#include "ResourceMgr.hpp"

Texture::Texture(float x, float y, float w, float h, float angle, const std::string& file, SDL_Renderer*& renderer)
:m_rect{ x - w/2, y - h/2, w, h }
,m_angle{ angle }
{
    m_texture = ResourceMgr::getMgr().getTexture(g::imagesDirectory + file, renderer);
}

Texture::Texture(const std::string& file, SDL_Renderer*& renderer)
{
    m_texture = ResourceMgr::getMgr().getTexture(g::imagesDirectory + file, renderer);
}

Texture::Texture(float x, float y, float w, float h, float angle)
:m_rect{ x - w/2, y - h/2, w, h }
,m_angle{ angle }
{
}

void Texture::render(SDL_Renderer*& renderer) const
{
    SDL_RenderCopyExF(renderer, m_texture, NULL, &m_rect, m_angle, NULL, SDL_FLIP_NONE);
}

void Texture::changeTexture(const std::string& file, SDL_Renderer*& renderer)
{
    m_texture = ResourceMgr::getMgr().getTexture(g::imagesDirectory + file, renderer);
}

void Texture::getTextureFromFont(TTF_Font* font, const std::string& text, SDL_Color color, SDL_Renderer*& renderer)
{
    SDL_Surface* s{ TTF_RenderText_Solid(font, text.c_str(), color) };
    m_texture = SDL_CreateTextureFromSurface(renderer, s);
    SDL_FreeSurface(s);
}

bool Texture::isClicked() const
{
    int x, y;
    SDL_GetMouseState(&x, &y);
    if(x < m_rect.x || x > m_rect.x + m_rect.w || y < m_rect.y || y > m_rect.y + m_rect.h) { return false; }
    return true;
}

void Texture::thrustForward()
{
    m_rect.x += sinf(m_angle / 180.0f * g::PI) * g::thrustMultiplier;
    m_rect.y -= cosf(m_angle / 180.0f * g::PI) * g::thrustMultiplier;
}
