#include "Player.h"
#include <cmath>

Player::Player(std::string path, std::string texPath, std::string rgba, std::string vert, std::string frag, bool isPlayer) : Model3D(path, texPath, rgba, vert, frag, isPlayer)
{
}

Player::~Player()
{
	Model3D::~Model3D();
}

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
    float mag;
    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) { // Move Forward
        pos.x -= F.x * SPEED * deltaTime;
        pos.z -= F.z * SPEED * deltaTime;
        entity = glm::translate(identity, pos);
        entity = glm::scale(entity, scale);
    }
    else if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) { // Move Backward
        pos.x += F.x * SPEED * deltaTime;
        pos.z += F.z * SPEED * deltaTime;
        entity = glm::translate(identity, pos);
        entity = glm::scale(entity, scale);
    }
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS) { // Move Right
        pos.x -= R.x * SPEED * deltaTime;
        pos.z -= R.z * SPEED * deltaTime;
        entity = glm::translate(identity, pos);
        entity = glm::scale(entity, scale);
    }
    else if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS) { // Move Left
        pos.x += R.x * SPEED * deltaTime;
        pos.z += R.z * SPEED * deltaTime;
        entity = glm::translate(identity, pos);
        entity = glm::scale(entity, scale);
    }

    if (glfwGetKey(window, GLFW_KEY_Q) == GLFW_PRESS) { // Move Up
        pos.y += SPEED * deltaTime;
        entity = glm::translate(identity, pos);
        entity = glm::scale(entity, scale);
    }
    else if (glfwGetKey(window, GLFW_KEY_E) == GLFW_PRESS) { // Move Down
        pos.y -= SPEED * deltaTime;
        entity = glm::translate(identity, pos);
        entity = glm::scale(entity, scale);
    }
}
