#pragma once

#include <glm/glm.hpp>
// #include "../src/game/include/player.hpp"
#include "../src-common/glimac/sphere_vertices.hpp"
#include "../src/render/program.hpp"
#include "glm/fwd.hpp"
#include "glm/gtc/type_ptr.hpp"
// #include "model.hpp"
#include "p6/p6.h"

class Cube {
private:
    std::vector<float>  vertices;
    std::vector<GLuint> indices;
    GLuint              vao, vbo, ibo;
    float               m_size;
    GLuint              texture;

public:
    /// \brief param constructor of the cube around the scene
    /// \param size the size of the cube
    explicit Cube(float size);

    ///\brief cube destructor
    ~Cube() = default;

    /// \brief creation of the texture for the cube
    /// \param img_load image use for the texture
    void textures(img::Image& img_load);

    /// \brief creation of a tab vertex of the cube with his coord
    void vectors();

    /// \brief configuration of vertex attribut for the rendering au cube
    void vertex();

    /// \brief verify the position of the player to be sure he is in the cube
    /// \param player the player
    // void borders(Player& player);

    /// \brief to initialise the cube
    /// \param img_load the image use for the texture of the cube
    void init(img::Image& img_load);

    /// \brief Draws the cube at the specified position and scale using the provided program and matrices
    ///\param pos The position of the cube.
    ///\param scale The scale of the cube.
    ///\param program The shader program to use for rendering.
    ///\param viewMatrix The view matrix for the scene.
    ///\param ProjMatrix The projection matrix for the scene.
    void draw(glm::vec3 pos, glm::vec3 scale, Program& program, glm::mat4 viewMatrix, glm::mat4 ProjMatrix) const;
};