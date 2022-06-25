#pragma once
#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <iostream>
class PerspectiveCamera
{
public:
    void initialize(glm::vec3 centerPos1);
    void update(GLFWwindow* window, float deltaTime, glm::vec3 pos);
    void updateUniforms(GLuint shaderProgram);

    // Getters
    glm::mat4 getView();
    glm::mat4 getProj();
    glm::vec3 getF();
    glm::vec3 getR();
    glm::vec3 getU();

private:
    float width = 800;
    float height = 800;

    glm::mat4 projection;
    glm::mat4 view;
    glm::vec3 cameraPos;

    glm::vec3 movement;
    glm::mat4 cameraOrientation;
    glm::mat4 cameraPosMat;


    float distance;
    float MOVE_SPEED = 50.f;
    glm::vec3 centerPos;
    glm::vec3 additional;

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

