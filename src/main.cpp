#include <imgui.h>
#include <cstdlib>
#include <vector>
#include "glm/fwd.hpp"
#include "glpp/glpp.hpp"
#define DOCTEST_CONFIG_IMPLEMENT
#include "../src/render/light.hpp"
#include "alea/MarkovChain.hpp"
#include "alea/RandomVariableGenerator.hpp"
#include "camera/camera.hpp"
#include "doctest/doctest.h"
#include "player/player.hpp"
#include "render/TextureBaker.hpp"
#include "render/cube.hpp"
#include "render/model.hpp"
#include "render/program.hpp"
#include "simulation/boids.hpp"
#include "simulation/simulation.hpp"

const int   nbBoids  = 100;
const float areaSize = 20.f;

int main()
{
    // Run the tests
    if (doctest::Context{}.run() != 0)
    {
        return EXIT_FAILURE;
    }

    // Actual application code
    auto ctx = p6::Context{{.title = "Boids will be boids"}};
    ctx.maximize_window();

    glEnable(GL_DEPTH_TEST);

    /*********************************
     * INITIALIZATION SIMULATION
     *********************************/
    Player player(glm::vec3(0.f, 0.f, 0.f));
    Camera camera(player.getPosition());
    float  scope = 1.5f;

    Cube       cube(areaSize, player.getPosition());
    Simulation simulation = Simulation(nbBoids, cube.getSize(), 0.03f, player.getPosition(), scope);
    bool       check      = false;
    int        timer      = 30;

    /*********************************
     * 3D INITIALIZATION
     *********************************/
    Program shader3D("../src/shaders/3D.vs.glsl", "../src/shaders/3D.fs.glsl");
    Program shaderCube("../src/shaders/cube.vs.glsl", "../src/shaders/cube.fs.glsl");

    img::Image imgBackground = p6::load_image_buffer("../assets/texture/background.png");
    img::Image imgPlayer     = p6::load_image_buffer("../assets/texture/player.png");
    img::Image imgBoid       = p6::load_image_buffer("../assets/texture/boid.jpg");
    img::Image imgSpark1     = p6::load_image_buffer("../assets/texture/spark1.png");
    img::Image imgSpark2     = p6::load_image_buffer("../assets/texture/spark2.png");
    img::Image imgSpark3     = p6::load_image_buffer("../assets/texture/spark3.png");
    img::Image imgSpark4     = p6::load_image_buffer("../assets/texture/spark4.png");
    img::Image imgRock       = p6::load_image_buffer("../assets/texture/planet.jpg");

    // UNIFORM VARIABLE
    shader3D.addUniformVariable("uMVPMatrix");
    shader3D.addUniformVariable("uMVMatrix");
    shader3D.addUniformVariable("uNormalMatrix");
    shader3D.addUniformVariable("uKd");
    shader3D.addUniformVariable("uKs");
    shader3D.addUniformVariable("uShininess");
    shader3D.addUniformVariable("uLightPos_vs");
    shader3D.addUniformVariable("uLightIntensity");
    shader3D.addUniformVariable("uLightPos2_vs");
    shader3D.addUniformVariable("uLightIntensity2");
    shader3D.addUniformVariable("uText");
    shaderCube.addUniformVariable("uTexture");
    shaderCube.addUniformVariable("uMVPMatrix");
    shaderCube.addUniformVariable("uMVMatrix");
    shaderCube.addUniformVariable("uNormalMatrix");

    // 3D MODEL
    Model planet   = Model();
    Model player3D = Model();
    Model boids3D  = Model();
    Model spark3D  = Model();

    // LOAD 3D MODEL
    player3D.loadModel("player.obj");
    boids3D.loadModel("star.obj");
    spark3D.loadModel("spark.obj");
    planet.loadModel("planet.obj");

    // TEXTURE
    GLuint              playerBake = TextureBaker::bake(imgPlayer);
    GLuint              boidBake   = TextureBaker::bake(imgBoid);
    GLuint              rockBake   = TextureBaker::bake(imgRock);
    GLuint              sparkBake1 = TextureBaker::bake(imgSpark1);
    GLuint              sparkBake2 = TextureBaker::bake(imgSpark2);
    GLuint              sparkBake3 = TextureBaker::bake(imgSpark3);
    GLuint              sparkBake4 = TextureBaker::bake(imgSpark4);
    std::vector<GLuint> textures   = {sparkBake1, sparkBake2, sparkBake3, sparkBake4};

    // VBO
    player3D.setVbo();
    boids3D.setVbo();
    planet.setVbo();
    spark3D.setVbo();

    // VAO
    player3D.setVao();
    boids3D.setVao();
    planet.setVao();
    spark3D.setVao();

    // CUBE
    cube.init(imgBackground);

    // MATRIX FOR SHADERS
    glm::mat4 ProjMatrix;

    ProjMatrix = glm::perspective(glm::radians(70.f), ctx.aspect_ratio(), 0.1f, 100.f);

    // LIGHTS
    Light lightPlayer = Light(glm::vec3(80.f));
    Light lightBoid   = Light(glm::vec3(1.f));

    // RANDOM
    RandomVariableGenerator randGen;

    // PLANETS
    int                    nbPlanets = static_cast<int>(randGen.triangular(1, 5, 10));
    std::vector<glm::vec4> planetsData;
    for (int i = 0; i < nbPlanets; i++)
    {
        // Générer des coordonnées aléatoires pour chaque axe à l'intérieur du cube
        auto xCoord     = static_cast<float>(randGen.uniformDiscrete(-static_cast<int>(areaSize) + 1, static_cast<int>(areaSize) - 1));
        auto yCoord     = static_cast<float>(randGen.uniformDiscrete(-static_cast<int>(areaSize) + 1, static_cast<int>(areaSize) - 1));
        auto zCoord     = static_cast<int>(randGen.uniformDiscrete(-static_cast<int>(areaSize) + 1, static_cast<int>(areaSize - 3)));
        auto planetSize = static_cast<float>(randGen.laplace(0.1, 0.2));
        if (planetSize < 0)
        {
            planetSize = -planetSize;
        }
        // Ajouter la position du rocher à la liste
        planetsData.emplace_back(xCoord, yCoord, zCoord, planetSize);
    }

    // MARKOV CHAIN
    std::vector<std::vector<double>> transitionMatrix = {
        {0.9, 0.25, 0.025, 0.025},
        {0.25, 0.9, 0.025, 0.025},
        {0.025, 0.025, 0.8, 0.15},
        {0.025, 0.025, 0.15, 0.8}
    };

    std::unordered_map<int, GLuint> textureMap = {
        {static_cast<int>(MarkovChainSparkState::TextureUpdate), sparkBake1},
        {static_cast<int>(MarkovChainSparkState::PositionUpdate), sparkBake2}
    };

    std::vector<int> states = {static_cast<int>(MarkovChainSparkState::TextureUpdate), static_cast<int>(MarkovChainSparkState::PositionUpdate)};

    MarkovChain markovChain(transitionMatrix, states, randGen);

    int  sparkState       = static_cast<int>((MarkovChainSparkState::TextureUpdate));
    auto playerLightState = static_cast<float>((MarkovChainLightState::LightOn));

    glEnable(GL_DEPTH_TEST);
    glm::vec3 sparkMatrix    = glm::vec3(0.0f, 0.0f, 0.0f);
    int       time           = 0;
    GLuint    currentTexture = sparkBake1;
    if (randGen.bernoulli(0.5) == 1)
    {
        currentTexture = sparkBake1;
    }
    else
    {
        currentTexture = sparkBake2;
    }

    ctx.imgui = [&]() {
        ImGui::Begin("Control Panel");

        // Section Planets
        ImGui::Text("Planets:");
        ImGui::Text("Number of Planets : %i", nbPlanets);

        // Section Player
        ImGui::Separator();
        ImGui::Text("Player:");
        ImGui::Text("Position: (%.2fx, %.2fy, %.2fz)", player.getPosition().x, player.getPosition().y, player.getPosition().z);

        // Section Boids
        ImGui::Separator();
        ImGui::Text("Boids:");
        ImGui::Checkbox("Enable Bounce", &check);
        ImGui::SliderFloat("Separation", simulation.getSeparationStrength(), 0.f, 0.5f);
        ImGui::SliderFloat("Cohesion", simulation.getCohesionStrength(), 0.f, 0.5f);
        ImGui::SliderFloat("Alignement", simulation.getAlignementStrength(), 0.f, 0.5f);
        ImGui::End();
    };

    ctx.update = [&]() {
        /*********************************
         * RENDERING
         *********************************/
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        /* *** MOVING PLAYER & BOIDS *** */
        player.move(ctx, player, camera, cube);
        simulation.simulate(cube.getSize(), check);
        glm::mat4 viewMatrix = camera.getViewMatrix();

        /* *** LIGHT *** */
        shader3D.use();
        sparkState       = markovChain.nextState(sparkState);
        playerLightState = static_cast<float>(markovChain.nextState(static_cast<int>(playerLightState)));
        lightPlayer.passToShader(shader3D, glm::vec3(148.0f / 255.0f, 203.0f / 255.0f, 246.0f / 255.0f), ProjMatrix, viewMatrix, glm::vec3(0.f));
        lightBoid.passToShader2(shader3D, glm::vec3(80.0f, 0.f, 50.f), ProjMatrix, viewMatrix, player.getPosition());

        if (time >= timer)
        {
            time = 0;
            if (sparkState == static_cast<int>(MarkovChainSparkState::TextureUpdate))
            {
                currentTexture = textures[randGen.hypergeometric(static_cast<int>(textures.size()), 2, 1)];
            }
            else
            {
                sparkMatrix = glm::vec3(randGen.normal(2.5, 2), randGen.normal(2.5, 2), randGen.normal(2.5, 2));
            }
            if (sparkState == static_cast<int>(MarkovChainLightState::LightOn))
            {
                lightPlayer = Light(glm::vec3(static_cast<float>(randGen.exponential(0.1))));
            }
            else
            {
                lightPlayer = Light(glm::vec3(80.f));
            }
            timer = randGen.geometric(0.01);
        }
        time++;
        spark3D.draw(sparkMatrix, glm::vec3{1.}, 0, glm::vec3(1.f), ProjMatrix, viewMatrix, shader3D, currentTexture);
        for (auto rockPosition : planetsData)
        {
            planet.draw(glm::vec3{rockPosition[0], rockPosition[1], rockPosition[2]}, glm::vec3{rockPosition[3]}, 0, glm::vec3(1.f), ProjMatrix, viewMatrix, shader3D, rockBake);
        }

        player3D.draw(player.getPosition(), glm::vec3{1.}, -100, glm::vec3(0.0f, 1.0f, 0.0f), ProjMatrix, viewMatrix, shader3D, playerBake);

        for (Boid& boid : simulation.getBoids())
        {
            shader3D.use();
            boids3D.draw(boid.getPosition(), glm::vec3{3.}, boid.getAngle(), glm::vec3(1.f), ProjMatrix, viewMatrix, shader3D, boidBake);
        }
        shaderCube.use();
        cube.draw(cube.getPosition(), glm::vec3{1}, shaderCube, viewMatrix, ProjMatrix);
    };

    // Should be done last. It starts the infinite loop.
    ctx.start();

    // delete 3D model
    player3D.~Model();

    /*glDeleteTextures(1, &tex);
    glDeleteBuffers(1, &vbo);
    glDeleteVertexArrays(1, &vao);*/
    cube.~Cube();

    shaderCube.~Program();
    shader3D.~Program();
}