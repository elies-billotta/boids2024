#include "simulation.hpp"
#include "p6/p6.h"

// Constructor
Simulation::Simulation(int nbBoids, float areaSize, glm::vec3 positionCube, float scope)
    : m_positionCube(positionCube), m_wallsScope(0.0f)
{
    glm::vec3 position = {0., 1., -10.};
    glm::vec3 velocity = {0.5f, 0.5f, 0.2f};
    velocity *= 0.1f;

    for (int i = 0; i < nbBoids; i++)
    {
        glm::vec3 direction = {p6::random::direction(), 1.};

        float x_Coord     = p6::random::number(-areaSize + positionCube.x, areaSize + positionCube.x);
        float y_Coord     = p6::random::number(-areaSize + positionCube.y, areaSize + positionCube.y);
        float z_Coord     = p6::random::number(-areaSize + positionCube.z, areaSize + positionCube.z);
        float randomAngle = p6::random::number(0, 360);

        position = {x_Coord, y_Coord, z_Coord};

        this->m_boids.emplace_back(position, velocity, direction, randomAngle);
        m_positionCube = positionCube;
        m_scope        = scope;
    }
}

void Simulation::simulate(float areaSize, bool check)
{
    for (Boid& boid : this->m_boids)
    {
        boid.move();
        if (check)
        {
            boid.bounce(areaSize, m_strengths.boundsStrength, m_positionCube, m_scope);
        }
        else
        {
            boid.noBounce(areaSize, m_positionCube);
        }
        separation(m_scope, boid, m_strengths.separationStrength);
        cohesion(m_scope, boid, m_strengths.cohesionStrength);
        alignement(m_scope, boid, m_strengths.alignementStrength);
    }
}

/* *** GETTERS *** */
std::vector<Boid> Simulation::getBoids() const
{
    return this->m_boids;
}

// privates

void Simulation::separation(const float scope, Boid& currentBoid, const float strength)
{
    glm::vec3 totalForce = {0., 0., 0.};
    int       neighbor   = 0;

    for (Boid& boid : this->m_boids)
    {
        if (&currentBoid == &boid)
        {
            continue;
        }
        float distance = glm::distance(currentBoid.getPosition(), boid.getPosition());
        if (distance < scope)
        {
            totalForce += strength * (currentBoid.getPosition() - boid.getPosition()) / distance;
            neighbor++;
        }
    }

    if (neighbor > 0)
    {
        glm::vec3 direction = currentBoid.getDirection();
        totalForce /= static_cast<float>(neighbor);
        currentBoid.setDirection(direction += totalForce);
        currentBoid.setDirection(glm::normalize(currentBoid.getDirection()));
    }
}

void Simulation::cohesion(const float scope, Boid& currentBoid, const float strength)
{
    glm::vec3 averagePosition = {0.f, 0.f, 0.f};
    int       neighbor        = 0;

    for (Boid& boid : this->m_boids)
    {
        if (&currentBoid == &boid)
        {
            continue;
        }
        const float distance = glm::distance(currentBoid.getPosition(), boid.getPosition());
        if (distance < scope)
        {
            averagePosition += boid.getPosition();
            neighbor++;
        }
    }
    if (neighbor > 0)
    {
        glm::vec3 direction = currentBoid.getDirection();
        averagePosition /= neighbor;
        currentBoid.setDirection(direction += ((averagePosition - currentBoid.getPosition()) * strength));
        currentBoid.setDirection(glm::normalize(currentBoid.getDirection()));
    }
}

void Simulation::alignement(const float scope, Boid& currentBoid, const float strength)
{
    glm::vec3 averageDirection = {0.f, 0.f, 0.f};

    int neighbor = 0;

    for (Boid& boid : this->m_boids)
    {
        if (&currentBoid == &boid)
        {
            continue;
        }
        const float distance = glm::distance(currentBoid.getPosition(), boid.getPosition());

        if (distance < scope)
        {
            averageDirection += boid.getDirection();
            neighbor++;
        }
    }

    if (neighbor > 0)
    {
        glm::vec3 direction = currentBoid.getDirection();
        averageDirection /= neighbor;
        currentBoid.setDirection(direction += averageDirection * strength);
        currentBoid.setDirection(glm::normalize(currentBoid.getDirection()));
    }
}

// GETTERS
float* Simulation::getSeparationStrength()
{
    return &m_strengths.separationStrength;
}

float* Simulation::getCohesionStrength()
{
    return &m_strengths.cohesionStrength;
}

float* Simulation::getAlignementStrength()
{
    return &m_strengths.alignementStrength;
}