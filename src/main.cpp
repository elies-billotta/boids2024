#include <imgui.h>
#include <cstdlib>
#include <iostream>
#include <vector>
#include "glm/fwd.hpp"
#include "glm/glm.hpp"
#include "glm/gtc/type_ptr.hpp"
#define DOCTEST_CONFIG_IMPLEMENT
#include "../src-common/glimac/common.hpp"
#include "../src-common/glimac/cone_vertices.hpp"
#include "../src-common/glimac/default_shader.hpp"
#include "../src-common/glimac/sphere_vertices.hpp"
#include "../src/render/light.hpp"
#include "alea/MarkovChain.hpp"
#include "alea/RandomVariableGenerator.hpp"
#include "camera/camera.hpp"
#include "doctest/doctest.h"
#include "glm/fwd.hpp"
#include "glm/gtc/type_ptr.hpp"
#include "p6/p6.h"
#include "player/player.hpp"
#include "render/cube.hpp"
#include "render/model.hpp"
#include "render/program.hpp"
#include "simulation/boids.hpp"
#include "simulation/simulation.hpp"

const int       N     = 100;
const float     speed = 0.01f;
const glm::vec3 posPlayer(0., 0., 0.);
// const glm::vec3 posCube(0., -5., -5.);
int    timer  = 30;
const double lambda = 0.1;
int          main()
{
    // Run the tests
    if (doctest::Context{}.run() != 0)
        return EXIT_FAILURE;

    // Actual application code
    auto ctx = p6::Context{{.title = "Boids will be boids"}};
    ctx.maximize_window();

    glEnable(GL_DEPTH_TEST);

    /*********************************
     * INITIALIZATION SIMULATION
     *********************************/
    Cube       cube(20.0f, posPlayer);
    Simulation simulation = Simulation(N, cube.getSize(), 0.03f, posPlayer);
    ImVec4     namedColor = ImVec4(0.4f, 0.7f, 0.0f, 1.0f);
    bool       check      = false;

    ctx.imgui = [&]() {
        ImGui::Begin("Test");
        // ImGui::SliderFloat("Square size", &areaSize, 0.f, 1.f);
        ImGui::ColorPicker4("Color", (float*)&namedColor);
        ImGui::Checkbox("Bounce", &check);
        ImGui::SliderFloat("Separation", simulation.getSeparationStrength(), 0.f, 0.5f);
        ImGui::SliderFloat("Cohesion", simulation.getCohesionStrength(), 0.f, 0.5f);
        ImGui::SliderFloat("Alignement", simulation.getAlignementStrength(), 0.f, 0.5f);
        ImGui::End();
    };

    Player player(posPlayer);
    Camera camera(posPlayer);

    /*********************************
     * 3D INITIALIZATION
     *********************************/

    // SHADER
    /*const p6::Shader shader = p6::load_shader(
        "../src/shaders/3D.vs.glsl",
        "../src/shaders/normals.fs.glsl"
    );*/

    Program shader3D("../src/shaders/3D.vs.glsl", "../src/shaders/3D.fs.glsl");
    Program shaderCube("../src/shaders/cube.vs.glsl", "../src/shaders/cube.fs.glsl");

    img::Image imgBackground = p6::load_image_buffer("../assets/texture/background.png");
    img::Image imgPlayer     = p6::load_image_buffer("../assets/texture/player.png");
    img::Image imgBoid       = p6::load_image_buffer("../assets/texture/boid.jpg");
    img::Image imgRock       = p6::load_image_buffer("../assets/texture/rock.png");
    img::Image imgSpark1     = p6::load_image_buffer("../assets/texture/spark1.png");
    img::Image imgSpark2     = p6::load_image_buffer("../assets/texture/spark2.png");

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
    shader3D.addUniformVariable("uLightDir_vs");
    shader3D.addUniformVariable("uText");

    shaderCube.addUniformVariable("uTexture");
    shaderCube.addUniformVariable("uMVPMatrix");
    shaderCube.addUniformVariable("uMVMatrix");
    shaderCube.addUniformVariable("uNormalMatrix");

    // 3D MODEL
    Model rock     = Model();
    Model player3D = Model();
    Model boids3D  = Model();
    Model spark3D  = Model();

    // LOAD 3D MODEL
    player3D.loadModel("player.obj");
    boids3D.loadModel("star.obj");
    rock.loadModel("rock.obj");
    spark3D.loadModel("spark.obj");

    GLuint playerBake;
    glGenTextures(1, &playerBake);
    glBindTexture(GL_TEXTURE_2D, playerBake);

    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, imgPlayer.width(), imgPlayer.height(), 0, GL_RGBA, GL_UNSIGNED_BYTE, imgPlayer.data());
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glBindTexture(GL_TEXTURE_2D, 0);

    GLuint boidBake;
    glGenTextures(1, &boidBake);
    glBindTexture(GL_TEXTURE_2D, boidBake);

    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, imgBoid.width(), imgBoid.height(), 0, GL_RGBA, GL_UNSIGNED_BYTE, imgBoid.data());
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glBindTexture(GL_TEXTURE_2D, 0);

    GLuint rockBake;
    glGenTextures(1, &rockBake);
    glBindTexture(GL_TEXTURE_2D, rockBake);

    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, imgRock.width(), imgRock.height(), 0, GL_RGBA, GL_UNSIGNED_BYTE, imgRock.data());
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glBindTexture(GL_TEXTURE_2D, 0);

    GLuint sparkBake1, sparkBake2;
    glGenTextures(1, &sparkBake1);
    glBindTexture(GL_TEXTURE_2D, sparkBake1);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, imgSpark1.width(), imgSpark1.height(), 0, GL_RGBA, GL_UNSIGNED_BYTE, imgSpark1.data());
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glBindTexture(GL_TEXTURE_2D, 0);

    // Génération de la texture sparkBake2
    glGenTextures(1, &sparkBake2);
    glBindTexture(GL_TEXTURE_2D, sparkBake2);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, imgSpark2.width(), imgSpark2.height(), 0, GL_RGBA, GL_UNSIGNED_BYTE, imgSpark2.data());
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glBindTexture(GL_TEXTURE_2D, 0); // Délier la texture

    // VBO
    player3D.setVbo();
    boids3D.setVbo();
    rock.setVbo();
    spark3D.setVbo();

    // VAO
    player3D.setVao();
    boids3D.setVao();
    rock.setVao();
    spark3D.setVao();

    // CUBE
    cube.init(imgBackground);

    /* MATRIX FOR SHADERS*/
    glm::mat4 ProjMatrix;
    glm::mat4 MVMatrix;
    glm::mat4 NormalMatrix;

    ProjMatrix = glm::perspective(glm::radians(70.f), ctx.aspect_ratio(), 0.1f, 100.f);

    // RANDOM
    RandomVariableGenerator randGen;

    /* *** LIGHTS *** */
    Light lightSun = Light(glm::vec3(randGen.exponential(lambda)));

    // ROCKS
    float rockSize;
    int   nbRock = randGen.uniformDiscrete(1, 100);
    // Taille de votre cube
    float cubeSize = 5.0f;
    // Position des rochers générés aléatoirement
    std::vector<glm::vec3> rockPositions;
    for (int i = 0; i < nbRock; i++)
    {
        rockSize = static_cast<float>(randGen.normal(0.9, 0.1));
        if (rockSize < 0.)
            rockSize = -rockSize;
        // Générer des coordonnées aléatoires pour chaque axe à l'intérieur du cube
        float x = randGen.triangular(-cubeSize*2, cubeSize*2, cubeSize*2);
        float y = randGen.triangular(-cubeSize*2, cubeSize*2, cubeSize*2);
        float z = randGen.triangular(-cubeSize*2, cubeSize*2, cubeSize*2);

        // Ajouter la position du rocher à la liste
        rockPositions.emplace_back(x, y, z);
    }

    // MARKOV CHAIN
    // Créer une matrice de transition pour gérer les changements d'état entre "Texture1" et "Texture2"
    std::vector<std::vector<double>> transitionMatrix = {
        {0.9, 0.1, 0.0, 0.0}, // Probabilités de transition de l'état 0 vers les autres états
        {0.1, 0.0, 0.0, 0.0}, // Probabilités de transition de l'état 1 vers les autres états
        {0.0, 0.0, 0.3, 0.0}, // Probabilités de transition de l'état 2 vers les autres états
        {0.0, 0.4, 0.2, 0.4}  // Probabilités de transition de l'état 3 vers les autres états
    };

    std::unordered_map<int, GLuint> textureMap = {
        {static_cast<int>(MarkovChainTextureState::Texture1), sparkBake1},
        {static_cast<int>(MarkovChainTextureState::Texture2), sparkBake2}
    };

    std::vector<int> states = {static_cast<int>(MarkovChainTextureState::Texture1), static_cast<int>(MarkovChainTextureState::Texture2)}; // États possibles de la chaîne de Markov

    // markovChain constructor
    MarkovChain markovChain(transitionMatrix, states, randGen);

    int   sparkState          = static_cast<int>((MarkovChainTextureState::Texture1)); // Supposons que 0 est l'état initial
    float sparkDirectionState = static_cast<float>((MarkovChainDirection::Left));      // Supposons que 0 est l'état initial

    glEnable(GL_DEPTH_TEST);
    glm::vec3 sparkMatrix = glm::vec3(0.0f, 0.0f, 0.0f);
    // Declare your infinite update loop.
    int    time           = 0;
    int    timeLight      = 0;
    GLuint currentTexture = sparkBake1;
    ctx.update            = [&]() {
        /*********************************
         * RENDERING
         *********************************/
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        /* *** MOVING PLAYER & BOIDS *** */
        player.move(ctx, player, camera, cube);
        simulation.simulate(cube.getSize(), check);

        // std::cout << "Camera position: " << camera.getPosition().x << " " << camera.getPosition().y << " " << camera.getPosition().z << std::endl;

        glm::mat4 viewMatrix = camera.getViewMatrix();

        /* *** LIGHT *** */
        shader3D.use();
        if (timeLight >= timer + 500)
        {
            lightSun = Light(glm::vec3(randGen.exponential(lambda)));
            timeLight = 0;
        }
        std::cout << "PLayer position " << player.getPosition().x << " " << player.getPosition().y << " " << player.getPosition().z << std::endl;
        lightSun.passToShader(shader3D, ProjMatrix, viewMatrix, player.getPosition());
        timeLight++;
        // Utiliser la chaîne de Markov pour déterminer l'état actuel des sparks

        sparkState          = markovChain.nextState(sparkState);
        sparkDirectionState = markovChain.nextState(sparkDirectionState);

        if (time >= timer)
        {
            time     = 0;
            // Mettre à jour la texture et la direction des sparks en fonction de leur état
            if (sparkState == static_cast<int>(MarkovChainTextureState::Texture1))
            {
                currentTexture = sparkBake1;
            }
            else
            {
                currentTexture = sparkBake2;
            }
            if (sparkState == static_cast<int>(MarkovChainDirection::Left))
            {
                sparkMatrix = glm::vec3(-1.0f, 0.0f, 0.0f);
            }
            else
                sparkMatrix = glm::vec3(1.0f, 0.0f, 0.0f);
            timer = randGen.geometric(0.1);
        }
        time++;
        spark3D.draw(sparkMatrix, glm::vec3{1.}, 0, glm::vec3(1.f), ProjMatrix, viewMatrix, shader3D, currentTexture);

        // Dessiner chaque rocher à sa position respective
        for (auto rockPosition : rockPositions)
        {
            rock.draw(rockPosition, glm::vec3{rockSize}, 0, glm::vec3(1.f), ProjMatrix, viewMatrix, shader3D, rockBake);
        }

        player3D.draw(player.getPosition(), glm::vec3{1.}, -100, glm::vec3(0.0f, 1.0f, 0.0f), ProjMatrix, viewMatrix, shader3D, playerBake);

        for (Boid& b : simulation.getBoids())
        {
            boids3D.draw(b.getPosition(), glm::vec3{3.}, b.getAngle(), glm::vec3(1.f), ProjMatrix, viewMatrix, shader3D, boidBake);
        }
        shaderCube.use();
        cube.draw(posPlayer, glm::vec3{1}, shaderCube, viewMatrix, ProjMatrix);

        // std::cout << "x" << cube.getCubePosition().x << "y" << cube.getCubePosition().y << "z" << cube.getCubePosition().z << std::endl;

        /*glDrawArrays(GL_TRIANGLES, 0, vertices.size());

        glBindVertexArray(0);*/
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