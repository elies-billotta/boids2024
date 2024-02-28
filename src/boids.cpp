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
    for (int i =0 ; i < 2 ; i++){
        if (position[i] > areaSize)
        {
            position[i] = -areaSize;
        }
        if (position[i] < -areaSize)
        {
            position[i] = areaSize;
        }
    } 
}
