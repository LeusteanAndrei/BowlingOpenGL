// Codul sursa este adaptat dupa OpenGLBook.com

#include <windows.h>  // biblioteci care urmeaza sa fie incluse
#include <stdlib.h> // necesare pentru citirea shader-elor
#include <stdio.h>
#include <GL/glew.h> // glew apare inainte de freeglut
#include <GL/freeglut.h> // nu trebuie uitat freeglut.h
#include "loadShaders.h"
#include "Ball.h"
#include "Pin.h"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>


#include <vector>
#include <string>
#include <iostream>
#include "ObjLoader.h"

#include <glm/gtc/quaternion.hpp>
#include <glm/gtx/quaternion.hpp>


//////////////////////////////////////

GLuint
VaoId,
VboId,
ColorBufferId,
ProgramId,VAO, VBO, EBO,
viewLocation,
projLocation;
std::string filePath = "Bowling.obj";
std::vector<Vertex> vertices;
std::vector<unsigned int> indices;

// variabile pentru matricea de vizualizare
float Refx = 0.0f, Refy = 0.0f, Refz = 0.0f;
float alpha = 0.0f, beta = 0.0f, dist = 20.0f;
float Obsx, Obsy, Obsz;
float Vx = 0.0f, Vy = 0.0f, Vz = -1.0f;
float incr_alpha1 = 0.01f, incr_alpha2 = 0.01f;

// variabile pentru matricea de proiectie
float width = 800, height = 600, znear = 1, fov = 30;

float const PI = 3.141592f;
float const U_MIN = -PI / 2, U_MAX = PI / 2, V_MIN = 0, V_MAX = 2 * PI;
int const NR_PARR = 10, NR_MERID = 20;
int const NR_VF = (NR_PARR + 1) * (NR_MERID + 1);
float step_u = (U_MAX - U_MIN) / NR_PARR, step_v = (V_MAX - V_MIN) / NR_MERID;

Ball ball;
Pin pin;
// pentru fereastra de vizualizare 
GLint winWidth = 1000, winHeight = 600;

// vectori
glm::vec3 Obs, PctRef, Vert;

// matrice utilizate
glm::mat4 view, projection;


std::vector<Pin> pins;
float initialHeight = -5.0;
float laneLength = 26.f;
float laneWidth = 5.f;
float pinDepth = 15;

void InitializePins()
{
	
	glm::quat quaternion;
	float  a1, a2, a3, a4;
	a1 = 0.707;
	a2 = 0.707;
	a3 = 0.0;
	a4 = 0.0;
	quaternion = glm::quat(a1, a2, a3, a4);
	glm::mat4 rotMatPin  = glm::toMat4(quaternion);
	float height = initialHeight;
	
	float distanceBetween = 1.0f;
	float depth = -pinDepth;
	//pin.globalTransformation = glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 0.0f, 0.0f)) ;

	pin.rotationMatrix = rotMatPin;
	pin.center.x += depth;
	pin.center += glm::vec3(0.0, 0.0, height);
	pins.push_back(pin);
	
	Pin pin2 = pin.fromPin(); 
	pin2.rotationMatrix = rotMatPin;
	pin2.center += glm::vec3(-distanceBetween, -distanceBetween, 0.0);
	pins.push_back(pin2);

	Pin pin3 = pin.fromPin();
	pin3.rotationMatrix = rotMatPin;
	pin3.center += glm::vec3(-distanceBetween, distanceBetween, 0.0);
	pins.push_back(pin3);



	Pin pin4 = pin.fromPin(); // stanga
	pin4.rotationMatrix = rotMatPin;
	pin4.center += glm::vec3(-2 * distanceBetween, -distanceBetween * 2, 0.0);
	pins.push_back(pin4);

	Pin pin5 = pin.fromPin();
	pin5.rotationMatrix = rotMatPin;
	pin5.center += glm::vec3(-2 * distanceBetween, 0.0, 0.0);
	pins.push_back(pin5);


	Pin pin6 = pin.fromPin();
	pin6.rotationMatrix = rotMatPin;
	pin6.center += glm::vec3(-2 * distanceBetween, distanceBetween * 2, 0.0);
	pins.push_back(pin6);



	Pin pin7 = pin.fromPin();
	pin7.rotationMatrix = rotMatPin;
	pin7.center += glm::vec3(-3 * distanceBetween, -2 * distanceBetween - distanceBetween, 0.0);
	pins.push_back(pin7);


	Pin pin8 = pin.fromPin();
	pin8.rotationMatrix = rotMatPin;
	pin8.center += glm::vec3(-3 * distanceBetween, -distanceBetween, 0.0);
	pins.push_back(pin8);

	Pin pin9 = pin.fromPin();
	pin9.rotationMatrix = rotMatPin;
	pin9.center += glm::vec3(-3 * distanceBetween, +distanceBetween, 0.0);
	pins.push_back(pin9);

	Pin pin10 = pin.fromPin();
	pin10.rotationMatrix = rotMatPin;
	pin10.center += glm::vec3(-3 * distanceBetween, 2 * distanceBetween + distanceBetween, 0.0);
	pins.push_back(pin10);

}

void InitializeBall()
{
	ball.center.z += initialHeight + ball.radius;
	ball.center.x += laneLength - pinDepth;
}
void CreateVBO(void)
{
	// varfurile 
	GLfloat Vertices[] = {
		-30.0f, -laneWidth, initialHeight, 1.0f,
		-30.0f, laneWidth, initialHeight,1.0f,
		30.0f, laneWidth, initialHeight,1.0f,
		30.0f, -laneWidth, initialHeight, 1.0f,

	};

	// culorile, ca atribute ale varfurilor
	GLfloat Colors[] = {
		1.0f, 1.0f, 0.0f, 1.0f,
		1.0f, 1.0f, 0.0f, 1.0f,
		1.0f, 1.0f, 0.0f, 1.0f,
		1.0f, 1.0f, 0.0f, 1.0f,
	};

	// se creeaza un buffer nou
	glGenBuffers(1, &VboId);
	glBindBuffer(GL_ARRAY_BUFFER, VboId);
	glBufferData(GL_ARRAY_BUFFER, sizeof(Vertices), Vertices, GL_STATIC_DRAW);

	glGenVertexArrays(1, &VaoId);
	glBindVertexArray(VaoId);
	glEnableVertexAttribArray(0);
	// 
	glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 0, 0);

	// un nou buffer, pentru culoare
	glGenBuffers(1, &ColorBufferId);
	glBindBuffer(GL_ARRAY_BUFFER, ColorBufferId);
	glBufferData(GL_ARRAY_BUFFER, sizeof(Colors), Colors, GL_STATIC_DRAW);
	// atributul 1 =  culoare
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, 0, 0);
}
void DestroyVBO(void)
{
	glDisableVertexAttribArray(1);
	glDisableVertexAttribArray(0);

	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glDeleteBuffers(1, &ColorBufferId);
	glDeleteBuffers(1, &VboId);

	glBindVertexArray(0);
	glDeleteVertexArrays(1, &VaoId);
}

void CreateShaders(void)
{
	ProgramId = LoadShaders("example.vert", "example.frag");
	glUseProgram(ProgramId);
}
void DestroyShaders(void)
{
	glDeleteProgram(ProgramId);
}

glm::vec3 center;
void Initialize(void)
{
	glClearColor(0.2f, 0.3f, 0.3f, 1.0f); // culoarea de fond a ecranului
	//CreateVBO();
	glEnable(GL_DEPTH_BUFFER);
	glEnable(GL_DEPTH_TEST);

	glm::vec3 min(FLT_MAX);
	glm::vec3 max(-FLT_MAX);

	for (const Vertex& v : vertices) {
		min.x = std::min(min.x, v.position.x);
		min.y = std::min(min.y, v.position.y);
		min.z = std::min(min.z, v.position.z);

		max.x = std::max(max.x, v.position.x);
		max.y = std::max(max.y, v.position.y);
		max.z = std::max(max.z, v.position.z);
	}
	center = (min + max) * 0.5f;
	CreateShaders();
	CreateVBO(); 
	ball.LoadBall("Bowling.obj");
	pin.LoadPin("Pin.obj");
	viewLocation = glGetUniformLocation(ProgramId, "viewShader");
	projLocation = glGetUniformLocation(ProgramId, "projectionShader");
	InitializePins();
	InitializeBall();
}

//	Functie utilizata in setarea matricelor de vizualizare si proiectie;
void SetMVP(void)
{
	//pozitia observatorului
	Obsx = Refx + dist * cos(alpha) * cos(beta);
	Obsy = Refy + dist * cos(alpha) * sin(beta);
	Obsz = Refz + dist * sin(alpha);

	// reperul de vizualizare
	glm::vec3 Obs = glm::vec3(Obsx, Obsy, Obsz);   // se schimba pozitia observatorului	
	glm::vec3 PctRef = glm::vec3(Refx, Refy, Refz); // pozitia punctului de referinta
	glm::vec3 Vert = glm::vec3(Vx, Vy, Vz); // verticala din planul de vizualizare 
	view = glm::lookAt(Obs, PctRef, Vert);
	glUniformMatrix4fv(viewLocation, 1, GL_FALSE, &view[0][0]);

	// matricea de proiectie 
	projection = glm::infinitePerspective(fov, GLfloat(width) / GLfloat(height), znear);
	glUniformMatrix4fv(projLocation, 1, GL_FALSE, &projection[0][0]);
}


void ProcessNormalKeys(unsigned char key, int x, int y)
{
	float ballForce = .1f;

	switch (key) {			//	Apasarea tastelor `-` si `+` schimba pozitia observatorului (se departeaza / aproprie);
	case '-':
		dist += 10.0;
		std::cout << "MINUS";

		break;
	case '+':
		dist -= 10.0;
		std::cout << "PLUS";

		break;
	case ' ':
		ball.velocity = glm::vec3(-ballForce, 0.0f, 0.0f);
		break;
	case 'r':
		ball.center = glm::vec3(0.0f, 0.0f, 0.0f);
		InitializeBall();
		ball.velocity = glm::vec3(0.0f, 0.0f, 0.0f);
		break;
	}

	if (key == 27)
		exit(0);
}
void ProcessSpecialKeys(int key, int xx, int yy)
{
	float ballHoverSpeed = 0.2f;
	switch (key)						//	Procesarea tastelor 'LEFT', 'RIGHT', 'UP', 'DOWN';
	{									//	duce la deplasarea observatorului pe axele Ox si Oy;
	case GLUT_KEY_LEFT:
		ball.center.y -= ballHoverSpeed;
		break;
	case GLUT_KEY_RIGHT:
		ball.center.y += ballHoverSpeed;
		break;

	case GLUT_KEY_UP:
		alpha += incr_alpha1;
		if (abs(alpha - PI / 2) < 0.05)
		{
			incr_alpha1 = 0.f;
		}
		else
		{
			incr_alpha1 = 0.01f;
		}
		break;
	case GLUT_KEY_DOWN:
		alpha -= incr_alpha2;
		if (abs(alpha + PI / 2) < 0.05)
		{
			incr_alpha2 = 0.f;
		}
		else
		{
			incr_alpha2 = 0.01f;
		}
		break;
	}
}

void CheckCollissions()
{
	ball.CheckCollision(pins);
}

void RenderFunction(void)
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	SetMVP();

	GLuint transLocation;
	transLocation = glGetUniformLocation(ProgramId, "transformationMatrix");

	glm::mat4 transformationMatrix = glm::mat4(1.0f);
	glUniformMatrix4fv(transLocation, 1, GL_FALSE, &transformationMatrix[0][0]);
	glBindVertexArray(VaoId);
	glPointSize(20);
	glDrawArrays(GL_TRIANGLE_FAN, 0, 4);

	CheckCollissions();
	ball.Move();


	for (int i = pins.size() - 1; i >= 0; i--)
	{
		pins[i].Draw(ProgramId);
	}
	ball.Draw(ProgramId);

	glutSwapBuffers();
	glFlush();
}
void Cleanup(void)
{
	DestroyShaders();
	DestroyVBO();
}

int main(int argc, char* argv[])
{
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_RGBA | GLUT_DEPTH | GLUT_DOUBLE);
	glutInitWindowPosition(100, 100); 
	glutInitWindowSize(600, 600);
	glutCreateWindow("Grafica pe calculator - primul exemplu"); 
	glewInit(); 
	Initialize();

	glutIdleFunc(RenderFunction);
	glutDisplayFunc(RenderFunction);
	glutKeyboardFunc(ProcessNormalKeys);	
	glutSpecialFunc(ProcessSpecialKeys);
	glutCloseFunc(Cleanup);

	glutMainLoop();
}

