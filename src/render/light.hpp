#pragma once

#include <glm/glm.hpp>
#include "../src/render/program.hpp"
#include "glm/fwd.hpp"
#include "p6/p6.h"

class Light {
private:
    glm::vec3 m_intensity;

public:
    /// \brief param constructor of the light
    Light(glm::vec3 intensity)
        : m_intensity(intensity){};

    /// \brief light destructor
    ~Light() = default;

    /// \brief draw the scene light
    /// \param pos position of the light
    /// \param projMatrix matrice of projection for the shader
    /// \param viewMatrix viewMatrix (camera) for the shader
    /// \param program shader will be used for drawing the model
    void drawLightScene(glm::vec3 pos, glm::mat4 projMatrix, glm::mat4 viewMatrix, Program& program);

    /// \brief draw the player light
    /// \param pos position of the light
    /// \param projMatrix matrice of projection for the shader
    /// \param viewMatrix viewMatrix (camera) for the shader
    /// \param program shader will be used for drawing the model
    void drawLightPlayer(glm::vec3 pos, glm::mat4 projMatrix, glm::mat4 viewMatrix, Program& program);
};