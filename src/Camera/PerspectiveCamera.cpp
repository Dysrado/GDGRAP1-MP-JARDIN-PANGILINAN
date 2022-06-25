#include "PerspectiveCamera.h"
#include <iostream>

// initializes the camera
void PerspectiveCamera::initialize(glm::vec3 centerPos1)
{
    // Computation for the perspective projection and view matrix
    glm::mat4 identity(1.0f); //Identity Matrix
    projection = glm::perspective(glm::radians(60.0f), height / width, 0.1f, 100.f); //Projection Matrix

    movement.x = 0;
    movement.y = 0;
    movement.z = 40.f;

    cameraPos = glm::vec3(0, 0, -10); //Camera Position
    WorldUp = glm::vec3(0.f, 1.f, 0.f); //World Up Coordinates

    cameraPosMat = glm::translate(identity, cameraPos * -1.0f); //Camera Position Matrix
    centerPos = glm::vec3(0.f, 3.f, 0.f); //Center Position Matrix

    F = glm::normalize(centerPos - cameraPos); //Forward Vector
    R = glm::normalize(glm::cross(F, WorldUp)); //Right Vector
    U = glm::normalize(glm::cross(R, F)); //Up Vector

    //Camera Orientation Matrix and View Matrix 
    cameraOrientation = glm::mat4(
        glm::vec4(R, 0),
        glm::vec4(U, 0),
        glm::vec4((F * -1.0f), 0),
        glm::vec4(glm::vec3(0, 0, 0),
            1)
    );
    view = cameraOrientation * cameraPosMat;
    additional = glm::vec3(0);
}

// Moves the camera
void PerspectiveCamera::update(GLFWwindow* window, float deltaTime, glm::vec3 pos)
{
    //glm::mat4 identity(1.0f);

    //// SUBJECT TO CHANGE ===========================================================================================================
    //cameraPosMat = glm::translate(identity, cameraPos); //Camera Position Matrix
    //projection = glm::perspective(glm::radians(60.f), height / width, 0.01f, 500.f);
    //F.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
    //F.y = sin(glm::radians(pitch));
    //F.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));

    //F = glm::normalize(F);
    //R = glm::normalize(glm::cross(F, WorldUp));
    //U = glm::normalize(glm::cross(R, F));

    //if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) {
    //    additional.x = cameraPos.x - 20.f * F.x * deltaTime;
    //    additional.z = cameraPos.z - 20.f * F.z * deltaTime;
    //    //cameraPos -= 20.f * F * deltaTime;
    //}

    ////Camera Orientation Matrix and View Matrix 
    //cameraOrientation = glm::mat4(
    //    glm::vec4(R, 0),
    //    glm::vec4(U, 0),
    //    glm::vec4((F * -1.0f), 0),
    //    glm::vec4(glm::vec3(0, 0, 0),
    //        1)
    //);
    //view = cameraOrientation * cameraPosMat;

    ///* Mouse Input */
    //glfwGetCursorPos(window, &mousePosX, &mousePosY);

    ////if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT) == GLFW_PRESS) {
    //    // initializes the starting point when the player presses the left button
    //if (firstMouse) {
    //    lastMousePosX = mousePosX;
    //    lastMousePosY = mousePosY;
    //    firstMouse = false;
    //}

    //// calculations for offset
    //mouseOffsetX = mousePosX - lastMousePosX;
    //mouseOffsetY = mousePosY - lastMousePosY;

    //// Sets the last value of x and y
    //lastMousePosX = mousePosX;
    //lastMousePosY = mousePosY;

    //// Move the Yaw and Pitch
    //pitch -= static_cast<GLfloat>(mouseOffsetY) * MOVE_SPEED * deltaTime;
    //// moves the camera upwards or downwards depending on the pitch
    //cameraPos.z = distance * cos(glm::radians(pitch)) + additional.z;
    //cameraPos.y = distance * sin(glm::radians(pitch));

    //yaw -= static_cast<GLfloat>(mouseOffsetX) * MOVE_SPEED * deltaTime;
    //// moves the camera left or right depending on the yaw
    //cameraPos.x = distance * cos(glm::radians(yaw)) + additional.x;
    //cameraPos.z = distance * sin(glm::radians(yaw)) + additional.z;

    // clamps or limits the pitch and yaw so that the user cannot continuously rotate 
    /*if (pitch > 90.f) {
        pitch = 90.f;
    }
    else if (pitch < -90.f) {
        pitch = -90.f;
    }*/

    /*if (yaw > 360.f || yaw < -360.f) {
        yaw = 0.f;
    }*/
    //}
    //else {
    //    firstMouse = true; // resets the firstMouse to initialize a new value on the mouse inputs later
    //}

    /*While the mouse button is being held, takes the offset between the position of when
        it was first pressed and the current position of the cursor. After taking the offset,
        camera pans in the direction dragged by the cursor*/
    F.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
    F.y = sin(glm::radians(pitch));
    F.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));
    F = glm::normalize(F);
    R = glm::normalize(glm::cross(F, WorldUp));
    U = glm::normalize(glm::cross(R, F));
    view = glm::lookAt(cameraPos, cameraPos + F, WorldUp);

    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) {
       cameraPos.x += 20.f * F.x * deltaTime;
       cameraPos.z += 20.f * F.z * deltaTime;
       //cameraPos -= 20.f * F * deltaTime;
    }

    /*if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_1) == GLFW_PRESS) {*/
        glfwGetCursorPos(window, &xpos, &ypos);
        if (mouse == true) {
            lastx = xpos;
            lasty = ypos;
            mouse = false;
        }

        xoffset = xpos - lastx;
        yoffset = lasty - ypos;
        lastx = xpos;
        lasty = ypos;

        yaw += xoffset * sens * deltaTime;
        pitch += yoffset * sens * deltaTime;

        /*Clamp Pitch so that it won't rotate endlessly upwards or downwards*/
        if (pitch > 89.0f)
            pitch = 89.0f;
        if (pitch < -89.0f)
            pitch = -89.0f;
//    }

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
