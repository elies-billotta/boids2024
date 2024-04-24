#include "glm/fwd.hpp"
#include "p6/p6.h"

#pragma once

class Boid {
public:
    // Constructor
    Boid(glm::vec3 position, glm::vec3 velocity, glm::vec3 direction);

    // Methods
    void move();
    void noBounce(float areaSize, glm::vec3 positionCube);
    void bounce(float& areaSize, const float& size, const float& strength, const float& scope);

    // GETTERS
    glm::vec3 getPosition();
    /*glm::vec3 getDirection();

    // SETTERS
    glm::vec3 setDirection(glm::vec3 direction);*/

private:
    glm::vec3 m_position;
    glm::vec3 m_velocity;
    glm::vec3 m_direction;
};
