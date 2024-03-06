#include "simulation.hpp"

// Constructor
Simulation::Simulation(int N, float areaSize)
{
    this->boids = boids;
    for (int i = 0; i < N; i++)
    {
        float x = areaSize * (2 * static_cast<float>(rand()) / static_cast<float>(RAND_MAX) - 1);
        float y = areaSize * (2 * static_cast<float>(rand()) / static_cast<float>(RAND_MAX) - 1);
        this->boids.emplace_back(x, y, 0.001f, 0.001f);
    }
}

void Simulation::simulate()
{
    cohesion();
    separation();
    alignment();
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
