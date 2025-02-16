#include <iostream>

#include "Sound.hpp"
#include "GameUtils.hpp"
#include "ResourceMgr.hpp"

SoundEffect::SoundEffect(const std::string& file)
{
    m_chunk = ResourceMgr::getMgr().getSound((g::soundsDirectory + file).c_str());
}

SoundEffect::~SoundEffect()
{
    Mix_FreeChunk(m_chunk);
}

Music::Music(const std::string& file)
{
    m_music = Mix_LoadMUS((g::soundsDirectory + file).c_str());
    if(!m_music) { std::cerr << "ERROR: "<< file << Mix_GetError(); exit(1); }
}

Music::~Music()
{
    Mix_FreeMusic(m_music);
}
