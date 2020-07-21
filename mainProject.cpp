#include <iostream>
#include <string>
#include <map>
#include <list>
#define GLEW_STATIC 1

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/common.hpp>
#include "shaders.h"
#include "Model.h"
#include <time.h>
#include <algorithm>
#include <list>
#include <vector>

//define namespaces for glm and c++ std
using namespace glm;
using namespace std;

#pragma region unitCubes
int createUnitCubeVertexArrayObject()
{
	vec3 whiteColor = glm::vec3(1.0f, 0.8f, 0.8f);
	vec3 posX = glm::vec3(1.0f, 0.0f, 0.0f);
	vec3 posY = glm::vec3(0.0f, 1.0f, 0.0f);
	vec3 posZ = glm::vec3(0.0f, 0.0f, 1.0f);

	// Cube model (position, colors)
	glm::vec3 vertexArray[] = {
		glm::vec3(-0.5f,-0.5f,-0.5f), whiteColor, -posX, //left 
		glm::vec3(-0.5f,-0.5f, 0.5f), whiteColor, -posX,
		glm::vec3(-0.5f, 0.5f, 0.5f), whiteColor, -posX,

		glm::vec3(-0.5f,-0.5f,-0.5f), whiteColor, -posX,
		glm::vec3(-0.5f, 0.5f, 0.5f), whiteColor, -posX,
		glm::vec3(-0.5f, 0.5f,-0.5f), whiteColor, -posX,

		glm::vec3(0.5f, 0.5f,-0.5f), whiteColor, -posZ, // far
		glm::vec3(-0.5f,-0.5f,-0.5f), whiteColor, -posZ,
		glm::vec3(-0.5f, 0.5f,-0.5f), whiteColor, -posZ,

		glm::vec3(0.5f, 0.5f,-0.5f),whiteColor, -posZ,
		glm::vec3(0.5f,-0.5f,-0.5f),whiteColor, -posZ,
		glm::vec3(-0.5f,-0.5f,-0.5f), whiteColor, -posZ,

		glm::vec3(0.5f,-0.5f, 0.5f), whiteColor, -posY, // bottom 
		glm::vec3(-0.5f,-0.5f,-0.5f), whiteColor, -posY,
		glm::vec3(0.5f,-0.5f,-0.5f), whiteColor, -posY,

		glm::vec3(0.5f,-0.5f, 0.5f), whiteColor, -posY,
		glm::vec3(-0.5f,-0.5f, 0.5f), whiteColor, -posY,
		glm::vec3(-0.5f,-0.5f,-0.5f), whiteColor, -posY,

		glm::vec3(-0.5f, 0.5f, 0.5f), whiteColor, posZ, // near
		glm::vec3(-0.5f,-0.5f, 0.5f), whiteColor, posZ,
		glm::vec3(0.5f,-0.5f, 0.5f), whiteColor, posZ,

		glm::vec3(0.5f, 0.5f, 0.5f),whiteColor, posZ,
		glm::vec3(-0.5f, 0.5f, 0.5f), whiteColor, posZ,
		glm::vec3(0.5f,-0.5f, 0.5f), whiteColor, posZ,

		glm::vec3(0.5f, 0.5f, 0.5f), whiteColor, posX, // right 
		glm::vec3(0.5f,-0.5f,-0.5f), whiteColor, posX,
		glm::vec3(0.5f, 0.5f,-0.5f), whiteColor, posX,

		glm::vec3(0.5f,-0.5f,-0.5f), whiteColor, posX,
		glm::vec3(0.5f, 0.5f, 0.5f),whiteColor, posX,
		glm::vec3(0.5f,-0.5f, 0.5f), whiteColor, posX,

		glm::vec3(0.5f, 0.5f, 0.5f), whiteColor, posY, // top 
		glm::vec3(0.5f, 0.5f,-0.5f), whiteColor, posY,
		glm::vec3(-0.5f, 0.5f,-0.5f), whiteColor, posY,

		glm::vec3(0.5f, 0.5f, 0.5f), whiteColor, posY,
		glm::vec3(-0.5f, 0.5f,-0.5f), whiteColor, posY,
		glm::vec3(-0.5f, 0.5f, 0.5f), whiteColor, posY
	};

	// Create a vertex array
	GLuint vertexArrayObject;
	glGenVertexArrays(1, &vertexArrayObject);
	glBindVertexArray(vertexArrayObject);

	// Upload Vertex Buffer to the GPU, keep a reference to it (vertexBufferObject)
	GLuint vertexBufferObject;
	glGenBuffers(1, &vertexBufferObject);
	glBindBuffer(GL_ARRAY_BUFFER, vertexBufferObject);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertexArray), vertexArray, GL_STATIC_DRAW);

	glVertexAttribPointer(0,                   // attribute 0 matches aPos in Vertex Shader
		3,                   // size
		GL_FLOAT,            // type
		GL_FALSE,            // normalized?
		3 * sizeof(glm::vec3), // stride - each vertex contain 3 vec3 (position, color, normal)
		(void*)0             // array buffer offset
	);
	glEnableVertexAttribArray(0);


	glVertexAttribPointer(1,                            // attribute 1 matches aColor in Vertex Shader
		3,
		GL_FLOAT,
		GL_FALSE,
		3 * sizeof(glm::vec3),
		(void*)sizeof(glm::vec3)      // color is offseted a vec3 (comes after position)
	);
	glEnableVertexAttribArray(1);

	
	glVertexAttribPointer(2,                            // attribute 2 matches aNormal in Vertex Shader
		3,
		GL_FLOAT,
		GL_FALSE,
		3 * sizeof(glm::vec3),
		(void*)(2 * sizeof(glm::vec3))      // normal is offseted 2 vec3 (comes after position and color)
	);
	glEnableVertexAttribArray(2);

	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);

	return vertexArrayObject;
}

int createVertexArrayObjectU3()
{
	glm::vec3 vertexArray[] = {
		glm::vec3(-0.5f,-0.5f,-0.5f), glm::vec3(1.0f, 0.0f, 1.0f),
		glm::vec3(-0.5f,-0.5f, 0.5f), glm::vec3(0.0f, 1.0f, 1.0f),
		glm::vec3(-0.5f, 0.5f, 0.5f), glm::vec3(1.0f, 0.0f, 1.0f),

		glm::vec3(-0.5f,-0.5f,-0.5f), glm::vec3(1.0f, 0.0f, 1.0f),
		glm::vec3(-0.5f, 0.5f, 0.5f), glm::vec3(0.0f, 1.0f, 1.0f),
		glm::vec3(-0.5f, 0.5f,-0.5f), glm::vec3(1.0f, 0.0f, 0.0f),

		glm::vec3(0.5f, 0.5f,-0.5f), glm::vec3(0.2f, 0.0f, 0.0f),
		glm::vec3(-0.5f,-0.5f,-0.5f), glm::vec3(0.2f, 0.0f, 0.0f),
		glm::vec3(-0.5f, 0.5f,-0.5f), glm::vec3(0.2f, 0.0f, 0.0f),

		glm::vec3(0.5f, 0.5f,-0.5f), glm::vec3(0.2f, 0.0f, 0.0f),
		glm::vec3(0.5f,-0.5f,-0.5f), glm::vec3(0.2f, 0.0f, 0.0f),
		glm::vec3(-0.5f,-0.5f,-0.5f), glm::vec3(0.2f, 0.0f, 0.0f),

		glm::vec3(0.5f,-0.5f, 0.5f), glm::vec3(1.0f, 0.0f, 0.0f),
		glm::vec3(-0.5f,-0.5f,-0.5f), glm::vec3(1.0f, 0.0f, 1.0f),
		glm::vec3(0.5f,-0.5f,-0.5f), glm::vec3(0.0f, 0.0f, 1.0f),

		glm::vec3(0.5f,-0.5f, 0.5f), glm::vec3(0.0f, 1.0f, 1.0f),
		glm::vec3(-0.5f,-0.5f, 0.5f), glm::vec3(1.0f, 1.0f, 1.0f),
		glm::vec3(-0.5f,-0.5f,-0.5f), glm::vec3(1.0f, 0.0f, 1.0f),

		glm::vec3(-0.5f, 0.5f, 0.5f), glm::vec3(1.0f, 1.0f, 1.0f),
		glm::vec3(-0.5f,-0.5f, 0.5f), glm::vec3(1.0f, 1.0f, 1.0f),
		glm::vec3(0.5f,-0.5f, 0.5f), glm::vec3(1.0f, 1.0f, 1.0f),

		glm::vec3(0.5f, 0.5f, 0.5f), glm::vec3(1.0f, 1.0f, 1.0f),
		glm::vec3(-0.5f, 0.5f, 0.5f), glm::vec3(1.0f, 1.0f, 1.0f),
		glm::vec3(0.5f,-0.5f, 0.5f), glm::vec3(1.0f, 1.0f, 1.0f),

		glm::vec3(0.5f, 0.5f, 0.5f), glm::vec3(1.0f, 0.0f, 1.0f),
		glm::vec3(0.5f,-0.5f,-0.5f), glm::vec3(0.0f, 0.1f, 0.0f),
		glm::vec3(0.5f, 0.5f,-0.5f), glm::vec3(1.0f, 0.0f, 1.0f),

		glm::vec3(0.5f,-0.5f,-0.5f), glm::vec3(1.0f, 0.0f, 1.0f),
		glm::vec3(0.5f, 0.5f, 0.5f), glm::vec3(0.0f, 0.1f, 0.0f),
		glm::vec3(0.5f,-0.5f, 0.5f), glm::vec3(0.0f, 1.0f, 1.0f),

		glm::vec3(0.5f, 0.5f, 0.5f), glm::vec3(1.0f, 0.0f, 1.0f),
		glm::vec3(0.5f, 0.5f,-0.5f), glm::vec3(0.0f, 0.1f, 0.0f),
		glm::vec3(-0.5f, 0.5f,-0.5f), glm::vec3(0.0f, 1.0f, 1.0f),

		glm::vec3(0.5f, 0.5f, 0.5f), glm::vec3(0.0f, 1.0f, 0.0f),
		glm::vec3(-0.5f, 0.5f,-0.5f), glm::vec3(0.0f, 0.0f, 1.0f),
		glm::vec3(-0.5f, 0.5f, 0.5f), glm::vec3(0.0f, 1.0f, 0.0f)
	};

	// Create a vertex array
	GLuint vertexArrayObject;
	glGenVertexArrays(1, &vertexArrayObject);
	glBindVertexArray(vertexArrayObject);

	// Upload Vertex Buffer to the GPU, keep a reference to it (vertexBufferObject)
	GLuint vertexBufferObject;
	glGenBuffers(1, &vertexBufferObject);
	glBindBuffer(GL_ARRAY_BUFFER, vertexBufferObject);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertexArray), vertexArray, GL_STATIC_DRAW);

	glVertexAttribPointer(0,                   // attribute 0 matches aPos in Vertex Shader
		3,                   // size
		GL_FLOAT,            // type
		GL_FALSE,            // normalized?
		2 * sizeof(glm::vec3), // stride - each vertex contain 2 vec3 (position, color)
		(void*)0             // array buffer offset
	);
	glEnableVertexAttribArray(0);


	glVertexAttribPointer(1,                            // attribute 1 matches aColor in Vertex Shader
		3,
		GL_FLOAT,
		GL_FALSE,
		2 * sizeof(glm::vec3),
		(void*)sizeof(glm::vec3)      // color is offseted a vec3 (comes after position)
	);
	glEnableVertexAttribArray(1);

	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);

	return vertexArrayObject;
}

int createVertexArrayObjectR4()
{
	// Cube model

	// Straight from the lab code, except I translated the vertices to start at 0,0,0 because otherwise my brain melts.
	vec3 vertexArray[] = {  // position, color
		vec3(0.0f, 0.0f, 0.0f), vec3(1.0f, 0.0f, 0.0f), //left - red
		vec3(0.0f, 0.0f, 1.0f), vec3(1.0f, 0.0f, 0.0f),
		vec3(0.0f, 1.0f, 1.0f), vec3(1.0f, 0.0f, 0.0f),

		vec3(0.0f, 0.0f, 0.0f), vec3(1.0f, 0.0f, 0.0f),
		vec3(0.0f, 1.0f, 1.0f), vec3(1.0f, 0.0f, 0.0f),
		vec3(0.0f, 1.0f, 0.0f), vec3(1.0f, 0.0f, 0.0f),

		vec3(1.0f, 1.0f, 0.0f), vec3(0.0f, 0.0f, 1.0f), // far - blue
		vec3(0.0f, 0.0f, 0.0f), vec3(0.0f, 0.0f, 1.0f),
		vec3(0.0f, 1.0f, 0.0f), vec3(0.0f, 0.0f, 1.0f),

		vec3(1.0f, 1.0f, 0.0f), vec3(0.0f, 0.0f, 1.0f),
		vec3(1.0f, 0.0f, 0.0f), vec3(0.0f, 0.0f, 1.0f),
		vec3(0.0f, 0.0f, 0.0f), vec3(0.0f, 0.0f, 1.0f),

		vec3(1.0f, 0.0f, 1.0f), vec3(0.0f, 1.0f, 1.0f), // bottom - turquoise
		vec3(0.0f, 0.0f, 0.0f), vec3(0.0f, 1.0f, 1.0f),
		vec3(1.0f, 0.0f, 0.0f), vec3(0.0f, 1.0f, 1.0f),

		vec3(1.0f, 0.0f, 1.0f), vec3(0.0f, 1.0f, 1.0f),
		vec3(0.0f, 0.0f, 1.0f), vec3(0.0f, 1.0f, 1.0f),
		vec3(0.0f, 0.0f, 0.0f), vec3(0.0f, 1.0f, 1.0f),

		vec3(0.0f, 1.0f, 1.0f), vec3(0.0f, 1.0f, 0.0f), // near - green
		vec3(0.0f, 0.0f, 1.0f), vec3(0.0f, 1.0f, 0.0f),
		vec3(1.0f, 0.0f, 1.0f), vec3(0.0f, 1.0f, 0.0f),

		vec3(1.0f, 1.0f, 1.0f), vec3(0.0f, 1.0f, 0.0f),
		vec3(0.0f, 1.0f, 1.0f), vec3(0.0f, 1.0f, 0.0f),
		vec3(1.0f, 0.0f, 1.0f), vec3(0.0f, 1.0f, 0.0f),

		vec3(1.0f, 1.0f, 1.0f), vec3(1.0f, 0.0f, 1.0f), // right - purple
		vec3(1.0f, 0.0f, 0.0f), vec3(1.0f, 0.0f, 1.0f),
		vec3(1.0f, 1.0f, 0.0f), vec3(1.0f, 0.0f, 1.0f),

		vec3(1.0f, 0.0f, 0.0f), vec3(1.0f, 0.0f, 1.0f),
		vec3(1.0f, 1.0f, 1.0f), vec3(1.0f, 0.0f, 1.0f),
		vec3(1.0f, 0.0f, 1.0f), vec3(1.0f, 0.0f, 1.0f),

		vec3(1.0f, 1.0f, 1.0f), vec3(1.0f, 1.0f, 0.0f), // top - yellow
		vec3(1.0f, 1.0f, 0.0f), vec3(1.0f, 1.0f, 0.0f),
		vec3(0.0f, 1.0f, 0.0f), vec3(1.0f, 1.0f, 0.0f),

		vec3(1.0f, 1.0f, 1.0f), vec3(1.0f, 1.0f, 0.0f),
		vec3(0.0f, 1.0f, 0.0f), vec3(1.0f, 1.0f, 0.0f),
		vec3(0.0f, 1.0f, 1.0f), vec3(1.0f, 1.0f, 0.0f)
	};


	// Create a vertex array
	GLuint vertexArrayObject;
	glGenVertexArrays(1, &vertexArrayObject);
	glBindVertexArray(vertexArrayObject);


	// Upload Vertex Buffer to the GPU, keep a reference to it (vertexBufferObject)
	GLuint vertexBufferObject;
	glGenBuffers(1, &vertexBufferObject);
	glBindBuffer(GL_ARRAY_BUFFER, vertexBufferObject);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertexArray), vertexArray, GL_STATIC_DRAW);

	glVertexAttribPointer(0,                   // attribute 0 matches aPos in Vertex Shader
		3,                   // size
		GL_FLOAT,            // type
		GL_FALSE,            // normalized?
		2 * sizeof(vec3), // stride - each vertex contain 2 vec3 (position, color)
		(void*)0             // array buffer offset
	);
	glEnableVertexAttribArray(0);


	glVertexAttribPointer(1,                            // attribute 1 matches aColor in Vertex Shader
		3,
		GL_FLOAT,
		GL_FALSE,
		2 * sizeof(vec3),
		(void*)sizeof(vec3)      // color is offseted a vec3 (comes after position)
	);
	glEnableVertexAttribArray(1);

	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);

	return vertexArrayObject;
}
#pragma endregion

#pragma region lines
int createVertexArrayObjectCoordinateXYZ()
{
	// Cube model (position, colors)
	glm::vec3 vertexArray[] = {
		glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(1.0f, 0.0f, 0.0f), // middle, red
		glm::vec3(5.0f, 0.0f, 0.0f), glm::vec3(1.0f, 0.0f, 0.0f), // right, red
		glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f), // middle, green
		glm::vec3(0.0f, 5.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f), // up, green
		glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 0.0f, 1.0f), // middle, blue
		glm::vec3(0.0f, 0.0f, 5.0f), glm::vec3(0.0f, 0.0f, 1.0f) // near, blue
	};

	// Create a vertex array
	GLuint vertexArrayObject;
	glGenVertexArrays(1, &vertexArrayObject);
	glBindVertexArray(vertexArrayObject);


	// Upload Vertex Buffer to the GPU, keep a reference to it (vertexBufferObject)
	GLuint vertexBufferObject;
	glGenBuffers(1, &vertexBufferObject);
	glBindBuffer(GL_ARRAY_BUFFER, vertexBufferObject);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertexArray), vertexArray, GL_STATIC_DRAW);

	glVertexAttribPointer(0,                   // attribute 0 matches aPos in Vertex Shader
		3,                   // size
		GL_FLOAT,            // type
		GL_FALSE,            // normalized?
		2 * sizeof(glm::vec3), // stride - each vertex contain 2 vec3 (position, color)
		(void*)0             // array buffer offset
	);
	glEnableVertexAttribArray(0);


	glVertexAttribPointer(1,                            // attribute 1 matches aColor in Vertex Shader
		3,
		GL_FLOAT,
		GL_FALSE,
		2 * sizeof(glm::vec3),
		(void*)sizeof(glm::vec3)      // color is offseted a vec3 (comes after position)
	);
	glEnableVertexAttribArray(1);

	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);

	return vertexBufferObject;
}

// Initialize variables for grid size
const int gridSize = 101; // Change only this value to change the grid size. If gridSize is 101 it Will make 100 x 100 squares in a grid
float halfGridSize = gridSize / 2.0f;
float lineLength = gridSize - 1.0f;
int createVertexArrayObjectGridLine()
{
	// Line Vertices Array containing position & colors 
	// One line is drawn by 4 vertices and it's doubled because we need lines in X and in Z therefore it's gridSize * 8
	glm::vec3 vertexArray[gridSize * 8];

	// For loops to add every vertex of position and color of X lines to the grid in the vertex array
	for (int i = 0; i < (gridSize * 8) / 2.0f; ++i)
	{
		if (i % 4 == 1 || i % 4 == 3) {
			vertexArray[i] = {
				glm::vec3(1.0f, 1.0f, 0.0f) // Color vertex (yellow)
			};
		}
		else if (i % 4 == 0) {
			vertexArray[i] = {
				glm::vec3(0.0f, 0.0f, 1.0f * i / 4.0f) // First vertex of position
			};
		}
		else {
			vertexArray[i] = {
				glm::vec3(1.0f * lineLength, 0.0f, 1.0f * ((i - 2) / 4.0f)) // Last vertex of position
			};
		}
	}

	// For loops to add every vertex of position and color of Z lines to the grid in the vertex array starting at index 400
	for (int i = 0; i < (gridSize * 8) / 2.0f; ++i)
	{
		if (i % 4 == 1 || i % 4 == 3) {
			vertexArray[i + (gridSize * 8) / 2] = {
				glm::vec3(1.0f, 1.0f, 0.0f) // Color vertex (yellow)
			};
		}
		else if (i % 4 == 0) {
			vertexArray[i + (gridSize * 8) / 2] = {
				glm::vec3(1.0f * i / 4, 0.0f, 0.0f) // First vertex of position
			};
		}
		else {
			vertexArray[i + (gridSize * 8) / 2] = {
				glm::vec3(1.0f * ((i - 2) / 4), 0.0f, 1.0f * lineLength) // Last vertex of position
			};
		}
	}

	// Create a vertex array
	GLuint vertexArrayObject;
	glGenVertexArrays(1, &vertexArrayObject);
	glBindVertexArray(vertexArrayObject);


	// Upload Vertex Buffer to the GPU, keep a reference to it (vertexBufferObject)
	GLuint vertexBufferObject;
	glGenBuffers(1, &vertexBufferObject);
	glBindBuffer(GL_ARRAY_BUFFER, vertexBufferObject);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertexArray), vertexArray, GL_STATIC_DRAW);

	glVertexAttribPointer(0,                   // attribute 0 matches aPos in Vertex Shader
		3,                   // size
		GL_FLOAT,            // type
		GL_FALSE,            // normalized?
		2 * sizeof(glm::vec3), // stride - each vertex contain 2 vec3 (position, color)
		(void*)0             // array buffer offset
	);
	glEnableVertexAttribArray(0);


	glVertexAttribPointer(1,                            // attribute 1 matches aColor in Vertex Shader
		3,
		GL_FLOAT,
		GL_FALSE,
		2 * sizeof(glm::vec3),
		(void*)sizeof(glm::vec3)      // color is offseted a vec3 (comes after position)
	);
	glEnableVertexAttribArray(1);

	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);

	return vertexBufferObject;
}
#pragma endregion

#pragma region cameraInput
// Camera parameters
float cameraSpeed = 1.0f;
float cameraFastSpeed = 2 * cameraSpeed;
float cameraHorizontalAngle = 90.0f;
float cameraVerticalAngle = -25.0f;
const float cameraAngularSpeed = 60.0f;
float magnificationFactor = 0.25f;
bool cameraFirstPerson = true, panMoveMode = false, angleMoveMode = false, zoomMoveMode = false, fastCam = false;

void handleCameraFlagInputs(GLFWwindow* window) {
	if (glfwGetKey(window, GLFW_KEY_1) == GLFW_PRESS) // move camera down
	{
		cameraFirstPerson = true;
	}

	if (glfwGetKey(window, GLFW_KEY_2) == GLFW_PRESS) // move camera down
	{
		cameraFirstPerson = false;
	}

	fastCam = glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS || glfwGetKey(window, GLFW_KEY_RIGHT_SHIFT) == GLFW_PRESS;

	// On key up, set movement mode flag
	if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_RIGHT) == GLFW_PRESS) {
		panMoveMode = true;
	}

	// On key down , unset movement mode flag
	if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_RIGHT) == GLFW_RELEASE)
	{
		panMoveMode = false;
	}

	if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_MIDDLE) == GLFW_PRESS)
	{
		angleMoveMode = true;
	}

	if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_MIDDLE) == GLFW_RELEASE)
	{
		angleMoveMode = false;
	}

	if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT) == GLFW_PRESS)
	{
		zoomMoveMode = true;
	}

	if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT) == GLFW_RELEASE)
	{
		zoomMoveMode = false;
	}

}

// Camera parameters for view transform
glm::vec3 cameraPosition(0.0f, 15.0f, 30.0f);
int xShift = 12, zShift = 4;
vec3 presetCameraPositions[] = { glm::vec3(0.0f, 15.0f, 30.0f), glm::vec3(-xShift, 5.0f, -zShift),
glm::vec3(xShift, 5.0f, -zShift), glm::vec3(0.0f, 5.0f, 8.0f), glm::vec3(-xShift, 5.0f, 5 * zShift),
glm::vec3(xShift, 5.0f, 5 * zShift) };
int cameraPresetPosCount = 6;
int currentCamPresetPositionIndex = 0;

//Add a delay of 10 frames between camera angle changes, otherwise is can change every
//frame which is pretty chaotic and confusing
int changeDelay = 10;
void handleCameraPositionInputs(GLFWwindow* window) {
	if (changeDelay <= 0) {
		if (glfwGetKey(window, GLFW_KEY_PERIOD) == GLFW_PRESS)
		{
			currentCamPresetPositionIndex++;
			if (currentCamPresetPositionIndex >= cameraPresetPosCount) {
				currentCamPresetPositionIndex = 0;
			}
			cameraPosition = presetCameraPositions[currentCamPresetPositionIndex];
			changeDelay = 10;
			cameraHorizontalAngle = 90.0f;
			cameraVerticalAngle = -25.0f;
			magnificationFactor = 0.25f;
		}

		if (glfwGetKey(window, GLFW_KEY_COMMA) == GLFW_PRESS)
		{
			currentCamPresetPositionIndex--;
			if (currentCamPresetPositionIndex < 0) {
				currentCamPresetPositionIndex = cameraPresetPosCount - 1;
			}
			cameraPosition = presetCameraPositions[currentCamPresetPositionIndex];
			changeDelay = 10;
			cameraHorizontalAngle = 90.0f;
			cameraVerticalAngle = -25.0f;
			magnificationFactor = 0.25f;
		}
	}
	else {
		changeDelay--;
	}
}
#pragma endregion

#pragma region modelInput
//storing the redering mode in a variable 
int renderingMode = GL_TRIANGLES;
void handleRenderingModeInput(GLFWwindow* window) {
	//----------------------------------------------------------------------------------
	//User can change the rendering mode
	if (glfwGetKey(window, GLFW_KEY_P) == GLFW_PRESS) //change to points
	{
		renderingMode = GL_POINTS;
	}

	if (glfwGetKey(window, GLFW_KEY_L) == GLFW_PRESS) //change to lines
	{
		renderingMode = GL_LINES;
	}

	if (glfwGetKey(window, GLFW_KEY_T) == GLFW_PRESS) //change to trianges
	{
		renderingMode = GL_TRIANGLES;
	}
}

mat4 worldOrientationModelMatrix = mat4(1.0f);

// Control variables
float moveSpeed = 1.0f;
float scaleFactor = 1.0f;
float scaleSpeed = 0.25f;

// Model variables
float rotationSpeed = 90.0f;
float modelXRotationAngle = 0.0f;
float modelYRotationAngle = 0.0f;
float modelScaleFactor = 1.0f;
vec3 modelPosition = vec3(0.0f, 0.0f, 0.0f);

// Declaring model matrices
mat4 modelScalingMatrix = mat4(1.0f);
mat4 modelRotationMatrix;
mat4 modelTranslationMatrix;
mat4 sharedModelMatrix;
void handleWorldOrientationInput(GLFWwindow* window, float dt) {
	//Changing World Orientation 
	if (glfwGetKey(window, GLFW_KEY_RIGHT) == GLFW_PRESS) //rotate X axis in anti-clockwise direction
	{
		worldOrientationModelMatrix = worldOrientationModelMatrix * rotate(mat4(1.0f), radians(5.0f), vec3(-1.0f, 0.f, 0.f));
	}

	if (glfwGetKey(window, GLFW_KEY_LEFT) == GLFW_PRESS) //rotate X axis in clockwise direction
	{
		worldOrientationModelMatrix = worldOrientationModelMatrix * rotate(mat4(1.0f), radians(5.0f), vec3(1.0f, 0.f, 0.f));
	}

	if (glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS) //rotate Y axis in anti-clockwise direction
	{
		worldOrientationModelMatrix = worldOrientationModelMatrix * rotate(mat4(1.0f), radians(5.0f), vec3(0.0f, -1.f, 0.f));
	}

	if (glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS) //rotate Y axis in clockwise direction
	{
		worldOrientationModelMatrix = worldOrientationModelMatrix * rotate(mat4(1.0f), radians(5.0f), vec3(0.0f, 1.0f, 0.f));
	}

	//reset world orientation to original settings
	//used Tab to test as I do not have a Home button
	if (glfwGetKey(window, GLFW_KEY_TAB) == GLFW_PRESS || glfwGetKey(window, GLFW_KEY_HOME) == GLFW_PRESS)
	{
		worldOrientationModelMatrix = mat4(1.0f);
		modelScaleFactor = 1.0f;
		modelPosition = vec3(0.0f);
		modelXRotationAngle = 0.0f;
		modelYRotationAngle = 0.0f;
	}

	// Role 4 input handling
	if (glfwGetKey(window, GLFW_KEY_U) == GLFW_PRESS)
	{
		// scale up
		modelScaleFactor += scaleSpeed * dt;
	}

	if (glfwGetKey(window, GLFW_KEY_J) == GLFW_PRESS)
	{
		// scale down
		modelScaleFactor -= scaleSpeed * dt;
	}

	if ((glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS || glfwGetKey(window, GLFW_KEY_RIGHT_SHIFT) == GLFW_PRESS)
		&& glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) // move forwards
	{
		modelPosition += vec3(0.0f, 1.0f, 0.0f) * moveSpeed * dt;
	}

	if ((glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS || glfwGetKey(window, GLFW_KEY_RIGHT_SHIFT) == GLFW_PRESS)
		&& glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) // move backwards
	{
		modelPosition += vec3(0.0f, -1.0f, 0.0f) * moveSpeed * dt;
	}

	if ((glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS || glfwGetKey(window, GLFW_KEY_RIGHT_SHIFT) == GLFW_PRESS)
		&& glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS) // move left 
	{
		modelPosition += vec3(-1.0f, 0.0f, 0.0f) * moveSpeed * dt;
	}

	if ((glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS || glfwGetKey(window, GLFW_KEY_RIGHT_SHIFT) == GLFW_PRESS)
		&& glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS) // move right 
	{
		modelPosition += vec3(1.0f, 0.0f, 0.0f) * moveSpeed * dt;
	}

	if ((glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_RELEASE && glfwGetKey(window, GLFW_KEY_RIGHT_SHIFT) == GLFW_RELEASE)
		&& glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
	{
		modelXRotationAngle -= rotationSpeed * dt;
	}

	if ((glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_RELEASE && glfwGetKey(window, GLFW_KEY_RIGHT_SHIFT) == GLFW_RELEASE)
		&& glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) 
	{
		modelXRotationAngle += rotationSpeed * dt;
	}

	if ((glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_RELEASE && glfwGetKey(window, GLFW_KEY_RIGHT_SHIFT) == GLFW_RELEASE)
		&& glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
	{
		modelYRotationAngle += rotationSpeed * dt;
	}

	if ((glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_RELEASE && glfwGetKey(window, GLFW_KEY_RIGHT_SHIFT) == GLFW_RELEASE)
		&& glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
	{
		modelYRotationAngle -= rotationSpeed * dt;
	}
}
#pragma endregion

#pragma region alphabetStuff
// font taken from https://fontstruct.com/fontstructions/show/716744/3_by_5_pixel_font
// I'm really sorry this is all in one file. I am so freakin illiterate in C++, I have no clue what do.
// But basically, this maps a char to a string that represents a square pattern following the pixel font linked above
std::map<char, char*> alphabet = {
	{ 'A',
		"***"
		"* *"
		"***"
		"* *"
		"* *"
	},
	{ 'B',
		"** "
		"* *"
		"** "
		"* *"
		"** "
	},
	{ 'C',
		" **"
		"*  "
		"*  "
		"*  "
		" **"
	},
	{ 'D',
		"** "
		"* *"
		"* *"
		"* *"
		"** "
	},
	{ 'E',
		"***"
		"*  "
		"***"
		"*  "
		"***"
	},
	{ 'F',
		"***"
		"*  "
		"***"
		"*  "
		"*  "
	},
	{ 'G',
		" **"
		"*  "
		"* *"
		"* *"
		" **"
	},
	{ 'H',
		"* *"
		"* *"
		"***"
		"* *"
		"* *"
	},
	{ 'I',
		"***"
		" * "
		" * "
		" * "
		"***"
	},
	{ 'J',
		"  *"
		"  *"
		"  *"
		"* *"
		"***"
	},
	{ 'K',
		"* *"
		"* *"
		"** "
		"* *"
		"* *"
	},
	{ 'L',
		"*  "
		"*  "
		"*  "
		"*  "
		"***"
	},
	{ 'M',
		"* *"
		"***"
		"* *"
		"* *"
		"* *"
	},
	{ 'N',
		"***"
		"* *"
		"* *"
		"* *"
		"* *"
	},
	{ 'O',
		"***"
		"* *"
		"* *"
		"* *"
		"***"
	},
	{ 'P',
		"***"
		"* *"
		"***"
		"*  "
		"*  "
	},
	{ 'Q',
		"** "
		"** "
		"** "
		"** "
		"***"
	},
	{ 'R',
		"** "
		"* *"
		"** "
		"* *"
		"* *"
	},
	{ 'S',
		" **"
		"*  "
		" * "
		"  *"
		"** "
	},
	{ 'T',
		"***"
		" * "
		" * "
		" * "
		" * "
	},
	{ 'U',
		"* *"
		"* *"
		"* *"
		"* *"
		"***"
	},
	{ 'V',
		"* *"
		"* *"
		"* *"
		"* *"
		"** "
	},
	{ 'W',
		"* *"
		"* *"
		"* *"
		"***"
		"* *"
	},
	{ 'X',
		"* *"
		"* *"
		" * "
		"* *"
		"* *"
	},
	{ 'Y',
		"* *"
		"* *"
		"***"
		" * "
		" * "
	},
	{ 'Z',
		"***"
		"  *"
		" * "
		"*  "
		"***"
	},
	{ '0',
		" * "
		"* *"
		"* *"
		"* *"
		" * "
	},
	{ '1',
		" * "
		"** "
		" * "
		" * "
		"***"
	},
	{ '2',
		"** "
		"  *"
		" * "
		"*  "
		"***"
	},
	{ '3',
		"***"
		"  *"
		" **"
		"  *"
		"***"
	},
	{ '4',
		"* *"
		"* *"
		"***"
		"  *"
		"  *"
	},
	{ '5',
		"***"
		"*  "
		"***"
		"  *"
		"** "
	},
	{ '6',
		" **"
		"*  "
		"***"
		"* *"
		"***"
	},
	{ '7',
		"***"
		"  *"
		" * "
		"*  "
		"*  "
	},
	{ '8',
		"***"
		"* *"
		"***"
		"* *"
		"***"
	},
	{ '9',
		"***"
		"* *"
		"***"
		"  *"
		"** "
	}
};

/*
 * This is the main rendering function.
 * It picks a letter from the alphabet and draws it according to the
 * modelMatrix transform
 */
void drawLetter(char c, int index, mat4 modelMatrix, GLuint worldMatrixLocation)
{
	char* letter = alphabet[c];

	// The primitive matrices (They place each cube to form the letters -- you could actually make all the cubes spin :) )
	mat4 primitiveScalingMatrix = scale(mat4(1.0f), vec3(1.0f)); // You can modify this make the letters thicker (but < 1 is bad, cuz they won't connect anymore)
	mat4 primitiveRotationMatrix = rotate(mat4(1.0f), 0.0f, vec3(1.0f));
	mat4 primitiveTranslationMatrix;

	mat4 worldMatrix;

	for (int i = 0; i < 15; i++)
	{
		if (letter[i] == '*')
		{
			primitiveTranslationMatrix = translate(mat4(1.0f), vec3(-3.5f + (index * 4.0f) + (i % 3)*1.0f, 1.5 + (ceil(i / 3)*-1.0f), -0.5f));
			worldMatrix = modelMatrix * primitiveTranslationMatrix * primitiveRotationMatrix * primitiveScalingMatrix;

			glUniformMatrix4fv(worldMatrixLocation, 1, GL_FALSE, &worldMatrix[0][0]);
			glDrawArrays(renderingMode, 0, 36); // 36 vertices (because cube), starting at index 0
		}
	}
}
#pragma endregion

int numVerticesPerCube = 36;
Model* makeL9Model(int vao) {
	/*  This is the hierarchy for L9, built with Model objects holding other Model objects:

                                      modelL9

                            /                           \

                modelL                                         model9

            /            \                       /           /           \          \

     modelLbottomBar modelLverticalBar      model9top  model9right   model9left  model9bottom

	Each model applies its own TRS transformations to its children recursively, achieving the same affect
	we used to get by multiplying out each matrix many times over, like so:
			glm::mat4 Lpart = scale(glm::mat4(1.0f), glm::vec3(1.0f, 5.0f, 1.0f));
			glm::mat4 modelLbottomBar = L9Matrix * LMatrix * Lpart;
	The recursive calls in draw() now do the above matrix multiplication without us having to specify them for each piece.
	*/


	// Draw L9 using hierarchical modeling, start at the lowest model(s) in the hierarchy
	glm::mat4 setUpScaling = mat4(1.0f);
	glm::mat4 setUpRotation = mat4(1.0f);
	glm::mat4 setUpTranslation = mat4(1.0f);


	// Creating left-part of the letter L
	setUpTranslation = translate(glm::mat4(1.0f), glm::vec3(-1.0f, 0.0f, 0.0f));
	setUpScaling = scale(glm::mat4(1.0f), glm::vec3(1.0f, 5.0f, 1.0f));
	Model* modelLbottomBar = new Model(vao, numVerticesPerCube, vector<Model*>(), setUpTranslation, mat4(1.0f), setUpScaling);


	// Creating right-part of the letter L
	setUpScaling = scale(glm::mat4(1.0f), glm::vec3(2.0f, 1.0f, 1.0f));
	setUpTranslation = translate(glm::mat4(1.0f), glm::vec3(0.5f, -2.0f, 0.0f));
	Model* modelLverticalBar = new Model(vao, numVerticesPerCube, vector<Model*>(), setUpTranslation, mat4(1.0f), setUpScaling);

	// Setting up the letter L
	vector<Model*> LChildren = vector<Model*>();
	LChildren.push_back(modelLbottomBar);
	LChildren.push_back(modelLverticalBar);
	//The pieces of the L are placed such that the entire L is centered at origin on all axes
	//We can then very simply manipulate this modelL to transform the entire L
	//for example, to scoot the L left to make room for the number, making the entire L9 centered.
	setUpTranslation = translate(glm::mat4(1.0f), glm::vec3(-2.0f, 0.0f, 0.0f));
	Model* modelL = new Model(vao, 0, LChildren, setUpTranslation, mat4(1.0f), mat4(1.0f));


	// Creating top-part of the number 9
	setUpTranslation = translate(glm::mat4(1.0f), glm::vec3(0.0f, 2.0f, 0.0f));
	setUpScaling = scale(glm::mat4(1.0f), glm::vec3(2.0f, 1.0f, 1.0f));
	Model* model9top = new Model(vao, numVerticesPerCube, vector<Model*>(), setUpTranslation, mat4(1.0f), setUpScaling);

	// Creating right-part of the number 9
	setUpTranslation = translate(glm::mat4(1.0f), glm::vec3(1.5f, 0.0f, 0.0f));
	setUpScaling = scale(glm::mat4(1.0f), glm::vec3(1.0f, 5.0f, 1.0f));
	Model* model9right = new Model(vao, numVerticesPerCube, vector<Model*>(), setUpTranslation, mat4(1.0f), setUpScaling);

	// Creating left-part of the number 9
	setUpTranslation = translate(glm::mat4(1.0f), glm::vec3(-0.5f, 0.5f, 0.0f));
	setUpScaling = scale(glm::mat4(1.0f), glm::vec3(1.0f, 2.0f, 1.0f));
	Model* model9left = new Model(vao, numVerticesPerCube, vector<Model*>(), setUpTranslation, mat4(1.0f), setUpScaling);

	// Creating bottom-part of the number 9
	setUpTranslation = translate(glm::mat4(1.0f), glm::vec3(0.5f, 0.0f, 0.0f));
	Model* model9bottom = new Model(vao, numVerticesPerCube, vector<Model*>(), setUpTranslation, mat4(1.0f), mat4(1.0f));

	// Setting up the number 9
	vector<Model*> nineChildren = vector<Model*>();
	nineChildren.push_back(model9top);
	nineChildren.push_back(model9right);
	nineChildren.push_back(model9left);
	nineChildren.push_back(model9bottom);
	setUpTranslation = translate(glm::mat4(1.0f), glm::vec3(1.5f, 0.0f, 0.0f));
	Model* model9 = new Model(vao, 0, nineChildren, setUpTranslation, mat4(1.0f), mat4(1.0f));


	// Setting up the entire L9
	// This will be the root, and will be provided with the current world and sharedModel matrices in draw() from main()
	vector<Model*> L9Children = vector<Model*>();
	L9Children.push_back(modelL);
	L9Children.push_back(model9);
	Model* modelL9 = new Model(vao, 0, L9Children, mat4(1.0f), mat4(1.0f), mat4(1.0f));

	return modelL9;
}

void handleExitInput(GLFWwindow* window) {
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
		glfwSetWindowShouldClose(window, true);
}

void checkErrors() {
	int errorValue = glGetError();
	if (errorValue != 0) {
		std::cout << "error of some sort: " << errorValue << std::endl;
	}
}

void useShader(int shaderProgram, mat4 projectionMatrix, mat4 viewMatrix) {
	glUseProgram(shaderProgram);

	GLuint viewMatrixLocation = glGetUniformLocation(shaderProgram, "viewMatrix");
	glUniformMatrix4fv(viewMatrixLocation, 1, GL_FALSE, &viewMatrix[0][0]);

	GLuint projectionMatrixLocation = glGetUniformLocation(shaderProgram, "projectionMatrix");
	glUniformMatrix4fv(projectionMatrixLocation, 1, GL_FALSE, &projectionMatrix[0][0]);
}

void useStandardShader(int defaultShaderProgram, mat4 projectionMatrix, mat4 viewMatrix) {
	useShader(defaultShaderProgram, projectionMatrix, viewMatrix);
}

void useLightingShader(int phongLightShaderProgram, mat4 projectionMatrix, mat4 viewMatrix) {
	useShader(phongLightShaderProgram, projectionMatrix, viewMatrix);

	//Set up vertex shader uniforms
	GLuint lightCamPosLocation = glGetUniformLocation(phongLightShaderProgram, "cameraPosition");
	glUniform3f(lightCamPosLocation, cameraPosition.x, cameraPosition.y, cameraPosition.z);
	GLuint lightPosLocation = glGetUniformLocation(phongLightShaderProgram, "lightPosition");
	glUniform3f(lightPosLocation, 0.0f, 30.0f, 0.0f);

	//Set up fragment shader uniforms
	float kCoefficients = 0.9f, lightCoefficients = 0.3f, lightAttenuationConstants = 1.0f;
	glUniform3f(glGetUniformLocation(phongLightShaderProgram, "matCoefficientKa"), kCoefficients, kCoefficients, kCoefficients);
	glUniform3f(glGetUniformLocation(phongLightShaderProgram, "matCoefficientKd"), kCoefficients, kCoefficients, kCoefficients);
	glUniform3f(glGetUniformLocation(phongLightShaderProgram, "matCoefficientKs"), kCoefficients, kCoefficients, kCoefficients);
	glUniform3f(glGetUniformLocation(phongLightShaderProgram, "matCoefficientShininessA"), kCoefficients, kCoefficients, kCoefficients);
	glUniform3f(glGetUniformLocation(phongLightShaderProgram, "lightCoefficientColor"), lightCoefficients, lightCoefficients, lightCoefficients);
	glUniform3f(glGetUniformLocation(phongLightShaderProgram, "lightComponentAmbientLa"), lightCoefficients, lightCoefficients, lightCoefficients);
	glUniform3f(glGetUniformLocation(phongLightShaderProgram, "lightComponentDiffuseLd"), lightCoefficients, lightCoefficients, lightCoefficients);
	glUniform3f(glGetUniformLocation(phongLightShaderProgram, "lightComponentSpecularLs"), lightCoefficients, lightCoefficients, lightCoefficients);
	glUniform3f(glGetUniformLocation(phongLightShaderProgram, "lightCoefficientAttenuationConstantA"), lightAttenuationConstants, lightAttenuationConstants, lightAttenuationConstants);
	glUniform3f(glGetUniformLocation(phongLightShaderProgram, "lightCoefficientAttenuationConstantB"), lightAttenuationConstants, lightAttenuationConstants, lightAttenuationConstants);
	glUniform3f(glGetUniformLocation(phongLightShaderProgram, "lightCoefficientAttenuationConstantC"), lightAttenuationConstants, lightAttenuationConstants, lightAttenuationConstants);
}

int main(int argc, char*argv[])
{
	checkErrors();

#pragma region windowSetUp
	// Initialize GLFW and OpenGL version
	glfwInit();

#if defined(PLATFORM_OSX)
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 2);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#else
	// On windows, we set OpenGL version to 2.1, to support more hardware
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 2);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 1);
#endif

	// Create Window and rendering context using GLFW, resolution is 800x600
	GLFWwindow* window = glfwCreateWindow(1024, 764, "Comp371 - Group 14 P1", NULL, NULL);
	if (window == NULL)
	{
		std::cerr << "Failed to create GLFW window" << std::endl;
		glfwTerminate();
		return -1;
	}
	glfwMakeContextCurrent(window);

	// Initialize GLEW
	glewExperimental = true; // Needed for core profile
	if (glewInit() != GLEW_OK) {
		std::cerr << "Failed to create GLEW" << std::endl;
		glfwTerminate();
		return -1;
	}

	// Black background
	glClearColor(0.0f, 0.0f, 25 / 225.0f, 1.0f);

	// Compile and link shaders here ...
	GLuint defaultShaderProgram = shader("../Source/COMP371-Group14-Project/modelShader.vs", "../Source/COMP371-Group14-Project/modelShader.fs");
	GLuint phongLightShaderProgram = shader("../Source/COMP371-Group14-Project/lightShader.vs", "../Source/COMP371-Group14-Project/lightShader.fs");

#pragma endregion windowSetUp

	// Camera parameters for view transform
	glm::vec3 cameraLookAt(0.0f, 0.0f, 0.0f);
	glm::vec3 cameraUp(0.0f, 1.0f, 0.0f);

	// Set projection matrix for shader, this won't change
	glm::mat4 projectionMatrix = glm::perspective(70.0f, // field of view in degrees
		1024.0f / 768.0f,  // aspect ratio
		0.01f, 100.0f);   // near and far (near > 0)

		// Set initial view matrix
	glm::mat4 viewMatrix = lookAt(cameraPosition,  // eye
		cameraPosition + cameraLookAt,  // center
		cameraUp); // up

	   
	// Define and upload geometry to the GPU here ...
	//We have a few different cubes since some people did fun colors,
	//And some are centered on origin while others have origin in a corner
	int unitCubeVAO = createUnitCubeVertexArrayObject();
	int rainbowCubeVAO = createVertexArrayObjectU3();
	int vao = createVertexArrayObjectR4();
	int gridVAO = createVertexArrayObjectGridLine();
	int xyzVAO = createVertexArrayObjectCoordinateXYZ();

	//Create hierarchical models
	mat4 L9BaseTranslation = translate(glm::mat4(1.0f), glm::vec3(-halfGridSize, 2.5f, -halfGridSize));	//Model's start pos doesn't change
	Model* l9Model = makeL9Model(unitCubeVAO);

	// For frame time
	float lastFrameTime = glfwGetTime();
	int lastMouseLeftState = GLFW_RELEASE;
	double lastMousePosX, lastMousePosY;
	glfwGetCursorPos(window, &lastMousePosX, &lastMousePosY);

	// Enable Backface culling and depth test
	glEnable(GL_CULL_FACE);
	glEnable(GL_DEPTH_TEST);

	glPointSize(3.0f);

	// Entering Main Loop
	while (!glfwWindowShouldClose(window))
	{
		// Frame time calculation
		float dt = glfwGetTime() - lastFrameTime;
		lastFrameTime += dt;

		// Each frame, reset color of each pixel to glClearColor
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		useStandardShader(defaultShaderProgram, projectionMatrix, viewMatrix);

		GLuint worldMatrixLocation = glGetUniformLocation(defaultShaderProgram, "worldMatrix");

		// Model Matrices - they control the transformations of the letters model
		modelScalingMatrix = scale(mat4(1.0f), vec3(1.0f, 1.0f, 1.0f) * modelScaleFactor);
		modelRotationMatrix = rotate(mat4(1.0f), radians(modelYRotationAngle), vec3(0.0f, 1.0f, 0.0f)) * rotate(mat4(1.0f), radians(modelXRotationAngle), vec3(1.0f, 0.0f, 0.0f));
		modelTranslationMatrix = translate(mat4(1.0f), modelPosition);
		sharedModelMatrix = modelTranslationMatrix * modelScalingMatrix * modelRotationMatrix;

#pragma region Grid and Coordinate Axis
		// Draw ground using Hierarchical Modeling
		glBindVertexArray(gridVAO);

		// Initialize variables for grid size
		glm::mat4 GridX = worldOrientationModelMatrix * translate(mat4(1.0f), vec3(-1.0f * gridSize / 2.0f, 0.0f, -1.0f * gridSize / 2.0f));
		glUniformMatrix4fv(worldMatrixLocation, 1, GL_FALSE, &GridX[0][0]);
		glDrawArrays(GL_LINES, 0, 2 * (gridSize * 2));


		// Set up Coordinate Axis Matrix using Hierarchical Modeling
		glm::mat4 Coordinates = worldOrientationModelMatrix * translate(glm::mat4(1.0f), glm::vec3(0.0f, 0.01f, 0.01f));

		int numLines = 3;
		glBindVertexArray(xyzVAO);

		glUniformMatrix4fv(worldMatrixLocation, 1, GL_FALSE, &Coordinates[0][0]);
		glLineWidth(5.0f);
		glDrawArrays(GL_LINES, 0, 2 * numLines);

#pragma endregion

		glLineWidth(1.0f);

<<<<<<< HEAD
		useLightingShader(phongLightShaderProgram, projectionMatrix, viewMatrix);
		worldMatrixLocation = glGetUniformLocation(phongLightShaderProgram, "worldMatrix");


#pragma region L9
		glBindVertexArray(unitCubeVBO);

		// Draw L9 using hierarchical modeling
		// Setting up the L9 Matrix - Changing the values of the translation of L9 will change its position in the world
		// The rightmost translation matrix ensures the model's Y-rotation axis is centered
		//glm::mat4 L9Matrix = worldOrientationModelMatrix * translate(glm::mat4(1.0f), glm::vec3(-halfGridSize, 2.5f, -halfGridSize)) * sharedModelMatrix * translate(glm::mat4(1.0f), glm::vec3(-3.5f, 0.0f, 0.0f));
		glm::mat4 L9Matrix = worldOrientationModelMatrix * translate(glm::mat4(1.0f), glm::vec3(0.0f, 2.5f, 0.0f)) * sharedModelMatrix * translate(glm::mat4(1.0f), glm::vec3(0.0f, 0.0f, 0.0f));

		// Setting up the letter L
		glm::mat4 LMatrix = scale(glm::mat4(1.0f), glm::vec3(1.0f, 1.0f, 1.0f));

		// Creating left-part of the letter L
		glm::mat4 Lpart = scale(glm::mat4(1.0f), glm::vec3(1.0f, 5.0f, 1.0f));

		glm::mat4 Lpart1 = L9Matrix * LMatrix * Lpart;

		glUniformMatrix4fv(worldMatrixLocation, 1, GL_FALSE, &Lpart1[0][0]);
		glDrawArrays(renderingMode, 0, 36);

		// Creating right-part of the letter L
		Lpart = translate(glm::mat4(1.0f), glm::vec3(1.5f, -2.0f, 0.0f)) * scale(glm::mat4(1.0f), glm::vec3(2.0f, 1.0f, 1.0f));

		glm::mat4 Lpart2 = L9Matrix * LMatrix * Lpart;

		glUniformMatrix4fv(worldMatrixLocation, 1, GL_FALSE, &Lpart2[0][0]);
		glDrawArrays(renderingMode, 0, 36);

		// Setting up the number 9
		glm::mat4 Num9Matrix = translate(glm::mat4(1.0f), glm::vec3(4.5f, 0.0f, 0.0f));

		// Creating top-part of the number 9
		glm::mat4 Num9part = translate(glm::mat4(1.0f), glm::vec3(0.0f, 2.0f, 0.0f)) * scale(glm::mat4(1.0f), glm::vec3(2.0f, 1.0f, 1.0f));

		glm::mat4 Num9part1 = L9Matrix * Num9Matrix * Num9part;

		glUniformMatrix4fv(worldMatrixLocation, 1, GL_FALSE, &Num9part1[0][0]);
		glDrawArrays(renderingMode, 0, 36);

		// Creating right-part of the number 9
		Num9part = translate(glm::mat4(1.0f), glm::vec3(1.5f, 0.0f, 0.0f)) * scale(glm::mat4(1.0f), glm::vec3(1.0f, 5.0f, 1.0f));

		glm::mat4 Num9part2 = L9Matrix * Num9Matrix * Num9part;

		glUniformMatrix4fv(worldMatrixLocation, 1, GL_FALSE, &Num9part2[0][0]);
		glDrawArrays(renderingMode, 0, 36);

		// Creating left-part of the number 9
		Num9part = translate(glm::mat4(1.0f), glm::vec3(-0.5f, 0.5f, 0.0f)) * scale(glm::mat4(1.0f), glm::vec3(1.0f, 2.0f, 1.0f));

		glm::mat4 Num9part3 = L9Matrix * Num9Matrix * Num9part;

		glUniformMatrix4fv(worldMatrixLocation, 1, GL_FALSE, &Num9part3[0][0]);
		glDrawArrays(renderingMode, 0, 36);

		// Creating bottom-part of the number 9
		Num9part = translate(glm::mat4(1.0f), glm::vec3(0.5f, 0.0f, 0.0f));

		glm::mat4 Num9part4 = L9Matrix * Num9Matrix * Num9part;

		glUniformMatrix4fv(worldMatrixLocation, 1, GL_FALSE, &Num9part4[0][0]);
		glDrawArrays(renderingMode, 0, 36);
=======
		//Draw L9
		mat4 L9Matrix = worldOrientationModelMatrix * L9BaseTranslation * sharedModelMatrix;
		l9Model->draw(L9Matrix, renderingMode, worldMatrixLocation);
>>>>>>> role3NP

#pragma endregion
/*
#pragma region I9
		glBindVertexArray(unitCubeVAO);

		//define identity matrices to translate, rotate and scale I9 model
		//used for hierarchical modeling 
		mat4 translateI9Model = translate(mat4(1.0f), vec3(halfGridSize - 1, 2.5f, -halfGridSize));

		//----------------------------------------------------------------------------------
		//get the worldview of the model within the scene
		mat4 I9_Model = worldOrientationModelMatrix * translateI9Model * sharedModelMatrix * translate(mat4(1.0f), vec3(0.0f, -2.5f, 0.0f));

		//topI
		mat4 topI = I9_Model * translate(mat4(1.0f), vec3(-3.0f, 4.5f, 0.0f))* scale(mat4(1.0f), vec3(4.0f, 1.0f, 1.0f));
		glUniformMatrix4fv(worldMatrixLocation, 1, GL_FALSE, &topI[0][0]);
		glDrawArrays(renderingMode, 0, 36);

		//middleI
		mat4 middleI = I9_Model * translate(mat4(1.0f), vec3(-3.0f, 2.5f, 0.0f)) * scale(mat4(1.0f), vec3(1.0f, 3.0f, 1.0f));
		glUniformMatrix4fv(worldMatrixLocation, 1, GL_FALSE, &middleI[0][0]);
		glDrawArrays(renderingMode, 0, 36);

		//bottomI
		mat4 bottomI = I9_Model * translate(mat4(1.0f), vec3(-3.0f, 0.5f, 0.0f)) * scale(mat4(1.0f), vec3(4.0f, 1.0f, 1.0f));
		glUniformMatrix4fv(worldMatrixLocation, 1, GL_FALSE, &bottomI[0][0]);
		glDrawArrays(renderingMode, 0, 36);

		//top9
		mat4 top9 = I9_Model * translate(mat4(1.0f), vec3(2.75f, 4.5f, 0.0f)) * scale(mat4(1.0f), vec3(1.5f, 1.0f, 1.0f));
		glUniformMatrix4fv(worldMatrixLocation, 1, GL_FALSE, &top9[0][0]);
		glDrawArrays(renderingMode, 0, 36);

		//left9
		mat4 left9 = I9_Model * translate(mat4(1.0f), vec3(1.5f, 3.25f, 0.0f)) * scale(mat4(1.0f), vec3(1.0f, 3.5f, 1.0f));
		glUniformMatrix4fv(worldMatrixLocation, 1, GL_FALSE, &left9[0][0]);
		glDrawArrays(renderingMode, 0, 36);

		//bottom9
		mat4 bottom9 = I9_Model * translate(mat4(1.0f), vec3(2.75f, 2.0f, 0.0f)) * scale(mat4(1.0f), vec3(1.5f, 1.0f, 1.0f));
		glUniformMatrix4fv(worldMatrixLocation, 1, GL_FALSE, &bottom9[0][0]);
		glDrawArrays(renderingMode, 0, 36);


		//right9
		mat4 right9 = I9_Model * translate(mat4(1.0f), vec3(4.0f, 2.51f, 0.0f)) * scale(mat4(1.0f), vec3(1.0f, 5.f, 1.0f));
		glUniformMatrix4fv(worldMatrixLocation, 1, GL_FALSE, &right9[0][0]);
		glDrawArrays(renderingMode, 0, 36);
		//----------------------------------------------------------------------------------
#pragma endregion

#pragma region U3
		glBindVertexArray(rainbowCubeVAO);

		// Transforming the Unit Triangle - La(u)ra Wheatley 400(3)4960

		mat4 translateU3Model = translate(mat4(1.0f), vec3(0.0f, 2.5f, 0.0f));
		mat4 scaleU3Model(1.0f);

		mat4 WorldView_Model = worldOrientationModelMatrix * translateU3Model * scaleU3Model * sharedModelMatrix * translate(mat4(1.0f), vec3(-1.0f, 0.0f, 0.0f));

		// Creating U from unit cube

		mat4 uLeft = WorldView_Model * translate(mat4(1.0f), vec3(-3.0f, 0.0f, 0.0f)) * scale(mat4(1.0f), vec3(1.0f, 5.0f, 1.0f));
		glUniformMatrix4fv(worldMatrixLocation, 1, GL_FALSE, &uLeft[0][0]);
		glDrawArrays(renderingMode, 0, 36);

		mat4 uBottom = WorldView_Model * translate(mat4(1.0f), vec3(-1.5f, -2.0f, 0.0f)) * scale(mat4(1.0f), vec3(2.0f, 1.0f, 1.0f));
		glUniformMatrix4fv(worldMatrixLocation, 1, GL_FALSE, &uBottom[0][0]);
		glDrawArrays(renderingMode, 0, 36);

		mat4 uRight = WorldView_Model * translate(mat4(1.0f), vec3(0.0f, 0.0f, 0.0f)) * scale(mat4(1.0f), vec3(1.0f, 5.0f, 1.0f));
		glUniformMatrix4fv(worldMatrixLocation, 1, GL_FALSE, &uRight[0][0]);
		glDrawArrays(renderingMode, 0, 36);

		// Creating 3 from unit cube

		mat4 threeBase = WorldView_Model * translate(mat4(1.0f), vec3(4.0f, 0.0f, 0.0f)) * scale(mat4(1.0f), vec3(1.0f, 5.0f, 1.0f));
		glUniformMatrix4fv(worldMatrixLocation, 1, GL_FALSE, &threeBase[0][0]);
		glDrawArrays(renderingMode, 0, 36);

		mat4 threeArm1 = WorldView_Model * translate(mat4(1.0f), vec3(2.5f, 2.0f, 0.0f)) * scale(mat4(1.0f), vec3(2.0f, 1.0f, 1.0f));
		glUniformMatrix4fv(worldMatrixLocation, 1, GL_FALSE, &threeArm1[0][0]);
		glDrawArrays(renderingMode, 0, 36);

		mat4 threeArm2 = WorldView_Model * translate(mat4(1.0f), vec3(2.5f, -2.0f, 0.0f)) * scale(mat4(1.0f), vec3(2.0f, 1.0f, 1.0f));
		glUniformMatrix4fv(worldMatrixLocation, 1, GL_FALSE, &threeArm2[0][0]);
		glDrawArrays(renderingMode, 0, 36);

		mat4 threeArm3 = WorldView_Model * translate(mat4(1.0f), vec3(3.0f, 0.0f, 0.0f)) * scale(mat4(1.0f), vec3(1.0f, 1.0f, 1.0f));
		glUniformMatrix4fv(worldMatrixLocation, 1, GL_FALSE, &threeArm3[0][0]);
		glDrawArrays(renderingMode, 0, 36);
#pragma endregion

#pragma region T9
		// Draw geometry
		glBindVertexArray(vao);

		mat4 translateAAModel = worldOrientationModelMatrix * translate(mat4(1.0f), vec3(-halfGridSize, 2.5f, halfGridSize));

		drawLetter('T', 0, translateAAModel * sharedModelMatrix, worldMatrixLocation);
		drawLetter('9', 1, translateAAModel * sharedModelMatrix, worldMatrixLocation);
#pragma endregion

#pragma region C4
		glBindVertexArray(vao);
		int numFaces = 6, numTrianglesPerFace = 2, numVerticesPerTriangle = 3;
		int numTriangles = numFaces * numTrianglesPerFace;

		// controls model hierarchy movement and orientation	
		glm::mat4 groupTranslationMatrix = glm::translate(glm::mat4(1.0f), glm::vec3(halfGridSize - 1, 2.5f, halfGridSize));
		glm::mat4 groupMatrix = worldOrientationModelMatrix * groupTranslationMatrix * sharedModelMatrix * glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 0.0f, -1.0f));
		glm::mat4 cMatrix = glm::translate(glm::mat4(1.0f), glm::vec3(-4.5f, -2.5f, 0.5f));
		glm::mat4 fourMatrix = glm::translate(glm::mat4(1.0f), glm::vec3(3.5f, -2.5f, 0.5f));

		//C
		glm::mat4 cBarMatrix = glm::scale(glm::mat4(1.0f), glm::vec3(1.0f, 5.0f, 1.0f));
		glm::mat4 modelMatrix = groupMatrix * cMatrix * cBarMatrix;
		glUniformMatrix4fv(worldMatrixLocation, 1, GL_FALSE, &modelMatrix[0][0]);
		glDrawArrays(renderingMode, 0, numTriangles * numVerticesPerTriangle);

		glm::mat4 cBaseTranslationMatrix = glm::translate(glm::mat4(1.0f), glm::vec3(1.0f, 0.0f, 0.0f));
		glm::mat4 cBaseScaleMatrix = glm::scale(glm::mat4(1.0f), glm::vec3(3.0f, 1.0f, 1.0f));
		glm::mat4 cBaseMatrix = cBaseTranslationMatrix * cBaseScaleMatrix;
		modelMatrix = groupMatrix * cMatrix * cBaseMatrix;
		glUniformMatrix4fv(worldMatrixLocation, 1, GL_FALSE, &modelMatrix[0][0]);
		glDrawArrays(renderingMode, 0, numTriangles * numVerticesPerTriangle);

		glm::mat4 cTopTranslationMatrix = glm::translate(glm::mat4(1.0f), glm::vec3(1.0f, 4.0f, 0.0f));
		glm::mat4 cTopMatrix = cTopTranslationMatrix * cBaseScaleMatrix;
		modelMatrix = groupMatrix * cMatrix * cTopMatrix;
		glUniformMatrix4fv(worldMatrixLocation, 1, GL_FALSE, &modelMatrix[0][0]);
		glDrawArrays(renderingMode, 0, numTriangles * numVerticesPerTriangle);

		glm::mat4 freshWorld = glm::mat4(1.0f);
		glUniformMatrix4fv(worldMatrixLocation, 1, GL_FALSE, &freshWorld[0][0]);

		//4
		glm::mat4 fourBarMatrix = glm::scale(glm::mat4(1.0f), glm::vec3(1.0f, 5.0f, 1.0f));
		modelMatrix = groupMatrix * fourMatrix * fourBarMatrix;
		glUniformMatrix4fv(worldMatrixLocation, 1, GL_FALSE, &modelMatrix[0][0]);
		glDrawArrays(renderingMode, 0, numTriangles * numVerticesPerTriangle);

		glm::mat4 fourMiddleTranslationMatrix = glm::translate(glm::mat4(1.0f), glm::vec3(-3.0f, 2.0f, 0.0f));
		glm::mat4 fourMiddleScaleMatrix = glm::scale(glm::mat4(1.0f), glm::vec3(3.0f, 1.0f, 1.0f));
		glm::mat4 fourMiddleMatrix = fourMiddleTranslationMatrix * fourMiddleScaleMatrix;
		modelMatrix = groupMatrix * fourMatrix * fourMiddleMatrix;
		glUniformMatrix4fv(worldMatrixLocation, 1, GL_FALSE, &modelMatrix[0][0]);
		glDrawArrays(renderingMode, 0, numTriangles * numVerticesPerTriangle);

		glm::mat4 fourLeftTranslationMatrix = glm::translate(glm::mat4(1.0f), glm::vec3(-3.0f, 3.0f, 0.0f));
		glm::mat4 fourLeftScaleMatrix = glm::scale(glm::mat4(1.0f), glm::vec3(1.0f, 2.0f, 1.0f));
		glm::mat4 fourLeftMatrix = fourLeftTranslationMatrix * fourLeftScaleMatrix;
		modelMatrix = groupMatrix * fourMatrix * fourLeftMatrix;
		glUniformMatrix4fv(worldMatrixLocation, 1, GL_FALSE, &modelMatrix[0][0]);
		glDrawArrays(renderingMode, 0, numTriangles * numVerticesPerTriangle);
#pragma endregion
*/
		// End Frame, include swap interval to prevent blurriness
		glfwSwapBuffers(window);
		glfwSwapInterval(1);
		glfwPollEvents();

		// Handle inputs
		handleCameraFlagInputs(window);
		handleRenderingModeInput(window);
		handleWorldOrientationInput(window, dt);
		handleExitInput(window);
		handleCameraPositionInputs(window);


		float currentCameraSpeed = (fastCam) ? cameraFastSpeed : cameraSpeed;

		// Retrieving mouse coordinates
		double mousePosX, mousePosY;
		glfwGetCursorPos(window, &mousePosX, &mousePosY);

		// Calculating directional movement
		double dx = mousePosX - lastMousePosX;
		double dy = mousePosY - lastMousePosY;

		// Update last position coordinates
		lastMousePosX = mousePosX;
		lastMousePosY = mousePosY;

		cameraVerticalAngle = std::max(-85.0f, std::min(85.0f, cameraVerticalAngle));
		if (cameraHorizontalAngle > 360)
		{
			cameraHorizontalAngle -= 360;
		}
		else if (cameraHorizontalAngle < -360)
		{
			cameraHorizontalAngle += 360;
		}

		float theta = glm::radians(cameraHorizontalAngle);
		float phi = glm::radians(cameraVerticalAngle);

		cameraLookAt = glm::vec3(cosf(phi) * cosf(theta), sinf(phi), -cosf(phi) * sinf(theta));
		glm::vec3 cameraSideVector = glm::cross(cameraLookAt, glm::vec3(0.0f, 1.0f, 0.0f));

		glm::normalize(cameraSideVector);

#pragma region mouseCameraMovement
		/* Begin Part 2 - SIMULTANEOUS MOUSE AND KEY movement */
		if (panMoveMode)
		{
			if (dx > 0) {
				cameraHorizontalAngle -= cameraAngularSpeed * dt * 1 * dx;
			}
			// If position goes toward negative axis, increase cameraposition
			else if (dx < 0) {
				cameraHorizontalAngle += cameraAngularSpeed * dt * -1 * dx;
			}
		}

		if (angleMoveMode)
		{
			if (dy < 0) {
				cameraVerticalAngle += cameraAngularSpeed * dt * -1 * dy;
			}

			else if (dy > 0) {
				cameraVerticalAngle -= cameraAngularSpeed * dt * dy;
			}
		}

		if (zoomMoveMode)
		{
			if (dy > 0) { //INCR
				magnificationFactor = magnificationFactor * 1.5f;
			}

			else if (dy < 0) { // DECR
				magnificationFactor = magnificationFactor / 1.5f;
			}
		}
		/* END Part 2 - SIMULTANEOUS MOUSE AND KEY movement */

		// Update viewMatrix
		viewMatrix = lookAt(cameraPosition, cameraPosition + cameraLookAt, cameraUp);
		viewMatrix = glm::scale(viewMatrix, vec3(magnificationFactor, magnificationFactor, magnificationFactor));

#pragma endregion

	}

	// Shutdown GLFW
	glfwTerminate();

	return 0;
}

