#include "FreeflyCamera.hpp"

#include "glm/ext/matrix_transform.hpp"
#include "glm/fwd.hpp"
#include "glm/trigonometric.hpp"
#include <p6/p6.h>


Freefly::Freefly():m_Position(glm::vec3(0., 0., 0.)), m_Phi(p6::PI), m_Theta(0.){
	this->computeDirectionVectors();
};

Freefly::Freefly(glm::vec3 position, float phi, float theta):m_Position(position), m_Phi(phi), m_Theta(theta){
	this->computeDirectionVectors();
};

void Freefly::computeDirectionVectors(){
	m_FrontVector = glm::vec3(glm::cos(m_Theta)*glm::sin(m_Phi), glm::sin(m_Theta), glm::cos(m_Theta)*glm::cos(m_Phi));
	m_LeftVector = glm::vec3(glm::sin(m_Phi+(p6::PI/2)), 0, glm::cos(m_Phi+(p6::PI/2)));
	m_UpVector = glm::cross(m_FrontVector, m_LeftVector);
}

void Freefly::moveLeft(float t){
	// m_Position += t*m_LeftVector;
	m_Position += t*m_LeftVector;

    computeDirectionVectors();
}
void Freefly::moveFront(float t){
	m_Position += t*m_FrontVector;
	computeDirectionVectors();
}

void Freefly::rotateLeft(float degrees){
	m_Theta += glm::radians(degrees);
	computeDirectionVectors();
}
void Freefly::rotateUp(float degrees){
	m_Phi += glm::radians(degrees);
	computeDirectionVectors();
}

glm::mat4 Freefly::getViewMatrix() const{
	return glm::lookAt(m_Position, m_FrontVector, m_UpVector);
}


void cameraOption(Freefly &camera, bool &left, bool &right, bool &up, bool &down,p6::Context& ctx)	{
        if(right){
            camera.moveLeft(-0.01f);
        }
        if(left){
            camera.moveLeft(0.01f);
        }
        if(up){
            camera.moveFront(0.01f);
        }
        if(down){
            camera.moveFront(-0.01f);
        }

        ctx.key_pressed = [&right, &up, &left, &down](p6::Key key){
            if(key.logical == "d"){
                right = true;
            }
            if(key.logical == "q"){
                left = true;
            }
            if(key.logical == "z"){
                up = true;
            }
            if(key.logical == "s"){
                down = true;
            }
        };

        ctx.key_released = [&right, &up, &left, &down](p6::Key key){
            if(key.logical == "d"){
                right = false;
            }
            if(key.logical == "q"){
                left = false;
            }
            if(key.logical == "z"){
                up = false;
            }
            if(key.logical == "s"){
                down = false;
            }
        };

        ctx.mouse_dragged = [&camera](const p6::MouseDrag& button) {
            camera.rotateLeft(button.delta.x * 5);
            camera.rotateUp(-button.delta.y * 5);
        };

        ctx.mouse_scrolled = [&](p6::MouseScroll scroll){
            // std::cout << "dx : " << scroll.dx << " et dy : " << scroll.dy << std::endl;
            //dy = -1 recul 
            //dy = 1 avance
            camera.moveFront(-scroll.dy);
        };

		
	
	
        

}