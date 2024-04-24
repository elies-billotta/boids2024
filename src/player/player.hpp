#pragma once

#include <p6/p6.h>
#include <glm/glm.hpp>
#include <vector>
#include "../src-common/glimac/sphere_vertices.hpp"
#include "../src/camera/camera.hpp"
#include "../src/render/cube.hpp"
#include "../src/render/model.hpp"
#include "../src/render/program.hpp"
#include "glm/ext/matrix_transform.hpp"
#include "glm/fwd.hpp"
#include "glm/gtc/type_ptr.hpp"
#include "glm/trigonometric.hpp"
#include "p6/p6.h"

class Player {
public:
    Player(glm::vec3 pos);
    void move(const p6::Context& ctx, Player& player, Camera& camera, Cube& cube);

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
