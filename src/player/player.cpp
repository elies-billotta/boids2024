#include <cmath>

#include "../src/player/player.hpp"

Player::Player(glm::vec3 pos)
    : m_pos(pos), m_Phi(p6::PI), m_Theta(0.)
{
    this->computeDirectionVectors();
};

void Player::computeDirectionVectors()
{
    m_FrontVector = glm::vec3(std::cos(m_Phi) * std::sin(m_Theta), std::sin(m_Phi), std::cos(m_Phi) * std::cos(m_Theta));

    m_LeftVector = glm::vec3(std::sin(m_Theta - glm::half_pi<float>()), 0.0f, std::cos(m_Theta - glm::half_pi<float>()));

    m_UpVector = glm::cross(m_FrontVector, m_LeftVector);
}

bool verifBorder(glm::vec3 position, Cube& cube)
{
    for (int i = 0; i < 3; i++)
    {
        if (position[i] > cube.getCubePosition()[i])
        {
            return false;
        }
        if (position[i] < -cube.getCubePosition()[i])
        {
            return false;
        }
    }

    return true;
}

void Player::move(const p6::Context& ctx, Camera& camera, Cube& cube)
{
    glm::vec3 savePos = m_pos;

    if (ctx.key_is_pressed(GLFW_KEY_W) || ctx.key_is_pressed(GLFW_KEY_UP))
    {
        savePos += m_speed * m_FrontVector;
    }
    if (ctx.key_is_pressed(GLFW_KEY_S) || ctx.key_is_pressed(GLFW_KEY_DOWN))
    {
        savePos += -m_speed * m_FrontVector;
    }

    if (ctx.key_is_pressed(GLFW_KEY_A) || ctx.key_is_pressed(GLFW_KEY_LEFT))
    {
        savePos += m_speed * m_LeftVector;
    }
    if (ctx.key_is_pressed(GLFW_KEY_D) || ctx.key_is_pressed(GLFW_KEY_RIGHT))
    {
        savePos += -m_speed * m_LeftVector;
    }

    if (ctx.key_is_pressed(GLFW_KEY_C))
    {
        savePos += m_speed * m_UpVector;
    }
    if (ctx.key_is_pressed(GLFW_KEY_SPACE))
    {
        savePos += -m_speed * m_UpVector;
    }

    if (verifBorder(savePos, cube))
        m_pos = savePos;

    camera.setPosition(savePos);
};

/* *** GETTERS *** */
glm::vec3 Player::getPosition()
{
    return m_pos;
}
