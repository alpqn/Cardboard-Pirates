#pragma once

#include <string>
#include <unordered_map>

#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include <SDL2/SDL_mixer.h>

class ResourceMgr
{
public:
    static ResourceMgr& getMgr() noexcept;
    SDL_Texture* getTexture(const std::string& file) noexcept;
    TTF_Font* getFont(const std::string& file, int ptsize) noexcept;
    Mix_Chunk* getSound(const std::string& file) noexcept;
private:
    std::unordered_map<std::string, SDL_Texture*> m_textureDatabase;
    std::unordered_map<std::string, TTF_Font*> m_fontDatabase;
    std::unordered_map<std::string, Mix_Chunk*> m_soundDatabase;

    ResourceMgr() {}
    ResourceMgr(const ResourceMgr&) = delete;
    ResourceMgr& operator=(const ResourceMgr&) = delete;
};
