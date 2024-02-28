#include <cstdlib>
#include <vector>
#define DOCTEST_CONFIG_IMPLEMENT
#include "boids.hpp"
#include "doctest/doctest.h"
#include "p6/p6.h"

const int N = 100;
const float areaSize = 0.8f;

int main()
{
    // Run the tests
    if (doctest::Context{}.run() != 0)
        return EXIT_FAILURE;

    // Actual application code
    auto ctx = p6::Context{{.title = "Boids will be boids"}};
    ctx.maximize_window();
    Boid boid1 = Boid(0.0f, 0.0f, 0.00001f, 0.00001f);

    std::vector<Boid> boids;

    for (int i = 0; i < N; i++)
    {
        boids.emplace_back(0.0f, 0.0f, 0.01f, 0.01f);
    }
    ctx.update = [&]() {
        ctx.background(p6::NamedColor::Red);
        ctx.square(p6::Center(0.0f, 0.0f), p6::Radius(areaSize));
        for (Boid& b : boids)
        {
            b.update();
            b.draw(ctx, areaSize);
            
        }
    };
    ctx.start();
}