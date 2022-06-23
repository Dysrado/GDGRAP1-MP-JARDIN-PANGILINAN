#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <iostream>

#pragma once
class MyCamera
{
public:
	virtual void initialize();
	virtual void update(GLFWwindow* window, float deltaTime);
	virtual void updateUniforms(GLuint shaderProgram);

protected:
	float width = 800;
	float height = 800;

	glm::mat4 projection;
	glm::mat4 view;
	glm::vec3 cameraPos;
};

