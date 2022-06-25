#include "OrthoCamera.h"

// initializes the camera
void OrthoCamera::initialize(glm::vec3 centerPos)
{
    glm::mat4 identity(1.0f);
    // sets the camera's position on top of the center position
    this->cameraPos = glm::vec3(0,100,1);
    center = centerPos;

    // sets the clipping space of the camera
	projection = glm::ortho(-200.f, 200.f, -200.f, 200.f, 0.01f, 200.f);

    cameraPosMat = glm::translate(identity, cameraPos * -1.0f); //Camera Position Matrix
    center = glm::vec3(0.f, 3.f, 0.f); //Center Position Matrix

    F = glm::normalize(center - cameraPos); //Forward Vector
    R = glm::normalize(glm::cross(F, WorldUp)); //Right Vector
    U = glm::normalize(glm::cross(R, F)); //Up Vector

    cameraOrientation = glm::mat4(glm::vec4(R, 0), glm::vec4(U, 0), glm::vec4((F * -1.0f), 0), glm::vec4(glm::vec3(0, 0, 0), 1)); //Camera Orientation Matrix
    view = cameraOrientation * cameraPosMat; //View Matrix 
}

// since the camera does not move in this state then nothing happens
void OrthoCamera::update(GLFWwindow* window, float deltaTime, glm::vec3 pos)
{ 
    // sets the camera's position on top of the center position
    this->cameraPos = glm::vec3(0, 100, 1);

    // sets the clipping space of the camera
    projection = glm::ortho(-200.f, 200.f, -200.f, 200.f, 0.01f, 200.f);

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

glm::mat4 OrthoCamera::getView()
{
    return view;
}

glm::mat4 OrthoCamera::getProj()
{
    return projection;
}
