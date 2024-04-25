#include "glm/fwd.hpp"
#include "glm/glm.hpp"

#pragma once

class Boid {
public:
    // Constructor
    Boid(glm::vec3 position, glm::vec3 velocity, glm::vec3 direction, float randomAngle);

    // Methods
    void move();
    void noBounce(float areaSize, glm::vec3 positionCube);
    void bounce(float& areaSize, const float& strength, glm::vec3 positionCube, const float& scope);

    // GETTERS
    glm::vec3 getPosition() const;
    glm::vec3 getDirection() const ;
    float     getAngle() const;

    // SETTERS
    glm::vec3 setDirection(glm::vec3 direction);

private:
    glm::vec3 m_position;
    glm::vec3 m_velocity;
    glm::vec3 m_direction;
    float     m_angle;
};
