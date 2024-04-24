#include "../src/player/player.hpp"

/*float Player::getRotationAngle() const
{
    return m_RotationAngle; // Remplacez m_RotationAngle par la variable ou la valeur appropriée contenant l'angle de rotation de votre modèle
}

void Player::setRotationAngle(float rotationAngle)
{
    m_RotationAngle = rotationAngle; // Remplacez m_RotationAngle par la variable ou la valeur appropriée contenant l'angle de rotation de votre modèle
}
*/

Player::Player(glm::vec3 pos)
    : m_pos(pos), m_Phi(p6::PI), m_Theta(0.)
{
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

    if (verifBorder(savePos, cube))
        m_pos = savePos;

    camera.setPosition(savePos);
};

/* *** GETTERS *** */
glm::vec3 Player::getPosition()
{
    return m_pos;
}

/*Player::Player(glm::vec3 position, float phi, float theta)
    : m_Position(position), m_Phi(phi), m_Theta(theta)
{
    this->computeDirectionVectors();
};

void Player::computeDirectionVectors()
{
    m_FrontVector = glm::vec3(glm::cos(m_Theta) * glm::sin(m_Phi), glm::sin(m_Theta), glm::cos(m_Theta) * glm::cos(m_Phi));
    m_LeftVector  = glm::vec3(glm::sin(m_Phi + (p6::PI / 2)), 0, glm::cos(m_Phi + (p6::PI / 2)));
    m_UpVector    = glm::cross(m_FrontVector, m_LeftVector);
}

void Player::moveLeft(float t)
{
    m_Position += t * m_LeftVector;
    computeDirectionVectors();
}
void Player::moveFront(float t)
{
    m_Position += t * m_FrontVector;
    computeDirectionVectors();
}

void Player::rotateLeft(float degrees)
{
    m_Phi += glm::radians(degrees);
    computeDirectionVectors();
}
void Player::rotateUp(float degrees)
{
    m_Theta += glm::radians(degrees);
    computeDirectionVectors();
}

glm::mat4 Player::getViewMatrix() const
{
    return glm::lookAt(m_Position, m_Position + m_FrontVector, m_UpVector);
}

void cameraOption(Player& player, bool& left, bool& right, bool& up, bool& down, p6::Context& ctx)
{
    if (right)
    {
        player.rotateLeft(-0.5f);
    }
    if (left)
    {
        player.rotateLeft(+0.5f);
    }
    if (up)
    {
        player.moveFront(0.05f);
    }
    if (down)
    {
        player.moveFront(-0.05f);
    }

    ctx.key_pressed = [&right, &up, &left, &down](p6::Key key) {
        if (key.logical == "d")
        {
            right = true;
        }
        if (key.logical == "q")
        {
            left = true;
        }
        if (key.logical == "z")
        {
            up = true;
        }
        if (key.logical == "s")
        {
            down = true;
        }
    };

    ctx.key_released = [&right, &up, &left, &down](p6::Key key) {
        if (key.logical == "d")
        {
            right = false;
        }
        if (key.logical == "q")
        {
            left = false;
        }
        if (key.logical == "z")
        {
            up = false;
        }
        if (key.logical == "s")
        {
            down = false;
        }
    };

    ctx.mouse_dragged = [&player](const p6::MouseDrag& button) {
        player.rotateLeft(button.delta.x * 5);
        player.rotateUp(-button.delta.y * 5);
    };

    ctx.mouse_scrolled = [&](p6::MouseScroll scroll) {
        player.moveFront(-scroll.dy);
    };
}*/

/*void Player::drawPlayer(Model& model, glm::mat4 ViewMatrix, glm::mat4 ProjMatrix, Program& program, GLuint textName)
{
    // glm::mat4 ViewMatrix = camera->getViewMatrix();
    ViewMatrix = glm::translate(ViewMatrix, m_pos);
    // ViewMatrix = glm::rotate(ViewMatrix, 45.f, glm::vec3(1.,0.,0.));
    ViewMatrix = glm::translate(ViewMatrix, glm::vec3(0, -0.7, 0.0));

    ViewMatrix             = glm::scale(ViewMatrix, glm::vec3(0.2, 0.2, 0.2));
    glm::mat4 NormalMatrix = glm::transpose(glm::inverse(ViewMatrix));

    program.uniform1i("uText", 0);
    program.uniformMatrix4fv("uMVPMatrix", ProjMatrix * ViewMatrix);
    program.uniformMatrix4fv("uMVMatrix", ViewMatrix);
    program.uniformMatrix4fv("uNormalMatrix", NormalMatrix);

    model.drawArray(textName);
}*/
