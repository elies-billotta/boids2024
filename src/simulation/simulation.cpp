#include "simulation.hpp"

// Constructor
Simulation::Simulation(int N, float areaSize, float size, glm::vec3 positionCube)
{
    glm::vec3 position = {0., 1., -10.};
    glm::vec3 velocity = {1.f, 1.f, 1.f};
    velocity *= 0.1f;

    for (int i = 0; i < N; i++)
    {
        glm::vec3 direction = {p6::random::direction(), 1.};
        /*float x = areaSize * (2 * static_cast<float>(rand()) / static_cast<float>(RAND_MAX) - 1);
        float y = areaSize * (2 * static_cast<float>(rand()) / static_cast<float>(RAND_MAX) - 1);*/
        float x = p6::random::number(-areaSize + positionCube.x, areaSize + positionCube.x);
        float y = p6::random::number(-areaSize + positionCube.y, areaSize + positionCube.y);
        float z = p6::random::number(-areaSize + positionCube.z, areaSize + positionCube.z);

        position = {x, y, z};

        // float z = p6::random::number(-m_squareSize.z + m_size, m_squareSize.z - m_size);

        this->m_boids.emplace_back(position, velocity, direction);
        m_sizeBoid     = size;
        m_positionCube = positionCube;
    }
}

void Simulation::draw(p6::Context& ctx)
{
    for (Boid& b : this->m_boids)
    {
        ctx.stroke = p6::Color{0.0f, 0.0f, 0.0f};
        ctx.circle(p6::Center(b.getPosition().x, b.getPosition().y), p6::Radius(m_sizeBoid));
        // to see the scope of the boids
        ctx.stroke = p6::Color{1.0f, 0.0f, 0.0f};
        ctx.circle(p6::Center(b.getPosition().x, b.getPosition().y), p6::Radius(m_sizeBoid + m_boidScope));
    }
}

void Simulation::simulate(float areaSize, bool check)
{
    for (Boid& b : this->m_boids)
    {
        b.move();
        if (check)
            b.bounce(areaSize, m_sizeBoid, m_strengths.boundsStrength, m_boidScope, m_positionCube);
        else
            b.noBounce(areaSize, m_positionCube);

        separation(b, m_boidScope, m_strengths.separationStrength);
        cohesion(b, m_boidScope, m_strengths.cohesionStrength);
        alignement(b, m_boidScope, m_strengths.alignementStrength);
    }
}

/* *** GETTERS *** */
std::vector<Boid> Simulation::getBoids()
{
    return this->m_boids;
}

// privates

void Simulation::separation(Boid& currentBoid, const float scope, const float strength)
{
    glm::vec3 totalForce = {0., 0., 0.};
    int       neighbor   = 0;

    for (Boid& b : this->m_boids)
    {
        if (&currentBoid == &b)
            continue;

        float distance = glm::distance(currentBoid.getPosition(), b.getPosition());
        if (distance < scope)
        {
            totalForce += strength * (currentBoid.getPosition() - b.getPosition()) / distance;
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

void Simulation::cohesion(Boid& currentBoid, const float scope, const float strength)
{
    glm::vec3 averagePosition = {0.f, 0.f, 0.f};
    int       neighbor        = 0;

    for (Boid& b : this->m_boids)
    {
        if (&currentBoid == &b)
            continue;

        const float distance = glm::distance(currentBoid.getPosition(), b.getPosition());
        if (distance < scope)
        {
            averagePosition += b.getPosition();
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

void Simulation::alignement(Boid& currentBoid, const float scope, const float strength)
{
    glm::vec3 averageDirection = {0.f, 0.f, 0.f};

    int neighbor = 0;

    for (Boid& b : this->m_boids)
    {
        if (&currentBoid == &b)
            continue;

        const float distance = glm::distance(currentBoid.getPosition(), b.getPosition());

        if (distance < scope)
        {
            averageDirection += b.getDirection();
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