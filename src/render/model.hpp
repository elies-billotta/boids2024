#pragma once

#include "../src-common/glimac/common.hpp"
#include "../src/render/program.hpp"
#include "glm/ext/matrix_transform.hpp"
#include "glm/fwd.hpp"
#include "glm/matrix.hpp"
#include "p6/p6.h"

class Model {
protected:
    GLuint                           m_vbo;
    GLuint                           m_vao;
    std::vector<glimac::ShapeVertex> m_vertices;
    std::vector<int>                 m_index;
    GLsizei                          m_vertexCount;
    float                            m_Rotation;

public:
    Model() = default;

    /// \brief cdelete model
    ~Model()
    {
        glDeleteBuffers(1, &m_vbo);
        glDeleteVertexArrays(1, &m_vao);
    };

    /// \brief draw the model
    void drawArray(GLuint textName);

    /// \brief define the rotation of the model
    void setRotation(float angle);

    void draw(glm::vec3 pos, glm::vec3 scale, float angle, glm::vec3 RotVec, glm::mat4 ProjMatrix, glm::mat4 ViewMatrix, Program& program, GLuint textName);

    /// \brief set and bind a vao
    void setVao();

    /// \brief set and bind vbo
    void setVbo();

    /// \brief set buffers
    void setBuffers();

    /// \brief get data
    std::vector<glimac::ShapeVertex> getData() const;

    /// \brief get data of the vertices
    const glimac::ShapeVertex* getDataPointer() const;

    /// \brief get the number of the vertices
    GLsizei getVertexCount() const
    {
        return m_vertexCount;
    }

    const int* getIndexPointer() const;

    void loadModel(const std::string& fileName);
};