#include "camera.hpp"
#include <iostream> // Add this line

Camera::Camera(glm::vec3 posPlayer)
    : m_Phi(glm::pi<float>()), m_Theta(0.0f)
{
    m_position = posPlayer + glm::vec3(0.0f, 0.0f, 9.0f);
    // computeDirectionVectors();
}

glm::mat4 Camera::getViewMatrix() const
{
    glm::vec3 target = m_position + m_FrontVector;
    return glm::lookAt(m_position, target, m_UpVector);
}

/* *** GETTERS *** */
glm::vec3 Camera::getPosition()
{
    return m_position;
}

/* *** SETTERS *** */
glm::vec3 Camera::setPosition(glm::vec3 posPlayer, glm::mat4 playerRotation, float phi, float theta)
{
    m_Theta = theta;
    // m_Phi   = phi;

    m_position = posPlayer + glm::vec3(0.0f, 0.0f, 7.0f);

    // Try to make it rotate but not working
    /*glm::vec4 rotatedCameraPosition = playerRotation * glm::vec4(m_position, 1.0f);
    m_position                      = glm::vec3(rotatedCameraPosition);*/

    computeDirectionVectors();
    return m_position;
}