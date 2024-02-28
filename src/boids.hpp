#include "glm/fwd.hpp"
#include "p6/p6.h"

#pragma once

class Boid {
public:
    // Constructor
    Boid(float x, float y, float vx, float vy);

    // Destructor
    ~Boid();

    // Methods
    void update();
    void draw(p6::Context& ctx);

private:
    glm::vec2 position;
    glm::vec2 velocity;
};
