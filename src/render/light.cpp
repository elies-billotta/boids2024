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

glm::vec3 Light::getIntensity()
{
    return m_intensity;
}

void Light::passToShader2(Program& shader, glm::vec3 intensity, glm::mat4 projMatrix, glm::mat4 viewMatrix, glm::vec3 playerPosition)
{
    intensity = intensity * this->m_intensity;

    shader.uniform3fv("uLightPos2_vs", playerPosition);
    shader.uniform3fv("uLightIntensity2", intensity);
}

void Light::passToShader(Program& shader, glm::vec3 intensity, glm::mat4 projMatrix, glm::mat4 viewMatrix, glm::vec3 playerPosition)
{
    intensity = intensity * this->m_intensity;
    shader.uniform3fv("uKd", glm::vec3(243.0f / 255.0f, 179.0f / 255.0f, 180.0f / 255.0f));
    shader.uniform3fv("uKs", glm::vec3(1.0f, 0.95f, 1.f));
    shader.uniform1f("uShininess", 0.6);

    shader.uniform3fv("uLightPos_vs", playerPosition);
    shader.uniform3fv("uLightIntensity", intensity);
}