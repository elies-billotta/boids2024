#pragma once

#include <p6/p6.h>
#include <glm/glm.hpp>
#include "../src/camera/camera.hpp"
#include "../src/render/cube.hpp"
#include "glm/fwd.hpp"

class Player {
public:
    explicit Player(glm::vec3 pos);
    void move(const p6::Context& ctx, Camera& camera, Cube& cube);

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

    void computeDirectionVectors();
    void moveFront(float speed);
    void moveLeft(float speed);
};
