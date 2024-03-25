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
        ctx.circle(p6::Center(b.getPosition().x, b.getPosition().y), p6::Radius(m_sizeBoid));
    }
}

void Simulation::simulate(float areaSize, bool check)
{
    for (Boid& b : this->boids)
    {
        b.move();
        if (check)
            b.bounce(areaSize, m_sizeBoid, m_strengths.boundsStrength);
        else
            b.noBounce(areaSize);
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
