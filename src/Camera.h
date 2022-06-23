#pragma once
#include "Camera/PerspectiveCamera.h"
#include "Camera/OrthoCamera.h"

class Camera : public PerspectiveCamera, OrthoCamera
{
public:
    void initialize(glm::vec3 centerPos);
    void processInput(GLFWwindow* window);
    void update(GLFWwindow* window, float deltaTime, glm::vec3 pos);
    void updateUniforms(GLuint shaderProgram);

    // Getter
    bool isMovable();


private:
    PerspectiveCamera* pCam;
    OrthoCamera* oCam;
    bool inPers; // checks if in perspective mode

    float lastCDTime;
    glm::vec3 center;

};

