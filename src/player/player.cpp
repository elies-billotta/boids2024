#include "../src/player/player.hpp"

Player::Player(glm::vec3 pos, glm::vec2 mouse)
    : m_pos(pos), m_Phi(p6::PI), m_Theta(0.)
{
    m_oldMouse = mouse;
    this->computeDirectionVectors();
};

void Player::computeDirectionVectors()
{
    m_FrontVector = glm::vec3(cos(m_Phi) * sin(m_Theta), sin(m_Phi), cos(m_Phi) * cos(m_Theta));

    m_LeftVector = glm::vec3(sin(m_Theta - glm::half_pi<float>()), 0.0f, cos(m_Theta - glm::half_pi<float>()));

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

void Player::move(const p6::Context& ctx, Player& player, Camera& camera, Cube& cube)
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

    if (ctx.mouse() != m_oldMouse)
    {
        float mouseDeltaX = ctx.mouse().x - m_oldMouse.x;

        m_Theta += mouseDeltaX * m_sensitivity;
        m_Phi -= 0;

        m_rotMatrix = glm::rotate(glm::mat4(1.0f), m_Theta, glm::vec3(0.0f, -1.0f, 0.0f)) * glm::rotate(glm::mat4(1.0f), m_Phi, glm::vec3(1.0f, 0.0f, 0.0f));

        // if we want to limit the angle of the camera
        // m_Phi = glm::clamp(m_Phi, -glm::half_pi<float>(), glm::half_pi<float>());

        computeDirectionVectors();
        m_oldMouse = ctx.mouse();
    }

    camera.setPosition(savePos, m_rotMatrix, m_Phi, m_Theta);
};

void Player::draw(Model& model, glm::vec3 scale, glm::mat4 projMatrix, glm::mat4 viewMatrix, Program& program, GLuint textName)
{
    glm::mat4 viewMatrixModel = glm::translate(glm::mat4(1.0), m_pos);
    viewMatrixModel           = glm::rotate(viewMatrixModel, glm::radians(-80.f), glm::vec3{0.0f, 1.0f, 0.0f});
    viewMatrixModel           = glm::rotate(viewMatrixModel, glm::radians(-180.f), glm::vec3{1.0f, 0.0f, 0.0f});
    viewMatrixModel           = viewMatrixModel * m_rotMatrix;
    viewMatrixModel           = glm::scale(viewMatrixModel, scale);
    viewMatrixModel           = viewMatrix * viewMatrixModel;

    glm::mat4 NormalMatrix = glm::transpose(glm::inverse(viewMatrixModel));
    program.uniform1i("uText", 0);
    program.uniformMatrix4fv("uMVPMatrix", projMatrix * viewMatrixModel);
    program.uniformMatrix4fv("uMVMatrix", viewMatrixModel);
    program.uniformMatrix4fv("uNormalMatrix", NormalMatrix);

    model.drawArray(textName);
}

/* *** GETTERS *** */
glm::vec3 Player::getPosition()
{
    return m_pos;
}
