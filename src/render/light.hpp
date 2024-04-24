#pragma once
#include <glm/gtc/random.hpp>
#include "../src/render/program.hpp"
#include "glm/fwd.hpp"

class Light {
public:
    Light(const glm::vec3& intensity);

    void setDirection(const glm::vec3& direction);
    void setIntensity(const glm::vec3& intensity);
    void passToShader(Program& shader, glm::vec3 intensity, glm::mat4 projMatrix, glm::mat4 viewMatrix, glm::vec3 playerPosition);

private:
    glm::vec3 m_direction;
    glm::vec3 m_intensity;
};