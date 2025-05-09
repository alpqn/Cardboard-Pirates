#pragma once

#include <array>

#include <SDL2/SDL.h>

#include "GameUtils.hpp"

class Collider
{
public:
    Collider(float x, float y, float w, float h, float angle);

   void thrustForward() noexcept;
   constexpr void rotateLeft() noexcept { m_angle -= g::rotationMultiplier; update(); }
   constexpr void rotateRight() noexcept { m_angle += g::rotationMultiplier; update(); }
   void update() noexcept;
   bool isColliding(const Collider& target) const noexcept;
   bool checkCollisionAndResolve(const Collider& target) noexcept; // Only the caller is affected by the collision resolution
   bool checkCollisionAndResolve(Collider& target) noexcept;       // Both the caller and the target are affected by the collision resolution

   constexpr float getX() const noexcept { return m_center.x; }
   constexpr float getY() const noexcept { return m_center.y; }
   constexpr float getAngle() const noexcept { return m_angle; }
   constexpr std::array<SDL_FPoint, 4> getTransformed() const noexcept { return m_transformed; }

   constexpr void setX(const float x) noexcept { m_center.x = x; update(); }
   constexpr void setY(const float y) noexcept { m_center.y = y; update(); }
   constexpr void setAngle(const float angle) noexcept { m_angle = angle; }
private:
    float m_angle;
    SDL_FPoint m_center;
    std::array<SDL_FPoint, 4> m_model;
    std::array<SDL_FPoint, 4> m_transformed;
};
