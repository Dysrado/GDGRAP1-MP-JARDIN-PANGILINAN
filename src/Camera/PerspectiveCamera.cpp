#include "PerspectiveCamera.h"
#include <iostream>

// initializes the camera
void PerspectiveCamera::initialize(glm::vec3 centerPos)
{
    // Computation for the perspective and view matrix
    projection = glm::perspective(glm::radians(60.f), height / width, 0.01f, 100.f);
    distance = 40; // distance from the model to the camera

    this->centerPos = centerPos; // offset if needed
    cameraPos = glm::vec3(0.f, 0.f, distance);
    WorldUp = glm::vec3(0, 1, 0);

    // variables used for mouse input
    mousePosX = 0;
    mousePosY = 0;
    lastMousePosX = 0;
    lastMousePosY = 0;
    mouseOffsetX = 0;
    mouseOffsetY = 0;
    firstMouse = true;

    pitch = 0.f;
    yaw = 90.f;

    // sets the clipping space of the camera
    projection = glm::perspective(glm::radians(60.f), height / width, 0.01f, 500.f);

    // makes the F matrix dependent on the players/users mouse input
    glm::vec3 F = glm::vec3(0, 0, 0);
    F.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
    F.y = sin(glm::radians(pitch));
    F.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));

    F = glm::normalize(F);
    R = glm::normalize(glm::cross(F, WorldUp));
    U = glm::normalize(glm::cross(R, F));
    view = glm::lookAt(cameraPos, centerPos, WorldUp);

}

// moves the camera
void PerspectiveCamera::update(GLFWwindow* window, float deltaTime, glm::vec3 pos)
{
    projection = glm::perspective(glm::radians(60.f), height / width, 0.01f, 500.f);
    F.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
    F.y = sin(glm::radians(pitch));
    F.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));

    F = glm::normalize(F);
    R = glm::normalize(glm::cross(F, WorldUp));
    U = glm::normalize(glm::cross(R, F));

    view = glm::lookAt(cameraPos, F, WorldUp);

    /* Mouse Input */
    glfwGetCursorPos(window, &mousePosX, &mousePosY);

    if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT) == GLFW_PRESS) {
        // initializes the starting point when the player presses the left button
        if (firstMouse) {
            lastMousePosX = mousePosX;
            lastMousePosY = mousePosY;
            firstMouse = false;
        }

        // calculations for offset
        mouseOffsetX = mousePosX - lastMousePosX;
        mouseOffsetY = mousePosY - lastMousePosY;

        // Sets the last value of x and y
        lastMousePosX = mousePosX;
        lastMousePosY = mousePosY;

        // Move the Yaw and Pitch
        pitch += static_cast<GLfloat>(mouseOffsetY) * MOVE_SPEED * deltaTime;
        // moves the camera upwards or downwards depending on the pitch
        cameraPos.z = distance * cos(glm::radians(pitch));
        cameraPos.y = distance * sin(glm::radians(pitch));

        yaw += static_cast<GLfloat>(mouseOffsetX) * MOVE_SPEED * deltaTime;
        // moves the camera left or right depending on the yaw
        cameraPos.x = distance * cos(glm::radians(yaw));
        cameraPos.z = distance * sin(glm::radians(yaw));

        // clamps or limits the pitch and yaw so that the user cannot continuously rotate 
        if (pitch > 90.f) {
            pitch = 90.f;
        }
        else if (pitch < -90.f) {
            pitch = -90.f;
        }

        if (yaw > 360.f || yaw < -360.f) {
            yaw = 0.f;
        }
    }
    else {
        firstMouse = true; // resets the firstMouse to initialize a new value on the mouse inputs later
    }
}

// updates the uniforms depending the given data (projection, view, cameraPos)
void PerspectiveCamera::updateUniforms(GLuint shaderProgram)
{
    unsigned int projectionLoc = glGetUniformLocation(shaderProgram, "projection");
    glUniformMatrix4fv(projectionLoc, 1, GL_FALSE, glm::value_ptr(projection));

    unsigned int viewLoc = glGetUniformLocation(shaderProgram, "view");
    glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view));

    GLuint camPosAddress = glGetUniformLocation(shaderProgram, "cameraPos");
    glUniform3fv(camPosAddress, 1, glm::value_ptr(cameraPos));
}

glm::mat4 PerspectiveCamera::getView()
{
    return view;
}

glm::vec3 PerspectiveCamera::getF()
{
    return this->F;
}

glm::vec3 PerspectiveCamera::getR()
{
    return this->R;
}

glm::vec3 PerspectiveCamera::getU()
{
    return this->U;
}

glm::mat4 PerspectiveCamera::getProj()
{
    return projection;;
}
