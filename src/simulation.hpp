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

    // GETTERS
    float* getSeparationStrength();
    float* getCohesionStrength();
    float* getAlignementStrength();

private:
    void separation(Boid& currentBoid, const float scope, const float strength);
    // void      cohesion(Boid& currentBoid, const float scope, const float strength);
    void      cohesion(Boid& currentBoid, const float scope, const float strength);
    void      alignement(Boid& currentBoid, const float scope, const float strength);
    float     m_sizeBoid;
    Strengths m_strengths;
    float     m_boidScope = 0.09f;
    float     m_wallsScope;
};
