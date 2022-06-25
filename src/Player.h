#pragma once
#include "Model3D.h"
class Player : public Model3D
{
public:
	// Constructer and Deconstructer
	Player(std::string path, std::string texPath, std::string rgba, std::string vert, std::string frag, bool isPlayer, int lit);
	~Player();

	// Getter and Setters
	glm::vec3 getPosition();
	glm::vec3 getFront(glm::vec3 cameraPos);
	void setF(glm::vec3 value);
	void setR(glm::vec3 value);

	void update(GLFWwindow* window, float deltaTime, bool canMove);

private:
	const float SPEED = 50.f;
	float turn = 0;
	glm::vec3 F;
	glm::vec3 R;
};

