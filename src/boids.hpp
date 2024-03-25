#include "glm/fwd.hpp"
#include "p6/p6.h"

#pragma once

class Boid {
public:
    // Constructor
    Boid(float x, float y, float vx, float vy, float dx, float dy);

    // Methods
    void move();
    void noBounce(float areaSize);
    void bounce(float areaSize, const float& size, const float& strength);

    // GETTERS
    glm::vec2 getPosition();

private:
    glm::vec2 m_position;
    glm::vec2 m_direction;
    glm::vec2 m_velocity;
};
