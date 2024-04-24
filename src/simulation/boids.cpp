#include "boids.hpp"

// Constructor
Boid::Boid(glm::vec3 position, glm::vec3 velocity, glm::vec3 direction)
    : m_position(position), m_velocity(velocity), m_direction(direction)
{
}

// Methods
void Boid::move()
{
    m_position.x += m_direction.x * m_velocity.x;
    m_position.y += m_direction.y * m_velocity.y;
    m_position.z += m_direction.z * m_velocity.z;
}

void Boid::noBounce(float areaSize, glm::vec3 positionCube)
{
    for (int i = 0; i < 3; i++)
    {
        if (m_position[i] > areaSize + positionCube[i])
        {
            m_position[i] = -areaSize;
        }
        if (m_position[i] < -areaSize + positionCube[i])
        {
            m_position[i] = areaSize;
        }
    }
}

// The boids bounce against the walls
void Boid::bounce(float& areaSize, const float& size, const float& strength, const float& scope, glm::vec3 positionCube)
{
    glm::vec2   boundsForce = {0., 0.};
    const float maxX        = areaSize;
    const float maxY        = areaSize;
    const float minX        = -areaSize;
    const float minY        = -areaSize;

    // il faudra rajouter -size quand on aura la taille des boids
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
glm::vec3 Boid::getPosition()
{
    return m_position;
}

/*glm::vec3 Boid::getDirection()
{
    return m_direction;
}

// SETTERS
glm::vec3 Boid::setDirection(glm::vec3 direction)
{
    m_direction = direction;
}
*/