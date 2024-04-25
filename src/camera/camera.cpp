#include "camera.hpp"
#include "glm/ext/matrix_transform.hpp"

Camera::Camera(glm::vec3 posPlayer)
    : m_position(posPlayer + glm::vec3(0.0f, 0.0f, 9.0f)), m_Phi(glm::pi<float>()), m_Theta(0.0f), m_FrontVector(glm::vec3(0.0f, 0.0f, -1.0f)), m_LeftVector(glm::vec3(-1.0f, 0.0f, 0.0f)), m_UpVector(glm::vec3(0.0f, 1.0f, 0.0f))
{
    computeDirectionVectors();
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
glm::vec3 Camera::setPosition(glm::vec3 posPlayer)
{
    m_position = posPlayer + glm::vec3(0.0f, 0.0f, 9.0f);
    return m_position;
}