#pragma once

#include <p6/p6.h>
#include <glm/glm.hpp>
#include "../src/camera/camera.hpp"
#include "../src/render/cube.hpp"
#include "glm/fwd.hpp"

class Player {
public:
    explicit Player(glm::vec3 pos, glm::vec2 mouse);
    void move(const p6::Context& ctx, Camera& camera, Cube& cube);
    void draw(Model& model, glm::vec3 scale, glm::mat4 projMatrix, glm::mat4 viewMatrix, Program& program, GLuint textName);

    /* *** GETTERS *** */
    glm::vec3 getPosition();

private:
    glm::vec3 m_pos;
    float     m_Phi;
    float     m_Theta;
    float     m_speed = 0.1f;

    glm::vec3 m_FrontVector;
    glm::vec3 m_LeftVector;
    glm::vec3 m_UpVector;
    float     m_sensitivity = 0.5f;
    glm::vec2 m_oldMouse;

    glm::mat4 m_rotMatrix;

    void computeDirectionVectors();
    void moveFront(float speed);
    void moveLeft(float speed);
};
