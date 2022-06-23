#pragma once
#include "MyCamera.h"
class OrthoCamera :
    public MyCamera
{
public:
	void initialize(glm::vec3 centerPos);
	void update(GLFWwindow* window, float deltaTime, glm::vec3 pos);
	void updateUniforms(GLuint shaderProgram);

private:
	glm::vec3 center;
};

