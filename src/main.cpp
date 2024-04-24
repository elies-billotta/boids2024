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
#include "alea/RandomVariableGenerator.hpp"
#include "camera/camera.hpp"
#include "doctest/doctest.h"
#include "glm/fwd.hpp"
#include "glm/gtc/type_ptr.hpp"
#include "p6/p6.h"
#include "player/player.hpp"
#include "render/cube.hpp"
#include "render/light.hpp"
#include "render/model.hpp"
#include "render/program.hpp"
#include "simulation/boids.hpp"
#include "simulation/simulation.hpp"

const int       N     = 100;
const float     speed = 0.01f;
const glm::vec3 posPlayer(0., 0., 0.);
// const glm::vec3 posCube(0., -5., -5.);

int main()
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

    Program shader3D("../src/shaders/3D.vs.glsl", "../src/shaders/multiplePointLight.fs.glsl");
    Program shaderCube("../src/shaders/2Dnuages.vs.glsl", "../src/shaders/2Dnuages.fs.glsl");

    img::Image imgBackground = p6::load_image_buffer("../assets/texture/background.png");
    img::Image imgPlayer     = p6::load_image_buffer("../assets/texture/player.png");
    img::Image imgBoid       = p6::load_image_buffer("../assets/texture/boid.jpg");
    img::Image imgRock   = p6::load_image_buffer("../assets/texture/rock.png");

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
    Model rock    = Model();
    Model player3D = Model();
    Model boids3D  = Model();

    // LOAD 3D MODEL
    player3D.loadModel("player.obj");
    boids3D.loadModel("star.obj");
    rock.loadModel("rock.obj");


    // TEXTURES
    /*GLuint playerTexture;

    glGenTextures(1, &playerTexture);
    glBindTexture(GL_TEXTURE_2D, playerTexture);

    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, imgPlayer.width(), imgPlayer.height(), 0, GL_RGBA, GL_UNSIGNED_BYTE, imgPlayer.data());
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glBindTexture(GL_TEXTURE_2D, 0);*/

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

    // VBO
    player3D.setVbo();
    boids3D.setVbo();
    rock.setVbo();

    // VAO
    player3D.setVao();
    boids3D.setVao();
    rock.setVao();

    // CUBE
    cube.init(imgBackground);

    /* MATRIX FOR SHADERS*/
    glm::mat4 ProjMatrix;
    glm::mat4 MVMatrix;
    glm::mat4 NormalMatrix;

    ProjMatrix = glm::perspective(glm::radians(70.f), ctx.aspect_ratio(), 0.1f, 100.f);

    // LIGHTS
    Light lightScene = Light(glm::vec3{50.});
    Light lightPerso = Light(glm::vec3{0.01});

    // RANDOM
    RandomVariableGenerator randGen;

    //ROCKS
    float                   rockSize;
    int                     nbRock = randGen.uniformDiscrete(1, 10);
    std::cout << nbRock << std::endl;
    // Taille de votre cube
    float cubeSize = 5.0f;
    // Position des rochers générés aléatoirement
    std::vector<glm::vec3> rockPositions;
    for (int i = 0; i < nbRock; i++)
    {
        rockSize = static_cast<float>(randGen.normal(0.3, 0.01));
        if (rockSize < 0.) rockSize = -rockSize;
        std::cout << rockSize << std::endl;
        // Générer des coordonnées aléatoires pour chaque axe à l'intérieur du cube
        float x = randGen.triangular(-cubeSize / 2, 4, cubeSize / 2);
        float y = randGen.triangular(-cubeSize / 2, 0, cubeSize / 2);
        float z = randGen.triangular(-cubeSize / 2, 0, cubeSize / 2); 
        // Ajouter la position du rocher à la liste
        rockPositions.push_back(glm::vec3(x, y, z));
    }

    for (const auto& position : rockPositions)
    {
        std::cout << position.x << " " << position.y << " " << position.z << std::endl;
    }

    glEnable(GL_DEPTH_TEST);

    // Declare your infinite update loop.
    ctx.update = [&]() {
        /*********************************
         * RENDERING
         *********************************/
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        /* *** MOVING PLAYER & BOIDS *** */
        player.move(ctx, player, camera, cube);
        simulation.simulate(cube.getSize(), check);

        // std::cout << "Camera position: " << camera.getPosition().x << " " << camera.getPosition().y << " " << camera.getPosition().z << std::endl;

        glm::mat4 viewMatrix = camera.getViewMatrix();
        shader3D.use();

        MVMatrix     = glm::translate(glm::mat4(1.0), glm::vec3(0., -5., -5.));
        MVMatrix     = viewMatrix * MVMatrix;
        NormalMatrix = glm::transpose(glm::inverse(MVMatrix));

        lightScene.drawLightScene(glm::vec3(0., 5., -5.), ProjMatrix, viewMatrix, shader3D);
        //*lightPerso.drawLightPlayer(player.getPosition(), ProjMatrix, viewMatrix, shader3D);

        /*mvp_matrix_location    = glGetUniformLocation(shader.id(), "uMVPMatrix");
        mv_matrix_location     = glGetUniformLocation(shader.id(), "uMVMatrix");
        normal_matrix_location = glGetUniformLocation(shader.id(), "uNormalMatrix");

        glUniformMatrix4fv(mvp_matrix_location, 1, GL_FALSE, glm::value_ptr(ProjMatrix * MVMatrix));
        glUniformMatrix4fv(mv_matrix_location, 1, GL_FALSE, glm::value_ptr(MVMatrix));
        glUniformMatrix4fv(normal_matrix_location, 1, GL_FALSE, glm::value_ptr(NormalMatrix));*/
        /*glm::mat4 model = glm::rotate(glm::mat4(1.0f), glm::radians(-90.0f), glm::vec3(1.0f, 0.0f, 0.0f));
        glm::mat4 view  = camera.getViewMatrix();*/

        /*shader.use();
        shader.set("view", view);
        shader.set("projection", projection);
        shader.set("model", glm::mat4(1.f));
        shader.set("camPos", camera.getViewMatrix());

        shader.set("lightColor1", lightColor1);
        shader.set("lightPosition1", lightPosition1);

        shader.set("lightColor2", lightColor2);
        shader.set("lightPosition2", lightPosition2);

        cube.Draw(shader.id());*/

        /*player.drawPlayer(glm::vec3(0., -5., -5.), viewMatrix, ProjMatrix, ShaderPoint, playerTexture);*/

        // std::cout << player.getPosition().z << std::endl;
        // Dessiner chaque rocher à sa position respective
        for (const auto& position : rockPositions)
        {
            rock.draw(position, glm::vec3{rockSize}, 0, glm::vec3(0.0f, 1.0f, 0.0f), ProjMatrix, viewMatrix, ShaderPoint, rockBake);
        }
        player3D.draw(player.getPosition(), glm::vec3{1.}, -100, glm::vec3(0.0f, 1.0f, 0.0f), ProjMatrix, viewMatrix, shader3D, playerBake);

        for (Boid& b : simulation.getBoids())
        {
            boids3D.draw(b.getPosition(), glm::vec3{3.}, b.getAngle(), glm::vec3(1.f), ProjMatrix, viewMatrix, shader3D, boidBake);
        }
        shaderCube.use();

        cube.draw(posPlayer, glm::vec3{1}, shaderCube, viewMatrix, ProjMatrix);

        // std::cout << "x" << cube.getCubePosition().x << "y" << cube.getCubePosition().y << "z" << cube.getCubePosition().z << std::endl;
        std::cout << "x" << player.getPosition().x << "y" << player.getPosition().y << "z" << player.getPosition().z << std::endl;

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