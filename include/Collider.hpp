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
   // Only the caller is affected by the collision resolution
   bool checkCollisionAndResolve(const Collider& target) noexcept;
   // Both the caller and the target are affected by the collision resolution
   bool checkCollisionAndResolve(Collider& target) noexcept;

   constexpr float getX() const noexcept { return m_center.x; }
   constexpr float getY() const noexcept { return m_center.y; }
   constexpr float getAngle() const noexcept { return m_angle; }
   constexpr const std::array<SDL_FPoint, 4> getTransformed() const noexcept { return m_transformed; }

   constexpr void setX(float x) noexcept { m_center.x = x; update(); }
   constexpr void setY(float y) noexcept { m_center.y = y; update(); }
   constexpr void setAngle(float angle) noexcept { m_angle = angle; }
private:
    float m_angle;
    SDL_FPoint m_center;
    std::array<SDL_FPoint, 4> m_model;
    std::array<SDL_FPoint, 4> m_transformed;
};
