#include "Ball.h"
#include <iostream>
#include "ObjLoader.h"
#include <glm/gtc/matrix_transform.hpp>

void Ball::LoadBall(std::string ballPath)
{
	ObjLoader objLoader;
	objLoader.LoadObj(ballPath);
	this->indexCount = objLoader.indices.size();
	this->vaoId = objLoader.VAO;
	this->center = glm::vec3(0.0, 0.0, 0.0);
	this->radius = 1.f;
}

void Ball::Draw(GLuint ProgramId)
{
	GLuint transLocation;
	transLocation = glGetUniformLocation(ProgramId, "transformationMatrix");

	glm::mat4 transformationMatrix = glm::translate(glm::mat4(1.0f), this->center);
	glUniformMatrix4fv(transLocation, 1, GL_FALSE, &transformationMatrix[0][0]);

	glBindVertexArray(this->vaoId);
	glDrawElements(GL_TRIANGLES, this->indexCount, GL_UNSIGNED_INT, 0);
}

void Ball::Move()
{
	this->center += this->velocity;
	this->velocity *= 0.98;
}

bool Ball::doesCollision(Pin pin) 
{
	float radiusTotal = this->radius + pin.radius;

	glm::vec3 bottom = glm::vec3(this->center.x, this->center.y , this->center.z - this->radius);

	float distance = glm::distance(bottom, pin.center);

	if (distance < radiusTotal)
		return true;

	return false;
}

void Ball::CheckCollision(std::vector<Pin> pins)
{
	for (auto pin : pins)
	{
		if (doesCollision(pin))
		{
			std::cout << "Collision detected" << "\n";
		}
	}
}


