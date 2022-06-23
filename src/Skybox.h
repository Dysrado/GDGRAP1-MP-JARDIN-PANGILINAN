#pragma once
#include "Base.h"


class Skybox
{
public:
	void initialize(std::string vert, std::string frag);
	void bindBuffers();
	void updateUniforms(glm::mat4 view, glm::mat4 projection);
	void deleteBuffers();

private:
	GLuint skyboxShader;
	unsigned int skyboxVAO, skyboxVBO, skyboxEBO;
	unsigned int skyboxTex;

};

