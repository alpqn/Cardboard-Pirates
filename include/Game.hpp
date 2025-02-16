#pragma once

#include <SDL2/SDL.h>
#include <string>

class Game
{
public:
    Game(const std::string& title, int x, int y);
    ~Game();

    constexpr void start() noexcept { m_running = true; }
    constexpr void stop() noexcept { m_running = false; exit(1); }
    void run() noexcept;
private:
    SDL_Window* m_window;
    bool m_running{ true };
    bool sfxOn{ true };
    bool musicOn{ true };
};
