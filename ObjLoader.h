#pragma once
#include "glm/glm.hpp"
#include <string>
#include <GL/glew.h> 
#include <GL/freeglut.h> 
#include <vector>

struct Vertex {
    glm::vec3 position;
    glm::vec3 normal;
    glm::vec2 texCoords;
};

class ObjLoader
{
public:
    std::vector<Vertex> vertices;
    std::vector<unsigned int> indices;
    GLuint VBO, VAO, EBO;

    void LoadObj(std::string filePath);
};