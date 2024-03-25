#include <vector>
#include "Strengths.hpp"
#include "boids.hpp"
#include "glm/fwd.hpp"
#include "p6/p6.h"

#pragma once

class Simulation {
public:
    std::vector<Boid> boids;

    // Constructor
    Simulation(int N, float areaSize, float size);

    // Methods
    void draw(p6::Context& ctx);
    void simulate(float areaSize, bool check);

private:
    void      cohesion();
    void      separation();
    void      alignment();
    float     m_sizeBoid;
    Strengths m_strengths;
};
