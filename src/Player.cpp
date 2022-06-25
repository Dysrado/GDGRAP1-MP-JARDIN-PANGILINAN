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
        pos.x += F.x * SPEED * deltaTime;
        pos.z += F.z * SPEED * deltaTime;
    }
    else if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) { // Move Backward
        pos.x -= F.x * SPEED * deltaTime;
        pos.z -= F.z * SPEED * deltaTime;
    }
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS) { // Move Right
        rotation -= deltaTime * SPEED;
        /*pos.x += R.x * SPEED * deltaTime;
        pos.z += R.z * SPEED * deltaTime;*/
    }
    else if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS) { // Move Left
        rotation += deltaTime * SPEED;
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
    entity = glm::rotate(entity, glm::radians(rotation), glm::vec3(0, 1, 0));
    entity = glm::scale(entity, scale);
}

glm::vec3 Player::getF()
{
    return F;
}
