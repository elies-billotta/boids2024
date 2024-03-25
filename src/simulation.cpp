#include "simulation.hpp"

// Constructor
Simulation::Simulation(int N, float areaSize, float size, float strength)
{
    this->boids = boids;
    for (int i = 0; i < N; i++)
    {
        float x = areaSize * (2 * static_cast<float>(rand()) / static_cast<float>(RAND_MAX) - 1);
        float y = areaSize * (2 * static_cast<float>(rand()) / static_cast<float>(RAND_MAX) - 1);
        this->boids.emplace_back(x, y, 0.001f, 0.001f, 1.0f, 1.0f);
        m_size     = size;
        m_strength = strength;
    }
}

void Simulation::draw(p6::Context& ctx)
{
    for (Boid& b : this->boids)
    {
        ctx.circle(p6::Center(b.getPosition().x, b.getPosition().y), p6::Radius(m_size));
    }
}

void Simulation::simulate(float areaSize)
{
    for (Boid& b : this->boids)
    {
        b.move();
        // b.noBounce(areaSize);
        b.bounce(areaSize, m_size, m_strength);
    }
    /*cohesion();
    separation();
    alignment();*/
}

// privates

void Simulation::cohesion()
{
    // TODO
}

void Simulation::separation()
{
    // TODO
}

void Simulation::alignment()
{
    // TODO
}
