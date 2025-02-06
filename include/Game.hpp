#pragma once

#include <SDL2/SDL.h>
#include <string>

class Game
{
private:
    SDL_Window* m_window;
    int m_wWidth;
    int m_wHeight;
    bool m_running{ true };
    bool sfxOn{ true };
    bool musicOn{ true };
public:
    Game(const std::string& title, int x, int y, int w, int h);
    ~Game();

    void start() { m_running = true; }
    void stop() { m_running = false; }
    void run();
};
