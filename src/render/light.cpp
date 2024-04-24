#include "light.hpp"

Light::Light(const glm::vec3& intensity)
    : m_intensity(intensity)
{
}

void Light::setDirection(const glm::vec3& direction)
{
    m_direction = glm::normalize(direction);
}

void Light::setIntensity(const glm::vec3& intensity)
{
    m_intensity = intensity;
}

void Light::passToShader(Program& shader, glm::mat4 projMatrix, glm::mat4 viewMatrix, glm::vec3 playerPosition)
{
    glm::vec4 LightPos = viewMatrix * glm::vec4(playerPosition, 1.0);

    shader.uniform3fv("uKd", glm::vec3(243.0f / 255.0f, 179.0f / 255.0f, 180.0f / 255.0f));
    shader.uniform3fv("uKs", glm::vec3(1.0f, 0.95f, 1.f));
    shader.uniform1f("uShininess", 0.6);

    shader.uniform3fv("uLightPos_vs", playerPosition);
    shader.uniform3fv("uLightIntensity", this->m_intensity);

    glm::mat4 NormalMatrix = glm::transpose(glm::inverse(viewMatrix));

    shader.uniformMatrix4fv("uMVPMatrix", projMatrix * viewMatrix);
    shader.uniformMatrix4fv("uMVMatrix", viewMatrix);
    shader.uniformMatrix4fv("uNormalMatrix", NormalMatrix);
}