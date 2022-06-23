#pragma once
#include "MyCamera.h"
class PerspectiveCamera :
    public MyCamera
{
public:
    void initialize(glm::vec3 centerPos);
    void update(GLFWwindow* window, float deltaTime, glm::vec3 pos);
    void updateUniforms(GLuint shaderProgram);

    // Getters
    glm::mat4 getView();
    glm::vec3 getF();
    glm::vec3 getR();
    glm::vec3 getU();
    glm::mat4 getProj();

private:
    float distance;
    float MOVE_SPEED = 50.f;
    glm::vec3 centerPos;

    glm::vec3 WorldUp = glm::vec3(0, 1, 0);

    double mousePosX;
    double mousePosY;
    double lastMousePosX;
    double lastMousePosY;
    double mouseOffsetX;
    double mouseOffsetY;
    bool firstMouse;

    GLfloat pitch;
    GLfloat yaw;

    glm::vec3 F;
    glm::vec3 R;
    glm::vec3 U;
};

