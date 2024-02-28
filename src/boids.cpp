#include "boids.hpp"

// Constructor
Boid::Boid(float x, float y, float vx, float vy)
{
    position.x = x;
    position.y = y;
    velocity.x = vx;
    velocity.y = vy;
}

// Destructor
Boid::~Boid()
{
}

// Methods
void Boid::update()
{
    position.x += velocity.x;
    position.y += velocity.y;
}

void Boid::draw(p6::Context& ctx)
{
    ctx.triangle(
        p6::Point2D{position.x-0.1f, position.y-0.1f},
        p6::Point2D{position.x + 0.1f, position.y + 0.1f},
        p6::Point2D{position.x - 0.1f, position.y + 0.1f}
    );
}
