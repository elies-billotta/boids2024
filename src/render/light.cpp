#include "../src/render/light.hpp"
#include "glm/fwd.hpp"

void Light::drawLightScene(glm::vec3 pos, glm::mat4 projMatrix, glm::mat4 viewMatrix, Program& program)
{
    // glm::mat4 ViewMatrixLight = viewMatrix;
    program.use();
    program.uniform3fv("uKd", glm::vec3(1.0f, 0.95f, 0.8f));
    program.uniform3fv("uKs", glm::vec3(1.0f, 0.95f, 0.8f));
    program.uniform1f("uShininess", 0.6);

    glm::vec4 LightPos = viewMatrix * glm::vec4(pos, 1.0);
    program.uniform3fv("uLightPos_vs", glm::vec3(LightPos.x, LightPos.y, LightPos.z));

    program.uniform3fv("uLightIntensity", this->m_intensity);

    glm::mat4 NormalMatrix = glm::transpose(glm::inverse(viewMatrix));

    program.uniformMatrix4fv("uMVPMatrix", projMatrix * viewMatrix);
    program.uniformMatrix4fv("uMVMatrix", viewMatrix);
    program.uniformMatrix4fv("uNormalMatrix", NormalMatrix);
}

void Light::drawLightPlayer(glm::vec3 pos, glm::mat4 projMatrix, glm::mat4 viewMatrix, Program& program)
{
    glm::vec4 LightPos = viewMatrix * glm::vec4(pos, 1.0);
    program.uniform3fv("uLightPos2_vs", glm::vec3(LightPos.x, LightPos.y, LightPos.z));

    program.uniform3fv("uLightIntensity2", this->m_intensity);

    glm::mat4 NormalMatrix = glm::transpose(glm::inverse(viewMatrix));

    program.uniformMatrix4fv("uMVPMatrix", projMatrix * viewMatrix);
    program.uniformMatrix4fv("uMVMatrix", viewMatrix);
    program.uniformMatrix4fv("uNormalMatrix", NormalMatrix);
}