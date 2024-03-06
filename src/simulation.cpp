#include "simulation.hpp"

//Constructor
Simulation::Simulation(int N) {
    this->boids = boids;
    for (int i = 0; i < N ; i++)
    {
        this->boids.emplace_back(0.0f, 0.0f, 0.01f, 0.01f);
    }
}

void Simulation::simulate() {
    cohesion();
    separation();
    alignment();
}


//privates

void Simulation::cohesion() {
    // TODO
}

void Simulation::separation() {
    // TODO
}

void Simulation::alignment() {
    // TODO
}

