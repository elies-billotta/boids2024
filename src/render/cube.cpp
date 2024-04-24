#include "../src/render/cube.hpp"

Cube::Cube(float size, glm::vec3 position)
    : m_size(size), texture(0), m_position(position)
{
}

void Cube::textures(img::Image& img_load)
{
    glGenTextures(1, &texture);
    glBindTexture(GL_TEXTURE_2D, texture);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, img_load.width(), img_load.height(), 0, GL_RGBA, GL_UNSIGNED_BYTE, img_load.data());

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    glBindTexture(GL_TEXTURE_2D, 0);

    glGenBuffers(1, &vbo);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
}

void Cube::vectors()
{
    vertices = {
        // Face avant
        -m_size, -m_size, -m_size, // 0
        m_size, -m_size, -m_size,  // 1
        m_size, m_size, -m_size,   // 2
        -m_size, m_size, -m_size,  // 3

        // Face arrière
        m_size, -m_size, m_size,  // 4
        -m_size, -m_size, m_size, // 5
        -m_size, m_size, m_size,  // 6
        m_size, m_size, m_size,   // 7

        // Face gauche
        -m_size, -m_size, m_size,  // 8
        -m_size, -m_size, -m_size, // 9
        -m_size, m_size, -m_size,  // 10
        -m_size, m_size, m_size,   // 11

        // Face droite
        m_size, -m_size, -m_size, // 12
        m_size, -m_size, m_size,  // 13
        m_size, m_size, m_size,   // 14
        m_size, m_size, -m_size,  // 15

        // Face supérieure
        -m_size, m_size, -m_size, // 16
        m_size, m_size, -m_size,  // 17
        m_size, m_size, m_size,   // 18
        -m_size, m_size, m_size,  // 19

        // Face inférieure
        -m_size, -m_size, -m_size, // 20
        m_size, -m_size, -m_size,  // 21
        m_size, -m_size, m_size,   // 22
        -m_size, -m_size, m_size   // 23
    };

    std::vector<float> textureCoords = {
        // Face avant
        0.0f, 1.0f,
        1.0f, 1.0f,
        1.0f, 0.0f,
        0.0f, 0.0f,

        // Face arrière
        1.0f, 1.0f,
        0.0f, 1.0f,
        0.0f, 0.0f,
        1.0f, 0.0f,

        // Face gauche
        1.0f, 1.0f,
        0.0f, 1.0f,
        0.0f, 0.0f,
        1.0f, 0.0f,

        // Face droite
        0.0f, 1.0f,
        1.0f, 1.0f,
        1.0f, 0.0f,
        0.0f, 0.0f,

        // Face supérieure
        0.0f, 1.0f,
        1.0f, 1.0f,
        1.0f, 0.0f,
        0.0f, 0.0f,

        // Face inférieure
        0.0f, 1.0f,
        1.0f, 1.0f,
        1.0f, 0.0f,
        0.0f, 0.0f
    };

    glBufferData(GL_ARRAY_BUFFER, (vertices.size() + textureCoords.size()) * sizeof(float), nullptr, GL_STATIC_DRAW);
    glBufferSubData(GL_ARRAY_BUFFER, 0, vertices.size() * sizeof(float), vertices.data());
    glBufferSubData(GL_ARRAY_BUFFER, vertices.size() * sizeof(float), textureCoords.size() * sizeof(float), textureCoords.data());

    glBindBuffer(GL_ARRAY_BUFFER, 0);

    glGenBuffers(1, &ibo);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo);

    // Définir les indices des triangles
    indices = {
        // Face avant
        0, 1, 2,
        2, 3, 0,

        // Face arrière
        4, 5, 6,
        6, 7, 4,

        // Face gauche
        8, 9, 10,
        10, 11, 8,

        // Face droite
        12, 13, 14,
        14, 15, 12,

        // Face supérieure
        16, 17, 18,
        18, 19, 16,

        // Face inférieure
        20, 21, 22,
        22, 23, 20
    };

    glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned int), &indices[0], GL_STATIC_DRAW);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

    glGenVertexArrays(1, &vao);
    glBindVertexArray(vao);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo);
}

void Cube::vertex()
{
    const GLuint VERTEX_ATTR_POSITION = 0;
    const GLuint VERTEX_ATTR_NORMAL   = 1;
    const GLuint VERTEX_ATTR_UV       = 2;

    glBindBuffer(GL_ARRAY_BUFFER, vbo);

    glEnableVertexAttribArray(VERTEX_ATTR_POSITION);
    glEnableVertexAttribArray(VERTEX_ATTR_UV);
    glEnableVertexAttribArray(VERTEX_ATTR_NORMAL);

    glVertexAttribPointer(VERTEX_ATTR_POSITION, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glVertexAttribPointer(VERTEX_ATTR_UV, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), (void*)(vertices.size() * sizeof(float)));
    glVertexAttribPointer(VERTEX_ATTR_NORMAL, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)(2 * vertices.size() * sizeof(float)));

    glEnableVertexAttribArray(0);

    glBindBuffer(GL_ARRAY_BUFFER, 0);

    glBindVertexArray(0);
}

void Cube::init(img::Image& img_load)
{
    textures(img_load);

    vectors();

    vertex();
}

void Cube::draw(glm::vec3 pos, glm::vec3 scale, Program& program, glm::mat4 viewMatrix, glm::mat4 ProjMatrix) const
{
    glm::mat4 ViewMatrixModel = glm::translate(glm::mat4(1.0), pos);
    ViewMatrixModel           = glm::scale(ViewMatrixModel, scale);

    glm::mat4 NormalMatrix = glm::transpose(glm::inverse(ViewMatrixModel));

    ViewMatrixModel = viewMatrix * ViewMatrixModel;

    program.uniformMatrix4fv("uMVPMatrix", ProjMatrix * ViewMatrixModel);
    program.uniformMatrix4fv("uMVMatrix", ViewMatrixModel);
    program.uniformMatrix4fv("uNormalMatrix", NormalMatrix);
    glBindVertexArray(vao);
    glBindTexture(GL_TEXTURE_2D, texture);
    glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, 0);
}

/* *** GETTERS *** */
float Cube::getSize()
{
    return m_size;
}

glm::vec3 Cube::getPosition()
{
    return m_position;
}

glm::vec3 Cube::getCubePosition()
{
    return m_position + glm::vec3(m_size, m_size, m_size);
}
