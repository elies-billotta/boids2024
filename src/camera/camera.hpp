#pragma once

#include "glm/fwd.hpp"
#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "p6/p6.h"

class Camera {
public:
    explicit Camera(glm::vec3 posPlayer);

    glm::mat4 getViewMatrix() const;

    /* *** GETTERS *** */
    glm::vec3 getPosition();

    /* *** SETTERS *** */
    glm::vec3 setPosition(glm::vec3 posPlayer, glm::mat4 playerRotation, float phi, float theta);

private:
    glm::vec3 m_position;
    float     m_Phi;
    float     m_Theta;
    glm::vec3 m_FrontVector;
    glm::vec3 m_LeftVector;
    glm::vec3 m_UpVector;

    void computeDirectionVectors()
    {
        m_FrontVector = glm::vec3(cos(m_Phi) * sin(m_Theta), sin(m_Phi), cos(m_Phi) * cos(m_Theta));

        m_LeftVector = glm::vec3(sin(m_Theta - glm::half_pi<float>()), 0.0f, cos(m_Theta - glm::half_pi<float>()));

        m_UpVector = glm::cross(m_FrontVector, m_LeftVector);
    }
};