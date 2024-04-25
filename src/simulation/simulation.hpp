#pragma once

#include <vector>
#include "Strengths.hpp"
#include "boids.hpp"
#include "glm/fwd.hpp"
#include "p6/p6.h"

class Simulation {
public:
    // Constructor
    Simulation(int nbBoids, float areaSize, glm::vec3 positionCube, float scope);

    // Methods
    void draw(p6::Context& ctx);
    void simulate(float areaSize, bool check);

    // GETTERS
    std::vector<Boid> getBoids() const;
    float*            getSeparationStrength();
    float*            getCohesionStrength();
    float*            getAlignementStrength();

private:
    void              separation(float scope, Boid& currentBoid, float strength);
    void              cohesion(float scope, Boid& currentBoid, float strength);
    void              alignement(float scope, Boid& currentBoid, float strength);
    std::vector<Boid> m_boids;
    glm::vec3         m_positionCube;
    Strengths         m_strengths;
    float             m_scope;
    float             m_wallsScope;
};
