#pragma once
#include "Base.h"
#include "Light.h"
#include "Shader.h"
class Model3D
{
public:
	Model3D(std::string path, std::string texPath, std::string rgba, std::string vert, std::string frag, bool isPlayer, int lit);
	~Model3D();

	void initialize();
	void initVariables(glm::vec3 position, glm::vec3 rotation, glm::vec3 scale);
	virtual void update(GLFWwindow* window, float deltaTime, bool canMove);
	void updateUniforms();
	void render();
	GLuint getShader();

protected:
	glm::mat4 identity = glm::mat4(1.f);
	glm::mat4 entity;
	glm::vec3 pos;
	glm::vec3 rotation;
	glm::vec3 scale;

private:
	//Light* lightObj;
	std::string rgba;
	bool isPlayer;

	int img_width, img_height, colorChannels;
	int img_width2, img_height2, colorChannels2;

	std::vector<GLuint> mesh_indices;
	std::vector<GLfloat> fullVertexData;

	std::vector<glm::vec3> tangents;
	std::vector<glm::vec3> biTangents;

	// Textures
	GLuint texture;
	unsigned char* tex_bytes;

	GLuint norm_tex;
	unsigned char* norm_bytes;

	Shader* shader;

	GLuint shaderProgram;
	GLuint VAO, VBO;

	int lit;

};

