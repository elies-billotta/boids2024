#pragma once

#include <glm/glm.hpp>
#include "../src-common/glimac/sphere_vertices.hpp"
#include "../src/render/program.hpp"
#include "glm/fwd.hpp"
#include "glm/gtc/type_ptr.hpp"
#include "p6/p6.h"

class Cube {
private:
    std::vector<float>  vertices;
    std::vector<GLuint> indices;
    GLuint              vao, vbo, ibo;
    glm::vec3           m_position;
    float               m_size;
    GLuint              texture;

public:
    Cube(float size, glm::vec3 position);

    void textures(img::Image& img_load);

    void vectors();

    void vertex();

    void init(img::Image& img_load);

    void draw(glm::vec3 pos, glm::vec3 scale, Program& program, glm::mat4 viewMatrix, glm::mat4 ProjMatrix) const;

    /* *** GETTERS *** */
    float     getSize();
    glm::vec3 getPosition();
    glm::vec3 getCubePosition();
};