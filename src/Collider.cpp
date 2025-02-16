#include <cmath>

#include "Collider.hpp"

Collider::Collider(float x, float y, float w, float h, float angle)
:m_center{ x, y }
,m_model{{ { -w/2, -h/2 }, { w/2, -h/2 }, { w/2, h/2 }, { -w/2, h/2 } }} // Clockwise
,m_angle{ angle }
{
    update();
}


void Collider::thrustForward() noexcept
{
    m_center.x += sinf(m_angle / 180.0f * g::PI) * g::thrustMultiplier;
    m_center.y -= cosf(m_angle / 180.0f * g::PI) * g::thrustMultiplier;
    update();
}

void Collider::update() noexcept
{
    for(size_t i{ 0 }; i < 4; ++i)
    	m_transformed[i] =
        {
        	(m_model[i].x * cosf(m_angle / 180.0f * g::PI)) - (m_model[i].y * sinf(m_angle / 180.0f * g::PI)) + m_center.x,
    		(m_model[i].x * sinf(m_angle / 180.0f * g::PI)) + (m_model[i].y * cosf(m_angle / 180.0f * g::PI)) + m_center.y,
    	};
}

// SAT collision detection.
bool Collider::isColliding(const Collider& target) const noexcept
{
    const Collider* c1{ &target };
    const Collider* c2{ this };

    for(size_t i{ 0 }; i <= 1; ++i)
    {
        if(i) { c1 = this; c2 = &target; }

        for(size_t j{ 0 }; j < 4; ++j)
        {
            size_t a{ (j + 1) % 4 };
            SDL_FPoint axisProjection{ -(c1->m_transformed[a].y - c1->m_transformed[j].y), (c1->m_transformed[a].x - c1->m_transformed[j].x) };
            float d{ sqrtf(axisProjection.x * axisProjection.x + axisProjection.y * axisProjection.y) };
            axisProjection = { axisProjection.x / d, axisProjection.y / d };

            float minPoint1{ INFINITY };
            float maxPoint1{ -INFINITY };
            for(size_t k{ 0 }; k < 4; ++k)
            {
                float dot{ (c1->m_transformed[k].x * axisProjection.x + c1->m_transformed[k].y * axisProjection.y) };
                minPoint1 = std::min(minPoint1, dot);
                maxPoint1 = std::max(maxPoint1, dot);
            }

            float minPoint2{ INFINITY };
            float maxPoint2{ -INFINITY };
            for(size_t k{ 0 }; k < 4; ++k)
            {
                float dot{ (c2->m_transformed[k].x * axisProjection.x + c2->m_transformed[k].y * axisProjection.y) };
                minPoint2 = std::min(minPoint2, dot);
                maxPoint2 = std::max(maxPoint2, dot);
            }
            if(!(maxPoint2 >= minPoint1 && maxPoint1 >= minPoint2)) { return false; }
        }
    }

    return true;
}

// SAT collision detection.
bool Collider::checkCollisionAndResolve(Collider& target) noexcept
{
    const Collider* c1{ &target };
    const Collider* c2{ this };
    float overlap{ INFINITY };

    for(size_t i{ 0 }; i <= 1; ++i)
    {
        if(i) { c1 = this; c2 = &target; }

        for(size_t j{ 0 }; j < 4; ++j)
        {
            size_t a{ (j + 1) % 4 };
            SDL_FPoint axisProjection{ -(c1->m_transformed[a].y - c1->m_transformed[j].y), (c1->m_transformed[a].x - c1->m_transformed[j].x) };
            float d{ sqrtf(axisProjection.x * axisProjection.x + axisProjection.y * axisProjection.y) };
            axisProjection = { axisProjection.x / d, axisProjection.y / d };

            float minPoint1{ INFINITY };
            float maxPoint1{ -INFINITY };
            for(size_t k{ 0 }; k < 4; ++k)
            {
                float dot{ (c1->m_transformed[k].x * axisProjection.x + c1->m_transformed[k].y * axisProjection.y) };
                minPoint1 = std::min(minPoint1, dot);
                maxPoint1 = std::max(maxPoint1, dot);
            }

            float minPoint2{ INFINITY };
            float maxPoint2{ -INFINITY };
            for(size_t k{ 0 }; k < 4; ++k)
            {
                float dot{ (c2->m_transformed[k].x * axisProjection.x + c2->m_transformed[k].y * axisProjection.y) };
                minPoint2 = std::min(minPoint2, dot);
                maxPoint2 = std::max(maxPoint2, dot);
            }
            if(!(maxPoint2 >= minPoint1 && maxPoint1 >= minPoint2)) { return false; }
            overlap = std::min(std::min(maxPoint1, maxPoint2) - std::max(minPoint1, minPoint2), overlap);
        }
    }

    // Vector between the two center points.
    SDL_FPoint vec{ target.m_center.x - m_center.x, target.m_center.y - m_center.y };
    float d{ sqrtf(vec.x*vec.x + vec.y*vec.y) };
    vec.x /= d; vec.y /= d;
    overlap /= 2;
    target.m_center.x += overlap * vec.x;
    target.m_center.y += overlap * vec.y;
    m_center.x -= overlap * vec.x;
    m_center.y -= overlap * vec.y;
    update();
    target.update();
    return true;
}

// SAT collision detection.
bool Collider::checkCollisionAndResolve(const Collider& target) noexcept
{
    const Collider* c1{ &target };
    const Collider* c2{ this };
    float overlap{ INFINITY };

    for(size_t i{ 0 }; i <= 1; ++i)
    {
        if(i) { c1 = this; c2 = &target; }

        for(size_t j{ 0 }; j < 4; ++j)
        {
            size_t a{ (j + 1) % 4 };
            SDL_FPoint axisProjection{ -(c1->m_transformed[a].y - c1->m_transformed[j].y), (c1->m_transformed[a].x - c1->m_transformed[j].x) };
            float d{ sqrtf(axisProjection.x * axisProjection.x + axisProjection.y * axisProjection.y) };
            axisProjection = { axisProjection.x / d, axisProjection.y / d };

            float minPoint1{ INFINITY };
            float maxPoint1{ -INFINITY };
            for(size_t k{ 0 }; k < 4; ++k)
            {
                float dot{ (c1->m_transformed[k].x * axisProjection.x + c1->m_transformed[k].y * axisProjection.y) };
                minPoint1 = std::min(minPoint1, dot);
                maxPoint1 = std::max(maxPoint1, dot);
            }

            float minPoint2{ INFINITY };
            float maxPoint2{ -INFINITY };
            for(size_t k{ 0 }; k < 4; ++k)
            {
                float dot{ (c2->m_transformed[k].x * axisProjection.x + c2->m_transformed[k].y * axisProjection.y) };
                minPoint2 = std::min(minPoint2, dot);
                maxPoint2 = std::max(maxPoint2, dot);
            }
            if(!(maxPoint2 >= minPoint1 && maxPoint1 >= minPoint2)) { return false; }
            overlap = std::min(std::min(maxPoint1, maxPoint2) - std::max(minPoint1, minPoint2), overlap);
        }
    }
    // Vector between the two center points.
    SDL_FPoint vec{ target.m_center.x - m_center.x, target.m_center.y - m_center.y };
    float d{ sqrtf(vec.x*vec.x + vec.y*vec.y) };
    vec.x /= d; vec.y /= d;
    m_center.x -= overlap * vec.x;
    m_center.y -= overlap * vec.y;
    update();
    return true;
}
