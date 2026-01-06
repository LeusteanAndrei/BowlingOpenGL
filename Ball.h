#pragma once
#include "glm/glm.hpp"
#include "Pin.h"
#include <vector>
#include <GL/glew.h> 
#include <GL/freeglut.h> 

class Ball
{
public:

	glm::vec3 center;
	float radius;

	glm::vec3 velocity = glm::vec3(0.0, 0.0, 0.0);

	GLuint vaoId;
	int indexCount;

	Ball(){}
	Ball(glm::vec3 center, float radius) : center(center), radius(radius) {};

	void Move();
	void Draw(GLuint ProgramId);
	void LoadBall(std::string ballPath);


	bool doesCollision(Pin pin);
	void CheckCollision(std::vector<Pin> pins);


};
 