#pragma once

#include <p6/p6.h>
#include <filesystem>
#include <glm/glm.hpp>
#include <map>
#include <string>
#include <utility>
#include <vector>
#include "../src-common/glimac/sphere_vertices.hpp"
#include "glm/ext/matrix_transform.hpp"
#include "glm/fwd.hpp"
#include "glm/gtc/type_ptr.hpp"
#include "glm/trigonometric.hpp"
#include "p6/p6.h"

class Program {
private:
    std::filesystem::path        m_vsPath;
    std::filesystem::path        m_fsPath;
    p6::Shader                   m_program;
    std::map<std::string, GLint> m_uniformVariables;

public:
    Program();

    Program(std::filesystem::path vsPath, std::filesystem::path fsPath)
        : m_vsPath(vsPath), m_fsPath(fsPath), m_program(p6::load_shader(vsPath, fsPath)){};

    void addUniformVariable(const std::string& name);

    GLuint getId();

    void use();

    void uniformMatrix4fv(std::string name, glm::mat4 value);

    void uniform3fv(std::string name, glm::vec3 value);

    void uniform1i(std::string name, int value);

    void uniform1f(std::string name, float value);
};
