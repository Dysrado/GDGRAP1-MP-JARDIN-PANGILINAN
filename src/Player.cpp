#include "Player.h"
#include <cmath>

// Initialize the same way as Model3D
Player::Player(std::string path, std::string texPath, std::string rgba, std::string vert, std::string frag, bool isPlayer, int lit) : 
    Model3D(path, texPath, rgba, vert, frag, isPlayer, lit)
{
}

// Deconstruct the same way as Model3D
Player::~Player()
{
	Model3D::~Model3D();
}


/* Getters and Setters */
glm::vec3 Player::getPosition()
{
    return this->pos;
}

void Player::setF(glm::vec3 value)
{
    this->F = value;
}

void Player::setR(glm::vec3 value)
{
    this->R = value;
}


void Player::update(GLFWwindow* window, float deltaTime, bool canMove)
{
    // Based on the F and R move the obj accordingly
    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) { // Move Forward
        pos.x += SPEED * deltaTime * sin(glm::radians(turn));
        pos.z += SPEED * deltaTime * cos(glm::radians(turn));
    }
    else if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) { // Move Backward
        pos.x -= SPEED * deltaTime * sin(glm::radians(turn));
        pos.z -= SPEED * deltaTime * cos(glm::radians(turn));
    }
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS) { // Move Right
        turn -= deltaTime * SPEED;
        /*pos.x += R.x * SPEED * deltaTime;
        pos.z += R.z * SPEED * deltaTime;*/
    }
    else if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS) { // Move Left
        turn += deltaTime * SPEED;
        /*pos.x -= R.x * SPEED * deltaTime;
        pos.z -= R.z * SPEED * deltaTime;*/
    }

    if (glfwGetKey(window, GLFW_KEY_Q) == GLFW_PRESS) { // Move Up
        pos.y += SPEED * deltaTime;
    }
    else if (glfwGetKey(window, GLFW_KEY_E) == GLFW_PRESS) { // Move Down
        pos.y -= SPEED * deltaTime;
    }

    // Update the position, orientation, and scale of the object
    entity = glm::translate(identity, pos);
    entity = glm::rotate(entity, glm::radians(turn), glm::vec3(0, 1, 0));
    entity = glm::scale(entity, scale);
}

glm::vec3 Player::getFront(glm::vec3 cameraPos)
{
    glm::vec3 temp(0.f);
    temp.x-= SPEED  * sin(glm::radians(turn));
    temp.z -= SPEED  * cos(glm::radians(turn));
    return  pos - temp;
}
