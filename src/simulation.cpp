#include "simulation.hpp"

// Constructor
Simulation::Simulation(int N, float areaSize, float size)
{
    this->boids = boids;
    for (int i = 0; i < N; i++)
    {
        float x = areaSize * (2 * static_cast<float>(rand()) / static_cast<float>(RAND_MAX) - 1);
        float y = areaSize * (2 * static_cast<float>(rand()) / static_cast<float>(RAND_MAX) - 1);
        this->boids.emplace_back(x, y, 0.001f, 0.001f, 1.0f, 1.0f);
        m_sizeBoid = size;
    }
}

void Simulation::draw(p6::Context& ctx)
{
    for (Boid& b : this->boids)
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
    for (Boid& b : this->boids)
    {
        b.move();
        if (check)
            b.bounce(areaSize, m_sizeBoid, m_strengths.boundsStrength, m_boidScope);
        else
            b.noBounce(areaSize);

        separation(b, m_boidScope, m_strengths.separationStrength);
    }
    /*cohesion();
    separation();
    alignment();*/
}

// privates

void Simulation::separation(Boid& currentBoid, const float scope, const float strength)
{
    glm::vec2 totalForce = {0., 0.};
    int       neighbor   = 0;

    for (Boid& b : this->boids)
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
        glm::vec2 direction = currentBoid.getDirection();
        totalForce /= static_cast<float>(neighbor);
        currentBoid.setDirection(direction += totalForce);
        currentBoid.setDirection(glm::normalize(currentBoid.getDirection()));
    }
}

// GETTERS
float* Simulation::getSeparationStrength()
{
    return &m_strengths.separationStrength;
}