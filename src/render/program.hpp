#pragma once

#include <filesystem>
#include <glm/glm.hpp>
#include <map>
#include "../src-common/glimac/sphere_vertices.hpp"
#include "glm/fwd.hpp"
#include "glm/gtc/type_ptr.hpp"
#include "p6/p6.h"

class Program {
private:
    std::filesystem::path        m_vsPath;
    std::filesystem::path        m_fsPath;
    p6::Shader                   m_program;
    std::map<std::string, GLint> m_uniformVariables;

public:
    ///\brief default constructor
    Program();

    /// \brief Constructs a Program object with the specified vertex and fragment shader paths
    /// \param vsPath The path to the vertex shader file
    /// \param fsPath The path to the fragment shader file
    Program(std::filesystem::path vsPath, std::filesystem::path fsPath)
        : m_vsPath(vsPath), m_fsPath(fsPath), m_program(p6::load_shader(vsPath, fsPath)){};
    // ~Program();

    /// \brief Adds a uniform variable with the specified name to the program
    /// \param name The name of the uniform variable
    void addUniformVariable(const std::string& name);

    /// \brief Returns the ID of the OpenGL program
    /// \return The ID of the OpenGL program
    GLuint getId();

    /// \brief Activates the shader program for rendering
    void use();

    /// \brief Sets the value of a 4x4 matrix uniform variable in the shader program
    /// \param name The name of the uniform variable
    /// \param value The value to set for the uniform variable
    void uniformMatrix4fv(std::string name, glm::mat4 value);

    /// \brief Sets the value of a 3D vector uniform variable in the shader program
    /// \param name The name of the uniform variable
    /// \param value The value to set for the uniform variable
    void uniform3fv(std::string name, glm::vec3 value);

    /// \brief Sets the value of an integer uniform variable in the shader program (texture)
    /// \param name The name of the uniform variable
    /// \param value The value to set for the uniform variable
    void uniform1i(std::string name, int value);

    /// \brief Sets the value of a floating-point uniform variable in the shader program
    /// \param name The name of the uniform variable
    /// \param value The value to set for the uniform variable
    void uniform1f(std::string name, float value);
};
