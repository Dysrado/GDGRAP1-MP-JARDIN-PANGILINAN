#include "OrthoCamera.h"

// initializes the camera
void OrthoCamera::initialize(glm::vec3 centerPos)
{
    // sets the camera's position on top of the center position
    this->cameraPos = glm::vec3(0,100,1);
    center = centerPos;

    glm::vec3 WorldUp = glm::vec3(0, 1, 0);

    // sets the clipping space of the camera
	projection = glm::ortho(-100.f, 100.f, -100.f, 100.f, 0.01f, 100.f);

    glm::vec3 F = glm::normalize(centerPos - this->cameraPos);

    view = glm::lookAt(this->cameraPos, F, WorldUp);
}

// since the camera does not move in this state then nothing happens
void OrthoCamera::update(GLFWwindow* window, float deltaTime, glm::vec3 pos)
{ 
    // sets the camera's position on top of the center position
    this->cameraPos = glm::vec3(0, 100, 1);

    glm::vec3 WorldUp = glm::vec3(0, 1, 0);

    // sets the clipping space of the camera
    projection = glm::ortho(-100.f, 100.f, -100.f, 100.f, 0.01f, 100.f);

    glm::vec3 F = glm::normalize(center - this->cameraPos);

    view = glm::lookAt(this->cameraPos, F, WorldUp);
}

// updates the uniforms depending the given data (projection, view, cameraPos)
void OrthoCamera::updateUniforms(GLuint shaderProgram)
{
    unsigned int projectionLoc = glGetUniformLocation(shaderProgram, "projection");
    glUniformMatrix4fv(projectionLoc, 1, GL_FALSE, glm::value_ptr(projection));

    unsigned int viewLoc = glGetUniformLocation(shaderProgram, "view");
    glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view));

    GLuint camPosAddress = glGetUniformLocation(shaderProgram, "cameraPos");
    glUniform3fv(camPosAddress, 1, glm::value_ptr(this->cameraPos));
}
