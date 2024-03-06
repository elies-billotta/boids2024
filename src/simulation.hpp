#include <vector>
#include "boids.hpp"
#include "glm/fwd.hpp"
#include "p6/p6.h"

#pragma once

class Simulation {
public:
    std::vector<Boid> boids;

    // Constructor
    Simulation(int N, float areaSize);

    void simulate();

private:
    void cohesion();
    void separation();
    void alignment();
};
