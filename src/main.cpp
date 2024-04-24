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

const int       N     = 1;
const float     speed = 0.01f;
const glm::vec3 posPlayer(0., 0., -10.);

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
    auto areaSize = 0.8f;
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
    Simulation simulation = Simulation(N, areaSize, 0.03f);
    ImVec4     namedColor = ImVec4(0.4f, 0.7f, 0.0f, 1.0f);
    bool       check      = false;

    ctx.imgui = [&]() {
        ImGui::Begin("Test");
        ImGui::SliderFloat("Square size", &areaSize, 0.f, 1.f);
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
    img::Image imgRock   = p6::load_image_buffer("../assets/texture/rock.png");

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
    Model rock    = Model();

    // LOAD 3D MODEL
    ile.loadModel("player.obj");
    boids3D.loadModel("player.obj");
    rock.loadModel("rock.obj");

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

    GLuint rockBake;
    glGenTextures(1, &rockBake);
    glBindTexture(GL_TEXTURE_2D, rockBake);

    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, imgRock.width(), imgRock.height(), 0, GL_RGBA, GL_UNSIGNED_BYTE, imgRock.data());
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glBindTexture(GL_TEXTURE_2D, 0);

    // VBO
    ile.setVbo();
    boids3D.setVbo();
    rock.setVbo();

    // VAO
    ile.setVao();
    boids3D.setVao();
    rock.setVao();

    // CUBE
    Cube cube(5.0f);
    cube.init(Texture);

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

        /* *** MOVING PLAYER *** */
        moveListener(ctx, player, camera);

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
        // Dessiner chaque rocher à sa position respective
        for (const auto& position : rockPositions)
        {
            rock.draw(position, glm::vec3{rockSize}, 0, glm::vec3(0.0f, 1.0f, 0.0f), ProjMatrix, viewMatrix, ShaderPoint, rockBake);
        }

        for (Boid& b : simulation.getBoids())
        {
            int i = 0;
            i++;
            // std::cout << i << std::endl;
            boids3D.draw(b.getPosition(), glm::vec3{1.}, -100, glm::vec3(1.f), ProjMatrix, viewMatrix, ShaderPoint, boidBake);
        }
        shaderCube.use();
        cube.draw(glm::vec3(0., 0., 0.), glm::vec3{5.}, shaderCube, viewMatrix, ProjMatrix);

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