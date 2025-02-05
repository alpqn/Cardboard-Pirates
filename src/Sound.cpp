#include <iostream>

#include "Sound.hpp"
#include "GameUtils.hpp"
#include "ResourceMgr.hpp"

Sound::Sound(const std::string& file, bool isMusic)
{
    if(!isMusic) { m_chunk = ResourceMgr::getMgr().getSound((g::soundsDirectory + file).c_str()); }
    else{ m_music = Mix_LoadMUS((g::soundsDirectory + file).c_str()); if(!m_music) { std::cerr << "ERROR: "<< file << Mix_GetError(); exit(1); }}
}

Sound::~Sound()
{
    // Avoid double-deleting.
    m_chunk = nullptr;
    m_music = nullptr;
    Mix_FreeChunk(m_chunk);
    Mix_FreeMusic(m_music);
}
