#pragma once

#include <string>

#include <SDL2/SDL_mixer.h>

// SDL_mixer has two separate data structures for audio data. One called a "chunk", which is meant to be a file completely decoded into memory
// up front, and the other called "music" which is a file intended to be decoded on demand.

class SoundEffect
{
public:
    explicit SoundEffect(const std::string& file);
    ~SoundEffect();

    constexpr void play(int channel = -1, int repeats = 0) const noexcept { Mix_PlayChannel(channel, m_chunk, repeats); }
private:
    Mix_Chunk* m_chunk;
};


class Music
{
public:
    explicit Music(const std::string& file);
    ~Music();

    constexpr void play(int repeats = -1) const noexcept { Mix_PlayMusic(m_music, repeats); }
    constexpr void pause() const noexcept { Mix_PauseMusic(); }
    constexpr void resume() const noexcept { Mix_ResumeMusic(); }
    constexpr void fadeIn(int ms, int repeats = -1) const noexcept { Mix_FadeInMusic(m_music, repeats, ms); }
    constexpr void fadeOut(int ms) const noexcept { Mix_FadeOutMusic(ms); }
private:
    Mix_Music* m_music;
};
