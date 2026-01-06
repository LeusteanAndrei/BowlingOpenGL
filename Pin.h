#pragma once
#include "glm/glm.hpp"
#include "ObjLoader.h"
#include <vector>
#include <GL/glew.h> 
#include <GL/freeglut.h> 
#include <glm/gtc/matrix_transform.hpp>

class Pin
{
	public:

	glm::vec3 center = glm::vec3(0.0, 0.0, 0.0);
	float radius = 1.0, height = 1.0f;

	GLuint vaoId;
	int indexCount;
	static glm::mat4 globalTransformation;
	glm::mat4 rotationMatrix;
	

	Pin() {}
	Pin(glm::vec3 center, float radius, float height) : center(center), radius(radius), height(height) {};

	void Draw(GLuint programId);
	void Move();
	void LoadPin(std::string pinPath);

	Pin fromPin()
	{
		Pin pin;
		pin.vaoId = this->vaoId;
		pin.indexCount = this->indexCount;
		pin.center = this->center;
		return pin;
	}
};