#pragma once
#include "Base.h"
#include "Model3D.h"
class Light
{
public:
    Light();
    ~Light();
    void update(GLuint shaderProgram, glm::vec3 direction, glm::vec3 cameraPos);
    void updateInput(GLFWwindow* window);
private:
   

    glm::vec3 lightPos = glm::vec3(0, 7,100);
    glm::vec3 lightColor = glm::vec3(1.f, 1.f, 1.f);
    float ambientStr;
    glm::vec3 ambientColor = lightColor;

    float specStr = 0.5f;
    float specPhong = 16.f;
    int type = 0;
    float lastCDTime;
};






