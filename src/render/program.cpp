#include "program.hpp"
#include <p6/p6.h>
#include <string>
#include <utility>
#include <vector>
#include "glm/ext/matrix_transform.hpp"
#include "glm/fwd.hpp"
#include "glm/trigonometric.hpp"

void Program::addUniformVariable(const std::string& name)
{
    GLint uName = glGetUniformLocation(getId(), name.c_str());
    m_uniformVariables.insert(std::pair<std::string, GLint>(name, uName));
}

GLuint Program::getId()
{
    return m_program.id();
}

void Program::use()
{
    m_program.use();
}

void Program::uniformMatrix4fv(std::string name, glm::mat4 value)
{
    glUniformMatrix4fv(m_uniformVariables[name], 1, GL_FALSE, glm::value_ptr(value));
}

void Program::uniform3fv(std::string name, glm::vec3 value)
{
    glUniform3fv(m_uniformVariables[name], 1, glm::value_ptr(value));
}

void Program::uniform1i(std::string name, int value)
{
    glUniform1i(m_uniformVariables[name], value);
}

void Program::uniform1f(std::string name, float value)
{
    glUniform1f(m_uniformVariables[name], value);
}