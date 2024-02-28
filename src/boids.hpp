#include "glm/fwd.hpp"
#include "p6/p6.h"

#pragma once

class Boid {
public:
    // Constructor
    glm::vec2 position;

    Boid(float x, float y, float vx, float vy);

    // Destructor
    ~Boid();

    // Methods
    void update();
    void draw(p6::Context& ctx, float areaSize);
    
private:
    glm::vec2 velocity;
};
