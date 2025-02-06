#pragma once

#include <string>
#include <unordered_map>

#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include <SDL2/SDL_mixer.h>

class ResourceMgr
{
private:
    std::unordered_map<std::string, SDL_Texture*> m_textureDatabase;
    std::unordered_map<std::string, TTF_Font*> m_fontDatabase;
    std::unordered_map<std::string, Mix_Chunk*> m_soundDatabase;

    ResourceMgr() {}
    ResourceMgr(const ResourceMgr&) = delete;
    ResourceMgr& operator=(const ResourceMgr&) = delete;
public:
    static ResourceMgr& getMgr();
    SDL_Texture* getTexture(const std::string& file);
    TTF_Font* getFont(const std::string& file, int ptsize);
    Mix_Chunk* getSound(const std::string& file);
};
