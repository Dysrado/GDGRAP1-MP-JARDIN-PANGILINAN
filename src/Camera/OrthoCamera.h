#pragma once
#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <iostream>
class OrthoCamera
{
public:
	void initialize(glm::vec3 centerPos);
	void update(GLFWwindow* window, float deltaTime, glm::vec3 pos);
	void updateUniforms(GLuint shaderProgram);

	glm::mat4 getView();
	glm::mat4 getProj();

private:
	float width = 800;
	float height = 800;

	glm::mat4 projection;
	glm::mat4 view;
	glm::vec3 cameraPos;

	glm::vec3 WorldUp = glm::vec3(0, 1, 0);

	glm::mat4 cameraOrientation;
	glm::mat4 cameraPosMat;

	glm::vec3 F;
	glm::vec3 R;
	glm::vec3 U;

	glm::vec3 center;
};

