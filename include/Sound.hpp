#pragma once

#include <string>

#include <SDL2/SDL_mixer.h>

class Sound
{
private:
    Mix_Music* m_music{};
    Mix_Chunk* m_chunk{};
    // SDL_mixer has two separate data structures for audio data. One called a "chunk", which is meant to be a file completely decoded into memory
    // up front, and the other called "music" which is a file intended to be decoded on demand.
    bool m_isMusic;
public:
    Sound(const std::string& file, bool isMusic = false);
    ~Sound();
    void playMusic(int repeats = -1) const { Mix_PlayMusic(m_music, repeats); }
    void pauseMusic() const { Mix_PauseMusic(); }
    void resumeMusic() const { Mix_ResumeMusic(); }
    void fadeInMusic(int ms, int repeats = -1) const { Mix_FadeInMusic(m_music, repeats, ms); }
    void fadeOutMusic(int ms) const { Mix_FadeOutMusic(ms); }
    void play(int channel = -1, int repeats = 0) const { Mix_PlayChannel(channel, m_chunk, repeats); }
};
