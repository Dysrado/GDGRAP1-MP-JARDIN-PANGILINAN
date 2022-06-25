#pragma once
#include "Base.h"
#include "Model3D.h"
class Light
{
public:
    Light();
    ~Light();
    void update(GLuint shaderProgram, GLFWwindow* window, glm::vec3 direction);

private:
   


    // light
    glm::vec3 lightPos = glm::vec3(0, 7,50);
    glm::vec3 lightColor = glm::vec3(1.f, 1.f, 1.f);
   // int decision = 0;
    float ambientStr = 0.1f;
    glm::vec3 ambientColor = lightColor;

    float specStr = 0.5f;
    float specPhong = 16.f;
    int type = 0;
    float lastCDTime;
};






