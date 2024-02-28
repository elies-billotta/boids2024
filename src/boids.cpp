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

void Boid::draw(p6::Context& ctx, float areaSize)
{
    ctx.circle(p6::Center(position.x-0.03f, position.y-0.03f), p6::Radius(0.03f));
    if (position.x > areaSize)
    {
        position.x = -areaSize;
    }
    if (position.x < -areaSize)
    {
        position.x = areaSize;
    }
    if (position.y > areaSize)
    {
        position.y = -areaSize;
    }
    if (position.y < -areaSize)
    {
        position.y = areaSize;
    }
}
