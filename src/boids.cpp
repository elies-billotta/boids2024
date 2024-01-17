#include "boids.hpp"

// Constructor
Boid::Boid(float x, float y, float vx, float vy)
{
    positionX = x;
    positionY = y;
    velocityX = vx;
    velocityY = vy;
}

// Destructor
Boid::~Boid()
{
}

// Methods
void Boid::update()
{
    positionX += velocityX;
    positionY += velocityY;
}

void Boid::draw(p6::Context& ctx)
{
    ctx.triangle(
        p6::Point2D{positionX-0.1f, positionY-0.1f},
        p6::Point2D{positionX + 0.1f, positionY + 0.1f},
        p6::Point2D{positionX - 0.1f, positionY + 0.1f}
    );
}
