#include "Camera.h"

void Camera::initialize(glm::vec3 centerPos)
{
	this->center = centerPos;
	oCam = new OrthoCamera();
	oCam->initialize(this->center);

	pCam = new PerspectiveCamera();
	pCam->initialize(this->center);
	
	inPers = false;
	lastCDTime = glfwGetTime();
}

void Camera::processInput(GLFWwindow* window)
{
	float cooldownTimer = glfwGetTime();
	if (cooldownTimer > lastCDTime + 0.5f) {
		if (glfwGetKey(window, GLFW_KEY_1) == GLFW_PRESS) {
			lastCDTime = glfwGetTime();
			if (inPers) {
				inPers = false;
			}
			else {
				inPers = true;
			}
		}
	}
	
}

void Camera::update(GLFWwindow* window, float deltaTime, glm::vec3 pos)
{
	if (!inPers) {
		pCam->initialize(this->center);
		PerspectiveCamera::update(window, deltaTime, pos);
	}
	else {
		oCam->initialize(this->center);
		OrthoCamera::update(window, deltaTime, pos);
	}
}

void Camera::updateUniforms(GLuint shaderProgram)
{
	if (!inPers) {
		pCam->updateUniforms(shaderProgram);
	}
	else {
		oCam->updateUniforms(shaderProgram);
	}
}

bool Camera::isMovable()
{
	return inPers;
}
