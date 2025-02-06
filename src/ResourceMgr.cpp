#include <iostream>

#include <SDL2/SDL_image.h>

#include "ResourceMgr.hpp"
#include "GameUtils.hpp"

ResourceMgr& ResourceMgr::getMgr()
{
    static ResourceMgr s_mgr;
    return s_mgr;
}

SDL_Texture* ResourceMgr::getTexture(const std::string& file)
{
    auto search = m_textureDatabase.find(file);
    if(search != m_textureDatabase.end()) { return search->second; }
    else
    {
        SDL_Texture* t{ IMG_LoadTexture(g::renderer,file.c_str()) };
        if (!t) { std::cerr << "ERROR: " << file << IMG_GetError(); exit(1); }
        m_textureDatabase[file] = t;
        return t;
    }
}

TTF_Font* ResourceMgr::getFont(const std::string& file, int ptsize)
{
    auto search = m_fontDatabase.find(file);
    if(search != m_fontDatabase.end()) { return search->second; }
    else
    {
        TTF_Font* font{ TTF_OpenFont(file.c_str(), ptsize) };
        if(!font) { std::cerr << "ERROR: " << file << TTF_GetError(); exit(1); }
        m_fontDatabase[file] = font;
        return font;
    }
}

Mix_Chunk* ResourceMgr::getSound(const std::string& file)
{
    auto search = m_soundDatabase.find(file);
    if(search != m_soundDatabase.end()) { return search->second; }
    else
    {
        Mix_Chunk* sound{ Mix_LoadWAV(file.c_str()) };
        if(!sound) { std::cerr << "ERROR: "<< file << Mix_GetError(); exit(1); }
        m_soundDatabase[file] = sound;
        return sound;
    }
}
