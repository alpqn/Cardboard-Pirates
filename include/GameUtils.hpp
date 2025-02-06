#pragma once
#include <string>
#include <SDL2/SDL.h>

namespace g
{
    constexpr int windowW{ 1280 };
    constexpr int windowH{ 768 };
    constexpr float tileW{ 64.0f };
    constexpr float tileH{ 64.0f };
    constexpr double PI{ 3.14159 };
    constexpr float rotationMultiplier{ 5 };
    constexpr float thrustMultiplier{ 6.5f };
    constexpr float bulletSpeed{ 20.0f };
    constexpr int cannonDamage{ 10 };
    extern SDL_Renderer* renderer;
    inline const std::string imagesDirectory{ "../assets/images/"};
    inline const std::string soundsDirectory{ "../assets/sounds/"};
    inline const std::string mapsDirectory{ "../assets/maps/"};
    inline const std::string fontsDirectory{ "../assets/fonts/"};
}
