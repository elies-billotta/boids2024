#include "glm/fwd.hpp"
#include "p6/p6.h"

#pragma once

class Boid {
public:
    
    glm::vec2 position;

    // Constructor
    Boid(float x, float y, float vx, float vy);

    // Methods
    void update();
    void draw(p6::Context& ctx, float areaSize);
    
private:
    glm::vec2 velocity;
};
