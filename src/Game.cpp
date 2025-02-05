#include <iostream>
#include <cmath>
#include <format>

#include <SDL2/SDL_mixer.h>
#include <SDL2/SDL_ttf.h>

#include "Game.hpp"
#include "GameUtils.hpp"
#include "Map.hpp"
#include "Ship.hpp"
#include "Sound.hpp"
#include "ResourceMgr.hpp"


Game::Game(const std::string& title, int x, int y, int w, int h)
:m_wWidth{ w }, m_wHeight{ h }
{
    if(SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO) < 0) { std::cerr << "ERROR:" << SDL_GetError(); exit(1); }
    if(TTF_Init() < 0) { std::cerr << "ERROR:" << TTF_GetError(); exit(1); }
    if((IMG_Init(IMG_INIT_PNG)) != IMG_INIT_PNG) { std::cerr << "ERROR: " << IMG_GetError(); exit(1); }
    if((Mix_Init(MIX_INIT_MP3) != MIX_INIT_MP3) || (Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 2048) < 0))
    {
        std::cerr << "ERROR: " << Mix_GetError();
        exit(1);
    }
    m_window = SDL_CreateWindow(title.c_str(), x, y,  m_wWidth,  m_wHeight, SDL_WINDOW_SHOWN);
    SDL_SetWindowMinimumSize(m_window, 1280, 768);
    SDL_SetWindowMaximumSize(m_window, 1280, 768);
    m_renderer = SDL_CreateRenderer(m_window, -1, SDL_RENDERER_ACCELERATED);
}

Game::~Game()
{
    SDL_DestroyRenderer(m_renderer);
    SDL_DestroyWindow(m_window);
    TTF_Quit();
    IMG_Quit();
    Mix_CloseAudio();
    Mix_Quit();
    SDL_Quit();
}

std::vector<Texture> initializeWaterBackground(SDL_Renderer*& renderer)
{
    std::vector<Texture> water;
    water.push_back({ 640.0f, 384.0f, 1280.0f, 768.0f, 0, "water1.png", renderer });
    water.push_back({ -640.0f, 384.0f, 1280.0f, 768.0f, 0, "water1.png", renderer });
    water.push_back({ 640.0f, 384.0f, 1280.0f, 768.0f, 0, "water2.png", renderer });
    water.push_back({ 640.0f, -384.0f, 1280.0f, 768.0f, 0, "water2.png", renderer });
    SDL_SetTextureBlendMode(water[0].getTexture(), SDL_BLENDMODE_NONE);
    SDL_SetTextureBlendMode(water[1].getTexture(), SDL_BLENDMODE_NONE);
    SDL_SetTextureBlendMode(water[2].getTexture(), SDL_BLENDMODE_MOD);
    SDL_SetTextureBlendMode(water[3].getTexture(), SDL_BLENDMODE_MOD);

    return water;
}

void updateWaterBackground(std::vector<Texture>& water, SDL_Renderer*& renderer)
{
    // index 0 and 1 -> x axis
    // index 2 and 3 -> y axis
    water[0].setX(water[0].getX() + 1);
    if(water[0].getX() > 1919) { water[0].setX(-639); }
    water[1].setX(water[1].getX() + 1);
    if(water[1].getX() > 1919) { water[1].setX(-639); }
    water[2].setY(water[2].getY() + 1);
    if(water[2].getY() > 1151) { water[2].setY(-384); }
    water[3].setY(water[3].getY() + 1);
    if(water[3].getY() > 1151) { water[3].setY(-384); }
    for(const auto& t : water) { t.render(renderer); }
}

void Game::run()
{
    Ship red{ Ship::Color::RED, 64.0f, 384.0f, 0.0f, m_renderer };
    Ship blue{ Ship::Color::BLUE, 1216.0f, 384.0f, 0.0f, m_renderer };
    std::vector<Texture> water{ initializeWaterBackground(m_renderer) };
    Map map{ "map1.txt", m_renderer };
    TTF_Font* aurulentMono{ ResourceMgr::getMgr().getFont(g::fontsDirectory + "AurulentSansMNerdFontMono-Regular.otf", 40) };
    Texture quitButton{ 640.0f, 628.0f, 110.0f, 56.0f, 0.0f, "quitButton.png", m_renderer };
    Texture shield1{ 52.0f, 32.0f, 32.0f, 32.0f, 0.0f, "shield.png", m_renderer };
    Texture shield2{ 1228.0f, 32.0f, 32.0f, 32.0f, 0.0f, "shield.png", m_renderer };
    Texture redHealth{ 100.0f, 32.0f, 50.0f, 40.0f, 0.0f };
    Texture blueHealth{ 1180.0f, 32.0f, 50.0f, 40.0f, 0.0f };
    Texture redPoints{ 20.0f, 32.0f, 20.0f, 40.0f, 0.0f };
    Texture bluePoints{ 1260.0f, 32.0f, 20.0f, 40.0f, 0.0f };
    Sound uiClickSound{ "ui-click.wav" };
    Sound backgroundMusic{ "background.mp3", true };
    Mix_MasterVolume(40);
    Mix_VolumeMusic(40);
    {
        Texture logo{ 640.0f, 254.0f, 500.0f, 268.0f, 0.0f, "logo.png", m_renderer };
        Texture playButton{ 640.0f, 544.5f, 150.0f, 69.0f, 0.0f, "playButton.png", m_renderer };
        Texture sfxButton{ 138.0f, 719.5f, 66.0f, 89.0f, 0.0f, "sfxon.png", m_renderer };
        Texture musicButton{ 42.0f, 719.5f, 74.0f, 89.0f, 0.0f, "musicon.png", m_renderer };
        Sound menuMusic{ "menu.mp3", true };
        menuMusic.playMusic();
        bool inMenu{ true };
        while(inMenu)
        {
            SDL_Event event;
            while (SDL_PollEvent(&event))
            {
                const Uint8* state{ SDL_GetKeyboardState(NULL) };
                if(state[SDL_SCANCODE_RETURN]) { uiClickSound.play(); inMenu = false; }
                if(state[SDL_SCANCODE_ESCAPE]) { uiClickSound.play(); SDL_Delay(120); exit(1); }
                if(event.type == SDL_QUIT) { inMenu = false; stop(); }
                if(event.type == SDL_MOUSEBUTTONUP)
                {
                    if(playButton.isClicked()) { uiClickSound.play(); inMenu = false; }
                    else if(quitButton.isClicked()) { uiClickSound.play(); SDL_Delay(120); exit(1); }
                    else if(musicButton.isClicked())
                    {
                        musicOn = !musicOn;
                        if(musicOn) { musicButton.changeTexture("musicon.png",m_renderer); Mix_ResumeMusic(); }
                        else { musicButton.changeTexture("musicoff.png",m_renderer); Mix_PauseMusic(); }
                        uiClickSound.play();
                    }
                    else if(sfxButton.isClicked())
                    {
                        sfxOn = !sfxOn;
                        if(sfxOn) { sfxButton.changeTexture("sfxon.png",m_renderer); Mix_MasterVolume(50);}
                        else { sfxButton.changeTexture("sfxoff.png",m_renderer); Mix_MasterVolume(0); }
                        uiClickSound.play();
                    }
                }
            }

            SDL_RenderClear(m_renderer);
            updateWaterBackground(water, m_renderer);
            map.render(m_renderer);
            red.render(m_renderer);
            blue.render(m_renderer);
            logo.render(m_renderer);
            sfxButton.render(m_renderer);
            musicButton.render(m_renderer);
            quitButton.render(m_renderer);
            playButton.render(m_renderer);
            SDL_RenderPresent(m_renderer);
            SDL_Delay(16); // Max frame rate (60 FPS)
        }
    }

    Mix_HaltMusic();
    if(musicOn){ backgroundMusic.fadeInMusic(250); }
    SDL_ShowCursor(0);

    int round{ 1 };
    while(m_running)
    {
        red.reset(); blue.reset();
        redPoints.getTextureFromFont(aurulentMono, std::to_string(red.getPoints()), {255,0,0}, m_renderer);
        bluePoints.getTextureFromFont(aurulentMono, std::to_string(blue.getPoints()), {0,0,255}, m_renderer);

        while(red.getHealth() > 0 && blue.getHealth() > 0)
        {
            SDL_Event event;
            while(SDL_PollEvent(&event)) { if(event.type == SDL_QUIT) exit(1); }

            red.input(SDL_SCANCODE_W, SDL_SCANCODE_A, SDL_SCANCODE_D, SDL_SCANCODE_S, m_renderer);
            blue.input(SDL_SCANCODE_UP, SDL_SCANCODE_LEFT, SDL_SCANCODE_RIGHT, SDL_SCANCODE_DOWN, m_renderer);

            redHealth.getTextureFromFont(aurulentMono, std::format("{:<3}", std::to_string(red.getHealth())), {255,0,0}, m_renderer);
            blueHealth.getTextureFromFont(aurulentMono, std::format("{:>3}", std::to_string(blue.getHealth())), {0,0,255}, m_renderer);

            Ship::checkCollisionAndUpdate(red, blue);
            blue.checkBoundriesAndUpdate(map);
            red.checkBoundriesAndUpdate(map);

            SDL_RenderClear(m_renderer);
            updateWaterBackground(water, m_renderer);
            map.render(m_renderer);
            red.render(m_renderer);
            blue.render(m_renderer);
            shield1.render(m_renderer);
            shield2.render(m_renderer);
            redHealth.render(m_renderer);
            blueHealth.render(m_renderer);
            redPoints.render(m_renderer);
            bluePoints.render(m_renderer);
            SDL_RenderPresent(m_renderer);

            SDL_Delay(16); // Max frame rate (60 FPS)
        }

        // Determine who won.
        red.getHealth() > blue.getHealth() ? red.setPoints(red.getPoints() + 1) : blue.setPoints(blue.getPoints() + 1);
        static Sound pointSound{ "point-round.wav" }; pointSound.play();
        ++round;

        // If the point difference is 2 (best of 3) or the 3rd round is over.
        if(round > 3 || abs(red.getPoints() - blue.getPoints()) == 2)
        {
            // Display who won. Restart or quit.
            Sound win{ "win-sequence.wav" }; win.play(7);
            Ship& winner{ red.getPoints() > blue.getPoints() ? red : blue };
            Texture restartButton{ 639.5f, 544.5f, 245.0f, 69.0f, 0.0f, "restartButton.png", m_renderer };
            Texture winnerDisplay{ 640.0f, 384.0f, 200.0f, 50.0f, 0.0f };
            winner.setPos(640.0f, 284.0f);
            winnerDisplay.getTextureFromFont(aurulentMono, winner.getColorAsString() + " WON!", winner.getColorAsRGB(), m_renderer);
            bool inEndGame{ true };
            SDL_ShowCursor(1);

            while(inEndGame)
            {
                SDL_Event event;
                while(SDL_PollEvent(&event))
                {
                    const Uint8* state{ SDL_GetKeyboardState(NULL) };
                    if(state[SDL_SCANCODE_RETURN])
                    {
                        round = 1;
                        red.setPoints(0); blue.setPoints(0);
                        inEndGame = false;
                        SDL_ShowCursor(0);
                        uiClickSound.play();
                    }
                    if(state[SDL_SCANCODE_ESCAPE]) { uiClickSound.play(); SDL_Delay(120); exit(1); }
                    if(event.type == SDL_QUIT) { exit(1); }
                    if(event.type == SDL_MOUSEBUTTONUP)
                    {
                        if(quitButton.isClicked()) { uiClickSound.play(); SDL_Delay(120); exit(1); }
                        else if(restartButton.isClicked())
                        {
                            round = 1;
                            red.setPoints(0); blue.setPoints(0);
                            inEndGame = false;
                            SDL_ShowCursor(0);
                            uiClickSound.play();
                        }
                    }
                }

                SDL_RenderClear(m_renderer);
                updateWaterBackground(water, m_renderer);
                map.render(m_renderer);
                restartButton.render(m_renderer);
                quitButton.render(m_renderer);
                winnerDisplay.render(m_renderer);
                winner.render(m_renderer);
                SDL_RenderPresent(m_renderer);
                SDL_Delay(16);
            }
        }

        map.loadMapFromFile("map" + std::to_string(round) + ".txt", m_renderer);
    }
}
