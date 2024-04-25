#pragma once
#include <glm/gtc/random.hpp>
#include "../src/render/program.hpp"
#include "glm/fwd.hpp"

class Light {
public:
    Light(const glm::vec3& intensity);

    void      setDirection(const glm::vec3& direction);
    void      setIntensity(const glm::vec3& intensity);
    glm::vec3 getIntensity();
    void      drawLightPlayer(glm::vec3 pos, glm::mat4 projMatrix, glm::mat4 viewMatrix, Program& program);
    void      passToShader(Program& shader, glm::vec3 intensity, glm::mat4 projMatrix, glm::mat4 viewMatrix, glm::vec3 playerPosition);
    void      passToShader2(Program& shader, glm::vec3 intensity, glm::mat4 projMatrix, glm::mat4 viewMatrix, glm::vec3 playerPosition);

private:
    glm::vec3 m_direction;
    glm::vec3 m_intensity;
};