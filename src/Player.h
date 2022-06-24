#pragma once
#include "Model3D.h"
class Player : public Model3D
{
public:
	Player(std::string path, std::string texPath, std::string rgba, std::string vert, std::string frag, bool isPlayer, int lit);
	~Player();

	// Getter and Setters
	glm::vec3 getPosition();
	void setF(glm::vec3 value);
	void setR(glm::vec3 value);

	void update(GLFWwindow* window, float deltaTime, bool canMove);
	glm::vec3 getF();

private:
	const float SPEED = 20.f;
	glm::vec3 F;
	glm::vec3 R;
};

