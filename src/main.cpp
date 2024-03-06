#include <imgui.h>
#include <cstdlib>
#include <vector>
#define DOCTEST_CONFIG_IMPLEMENT
#include "boids.hpp"
#include "doctest/doctest.h"
#include "simulation.hpp"

const int N = 100;

int main()
{
    auto areaSize = 0.8f;
    // Run the tests
    if (doctest::Context{}.run() != 0)
        return EXIT_FAILURE;

    // Actual application code
    auto ctx = p6::Context{{.title = "Boids will be boids"}};
    ctx.maximize_window();
    Boid        boid1       = Boid(0.0f, 0.0f, 0.00001f, 0.00001f);
    const char* items[]     = {"Cohesion", "", "Option 3"};
    int         currentItem = 0;

    std::vector<Boid> boids;
    Simulation        simulation = Simulation(N, areaSize);
    // std::string       text       = "Hello";
    ImVec4 namedColor = ImVec4(0.4f, 0.7f, 0.0f, 1.0f);

    ctx.imgui = [&]() {
        ImGui::Begin("Test");
        ImGui::SliderFloat("Square size", &areaSize, 0.f, 1.f);
        // ImGui::InputText("Text", &text);
        ImGui::ColorPicker4("Color", (float*)&namedColor);
        ImGui::Combo("##combo", &currentItem, items, IM_ARRAYSIZE(items));
        ImGui::End();
        // ImGui::ShowDemoWindow();
    };
    ctx.update = [&]() {
        ctx.background(p6::Color(namedColor.x, namedColor.y, namedColor.z, namedColor.w));
        ctx.square(p6::Center(0.0f, 0.0f), p6::Radius(areaSize));
        for (Boid& b : simulation.boids)
        {
            b.update();
            b.draw(ctx, areaSize);
        }
    };
    ctx.start();
}