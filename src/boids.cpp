#include "boids.hpp"

// Constructor
Boid::Boid(float x, float y, float vx, float vy, float dx, float dy)
{
    m_position.x  = x;
    m_position.y  = y;
    m_velocity.x  = vx;
    m_velocity.y  = vy;
    m_direction.x = dx;
    m_direction.y = dy;
}

// Methods
void Boid::move()
{
    m_position.x += m_direction.x * m_velocity.x;
    m_position.y += m_direction.y * m_velocity.y;
}

void Boid::noBounce(float areaSize)
{
    for (int i = 0; i < 2; i++)
    {
        if (m_position[i] > areaSize)
        {
            m_position[i] = -areaSize;
        }
        if (m_position[i] < -areaSize)
        {
            m_position[i] = areaSize;
        }
    }
}

// The boids bounce against the walls
void Boid::bounce(float& areaSize, const float& size, const float& strength, const float& scope)
{
    glm::vec2   boundsForce = {0., 0.};
    const float maxX        = areaSize;
    const float maxY        = areaSize;
    const float minX        = -areaSize;
    const float minY        = -areaSize;

    if (m_position.x > areaSize - size - scope)
    {
        boundsForce.x = -glm::distance(m_position.x, maxX) * (m_position.x / maxX);
    }
    if (m_position.x < -areaSize + size + scope)
    {
        boundsForce.x = glm::distance(m_position.x, minX) * (minX / m_position.x);
    }
    if (m_position.y > areaSize - size - scope)
    {
        boundsForce.y = -glm::distance(m_position.y, maxY) * (m_position.y / maxY);
    }
    if (m_position.y < -areaSize + size + scope)
    {
        boundsForce.y = glm::distance(m_position.y, minY) * (minY / m_position.y);
    }

    m_direction += boundsForce * strength;
    m_direction = glm::normalize(m_direction);
}

// GETTERS
glm::vec2 Boid::getPosition()
{
    return m_position;
}

glm::vec2 Boid::getDirection()
{
    return m_direction;
}

// SETTERS
glm::vec2 Boid::setDirection(glm::vec2 direction)
{
    m_direction = direction;
}