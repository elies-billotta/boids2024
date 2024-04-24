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

const int       N     = 10;
const float     speed = 0.01f;
const glm::vec3 posPlayer(0., 0., 0.);
const glm::vec3 posCube(0., -5., -5.);
const float     areaSize = 20.f;

void moveListener(const p6::Context& ctx, Player& player, Camera& camera)
{
    if (ctx.key_is_pressed(GLFW_KEY_W) || ctx.key_is_pressed(GLFW_KEY_UP))
    {
        player.moveFront(speed);
    }
    if (ctx.key_is_pressed(GLFW_KEY_S) || ctx.key_is_pressed(GLFW_KEY_DOWN))
    {
        player.moveFront(-speed);
    }
    if (ctx.key_is_pressed(GLFW_KEY_A) || ctx.key_is_pressed(GLFW_KEY_LEFT))
    {
        player.moveLeft(speed);
    }
    if (ctx.key_is_pressed(GLFW_KEY_D) || ctx.key_is_pressed(GLFW_KEY_RIGHT))
    {
        player.moveLeft(-speed);
    }
    camera.setPosition(player.getPosition());
};

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

    // std::vector<Boid> boids;
    Simulation simulation = Simulation(N, areaSize, 0.03f, posPlayer);
    ImVec4     namedColor = ImVec4(0.4f, 0.7f, 0.0f, 1.0f);
    bool       check      = false;

    ctx.imgui = [&]() {
        ImGui::Begin("Test");
        // ImGui::SliderFloat("Square size", &areaSize, 0.f, 1.f);
        ImGui::ColorPicker4("Color", (float*)&namedColor);
        ImGui::Checkbox("Bounce", &check);
        /*ImGui::SliderFloat("Separation", simulation.getSeparationStrength(), 0.f, 0.5f);
        ImGui::SliderFloat("Cohesion", simulation.getCohesionStrength(), 0.f, 0.5f);
        ImGui::SliderFloat("Alignement", simulation.getAlignementStrength(), 0.f, 0.5f);*/
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

    Program ShaderPoint("../src/shaders/3D.vs.glsl", "../src/shaders/multiplePointLight.fs.glsl");
    Program shaderCube("../src/shaders/2Dnuages.vs.glsl", "../src/shaders/2Dnuages.fs.glsl");

    // IMAGE TEXTURE
    img::Image Texture   = p6::load_image_buffer("../assets/texture/background.png");
    img::Image imgPlayer = p6::load_image_buffer("../assets/texture/player.png");
    img::Image img_ile   = p6::load_image_buffer("../assets/texture/player.png");
    img::Image imgBoid   = p6::load_image_buffer("../assets/texture/boid.jpg");

    // UNIFORM VARIABLE
    ShaderPoint.addUniformVariable("uMVPMatrix");
    ShaderPoint.addUniformVariable("uMVMatrix");
    ShaderPoint.addUniformVariable("uNormalMatrix");
    ShaderPoint.addUniformVariable("uKd");
    ShaderPoint.addUniformVariable("uKs");
    ShaderPoint.addUniformVariable("uShininess");
    ShaderPoint.addUniformVariable("uLightPos_vs");
    ShaderPoint.addUniformVariable("uLightIntensity");
    ShaderPoint.addUniformVariable("uLightPos2_vs");
    ShaderPoint.addUniformVariable("uLightIntensity2");
    ShaderPoint.addUniformVariable("uText");

    shaderCube.addUniformVariable("uTexture");
    shaderCube.addUniformVariable("uMVPMatrix");
    shaderCube.addUniformVariable("uMVMatrix");
    shaderCube.addUniformVariable("uNormalMatrix");

    // 3D MODEL
    Model ile     = Model();
    Model boids3D = Model();

    // LOAD 3D MODEL
    ile.loadModel("player.obj");
    boids3D.loadModel("player.obj");

    // TEXTURES
    /*GLuint playerTexture;

    glGenTextures(1, &playerTexture);
    glBindTexture(GL_TEXTURE_2D, playerTexture);

    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, imgPlayer.width(), imgPlayer.height(), 0, GL_RGBA, GL_UNSIGNED_BYTE, imgPlayer.data());
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glBindTexture(GL_TEXTURE_2D, 0);*/

    GLuint ileBake;
    glGenTextures(1, &ileBake);
    glBindTexture(GL_TEXTURE_2D, ileBake);

    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, img_ile.width(), img_ile.height(), 0, GL_RGBA, GL_UNSIGNED_BYTE, img_ile.data());
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

    // VBO
    ile.setVbo();
    boids3D.setVbo();

    // VAO
    ile.setVao();
    boids3D.setVao();

    // CUBE
    Cube cube(areaSize);
    cube.init(Texture);

    /* MATRIX FOR SHADERS*/
    glm::mat4 ProjMatrix;
    glm::mat4 MVMatrix;
    glm::mat4 NormalMatrix;

    ProjMatrix = glm::perspective(glm::radians(70.f), ctx.aspect_ratio(), 0.1f, 100.f);

    // LIGHTS
    Light lightScene = Light(glm::vec3{50.});
    Light lightPerso = Light(glm::vec3{0.0001});

    glEnable(GL_DEPTH_TEST);

    // Declare your infinite update loop.
    ctx.update = [&]() {
        /*********************************
         * RENDERING
         *********************************/
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        /* *** MOVING PLAYER & BOIDS *** */
        moveListener(ctx, player, camera);
        simulation.simulate(areaSize, true);

        // std::cout << "Camera position: " << camera.getPosition().x << " " << camera.getPosition().y << " " << camera.getPosition().z << std::endl;

        glm::mat4 viewMatrix = camera.getViewMatrix();
        ShaderPoint.use();

        MVMatrix     = glm::translate(glm::mat4(1.0), glm::vec3(0., -5., -5.));
        MVMatrix     = viewMatrix * MVMatrix;
        NormalMatrix = glm::transpose(glm::inverse(MVMatrix));

        lightScene.drawLightScene(glm::vec3(0., 5., -5.), ProjMatrix, viewMatrix, ShaderPoint);
        //*lightPerso.drawLightPlayer(player.getPosition(), ProjMatrix, viewMatrix, ShaderPoint);

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

        ile.draw(player.getPosition(), glm::vec3{1.}, -100, glm::vec3(0.0f, 1.0f, 0.0f), ProjMatrix, viewMatrix, ShaderPoint, ileBake);

        std::cout << "taille des boids" + simulation.getBoids().size() << std::endl;

        for (Boid& b : simulation.getBoids())
        {
            int i = 0;
            i++;
            std::cout << i << std::endl;
            boids3D.draw(b.getPosition(), glm::vec3{1.}, -100, glm::vec3(1.f), ProjMatrix, viewMatrix, ShaderPoint, boidBake);
        }

        shaderCube.use();
        cube.draw(posPlayer, glm::vec3{1}, shaderCube, viewMatrix, ProjMatrix);

        /*glDrawArrays(GL_TRIANGLES, 0, vertices.size());

        glBindVertexArray(0);*/
    };

    // Should be done last. It starts the infinite loop.
    ctx.start();

    // delete 3D model
    ile.~Model();

    /*glDeleteTextures(1, &tex);
    glDeleteBuffers(1, &vbo);
    glDeleteVertexArrays(1, &vao);*/
    cube.~Cube();

    shaderCube.~Program();
    ShaderPoint.~Program();
}