#pragma once

#include <glm/glm.hpp>
#include "../src-common/glimac/sphere_vertices.hpp"
#include "../src/render/model.hpp"
#include "../src/render/program.hpp"
#include "glm/fwd.hpp"
#include "glm/gtc/type_ptr.hpp"
#include "p6/p6.h"

class Player {
public:
    ///\brief Computes the direction vectors (front, left, up) based on the current angles
    ///\brief defaut constructor
    // Player();

    ///\brief param constructor
    ///\param position The initial position of the player
    /// \param phi The initial phi angle (rotation around the y-axis) of the player's orientation
    /// \param theta The initial theta angle (rotation around the x-axis) of the player's orientation
    Player(glm::vec3 pos);

    void moveFront(float speed);

    void moveLeft(float speed);

    ///\brief destructor of the player
    /*~Player() = default;

    ///\brief setter of the position of the player
    ///\param posPlayer the player's position
    void setPosition(glm::vec3 posPlayer);

    ///\brief getter of the position of the player
    glm::vec3 getPosition();

    ///\brief setter of the rotation's angle of the player
    ///\param rotationAngle the player's rotation angle
    void setRotationAngle(float rotationAngle);

    ///\brief getter of the rotation's angle of the player
    float getRotationAngle() const;

    ///\brief move the player (the camera) belong the vector L
    ///\param t value of displacement
    void moveLeft(float t);

    ///\brief move the player (the camera) belong the vector F
    ///\param t value of displacement
    void moveFront(float t);

    ///\brief rotation the player (the camera) belon the horizontal axe
    ///\param degrees value of the rotation
    void rotateLeft(float degrees);

    ///\brief rotation the player (the camera) belon the vertical axe
    ///\param degrees value of the rotation
    void rotateUp(float degrees);

    ///\brief get the view matrix (the camera)
    glm::mat4 getViewMatrix() const;

    ///\brief Draws the player using the specified model, view matrix, projection matrix, program, and texture.
    /// \param model The model to use for rendering the player
    /// \param ViewMatrix The view matrix for the scene
    /// \param ProjMatrix The projection matrix for the scene
    /// \param program The shader program to use for rendering
    /// \param textName The texture name or ID to apply to the player
    void drawPlayer(Model& model, glm::mat4 ViewMatrix, glm::mat4 ProjMatrix, Program& program, GLuint textName);*/

    /* *** GETTERS *** */
    glm::vec3 getPosition();

private:
    glm::vec3 m_pos;   // camera position
    float     m_Phi;   // coord spherique de F
    float     m_Theta; // coord spherique de F
    // float     m_RotationAngle;

    glm::vec3 m_FrontVector;
    glm::vec3 m_LeftVector;
    glm::vec3 m_UpVector;

    void computeDirectionVectors();
};

///\brief Handles camera options based on user input, such as player movement and rotation
///\param player The player object to apply the camera options to
///\param left Indicates whether the left movement key or button is pressed
///\param right Indicates whether the right movement key or button is pressed
///\param up Indicates whether the up movement key or button is pressed
///\param down Indicates whether the down movement key or button is pressed
///\param ctx The p6::Context object containing user input events
void cameraOption(Player& camera, bool& left, bool& right, bool& up, bool& down, p6::Context& ctx);
