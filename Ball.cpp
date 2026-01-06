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
