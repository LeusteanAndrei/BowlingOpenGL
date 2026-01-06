#include "Pin.h"
#include "ObjLoader.h"
#include <iostream>

glm::mat4 Pin::globalTransformation = glm::mat4(1.0f); 
void Pin::LoadPin(std::string pinPath)
{
	ObjLoader objLoader;
	objLoader.LoadObj(pinPath);
	this->indexCount = objLoader.indices.size();
	this->vaoId = objLoader.VAO;
}

void Pin::Draw(GLuint ProgramId)
{
	GLuint transLocation;
	transLocation = glGetUniformLocation(ProgramId, "transformationMatrix");
	glm::mat4 translationMatrix = glm::translate(glm::mat4(1.0f), this->center);
	glm::mat4 transMat =  this->globalTransformation * translationMatrix *rotationMatrix;
	glUniformMatrix4fv(transLocation, 1, GL_FALSE, &transMat[0][0]);

	glBindVertexArray(this->vaoId);
	glDrawElements(GL_TRIANGLES, this->indexCount, GL_UNSIGNED_INT, 0);
}