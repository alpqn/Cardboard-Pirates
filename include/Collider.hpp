#pragma once

#include <array>

#include <SDL2/SDL.h>

#include "GameUtils.hpp"

class Collider
{
private:
    float m_angle;
    SDL_FPoint m_center;
    std::array<SDL_FPoint, 4> m_model;
    std::array<SDL_FPoint, 4> m_transformed;
public:
    Collider(float x, float y, float w, float h, float angle);

    void thrustForward();
    void rotateLeft() { m_angle -= g::rotationMultiplier; update(); }
    void rotateRight() { m_angle += g::rotationMultiplier; update(); }
    void update();
    bool isColliding(const Collider& target) const;
    // Only the caller is affected by the collision resolution
    bool checkCollisionAndResolve(const Collider& target);
    // Both the caller and the target are affected by the collision resolution
    bool checkCollisionAndResolve(Collider& target);

    float getX() const { return m_center.x; }
    float getY() const { return m_center.y; }
    float getAngle() const { return m_angle; }
    const std::array<SDL_FPoint, 4> getTransformed() const { return m_transformed; }

    void setX(float x) { m_center.x = x; update(); }
    void setY(float y) { m_center.y = y; update(); }
    void setAngle(float angle) { m_angle = angle; }
};
