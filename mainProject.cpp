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

const int windowWidth = 1024, windowHeight = 764;
const float depthWidth = 1024, depthHeight = 1024;

#pragma region unitCubes
int createPlaneVertexArrayObject()
{
	vec3 whiteColor = vec3(1.0f, 1.0f, 1.0f);
	vec3 posY = vec3(0.0f, 1.0f, 0.0f);

	vec3 vertexArray[] = {
		vec3(-0.5f, 0.0f,-0.5f), whiteColor, posY,
		vec3(-0.5f, 0.0f, 0.5f), whiteColor, posY,
		vec3(0.5f, 0.0f, 0.5f), whiteColor, posY,

		vec3(0.5f, 0.0f, 0.5f), whiteColor, posY,
		vec3(0.5f, 0.0f, -0.5f), whiteColor, posY,
		vec3(-0.5f, 0.0f, -0.5f), whiteColor, posY
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
		3 * sizeof(vec3), // stride - each vertex contain 3 vec3 (position, color, normal)
		(void*)0             // array buffer offset
	);
	glEnableVertexAttribArray(0);


	glVertexAttribPointer(1,                            // attribute 1 matches aColor in Vertex Shader
		3,
		GL_FLOAT,
		GL_FALSE,
		3 * sizeof(vec3),
		(void*)sizeof(vec3)      // color is offseted a vec3 (comes after position)
	);
	glEnableVertexAttribArray(1);


	glVertexAttribPointer(2,                            // attribute 2 matches aNormal in Vertex Shader
		3,
		GL_FLOAT,
		GL_FALSE,
		3 * sizeof(vec3),
		(void*)(2 * sizeof(vec3))      // normal is offseted 2 vec3 (comes after position and color)
	);
	glEnableVertexAttribArray(2);

	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);

	return vertexArrayObject;
}

int createUnitCubeVertexArrayObject()
{
	vec3 whiteColor = vec3(1.0f, 1.0f, 1.0f);
	vec3 posX = vec3(1.0f, 0.0f, 0.0f);
	vec3 posY = vec3(0.0f, 1.0f, 0.0f);
	vec3 posZ = vec3(0.0f, 0.0f, 1.0f);

	// Cube model (position, colors, normals)
	vec3 vertexArray[] = {
		vec3(-0.5f,-0.5f,-0.5f), whiteColor, -posX, //left 
		vec3(-0.5f,-0.5f, 0.5f), whiteColor, -posX,
		vec3(-0.5f, 0.5f, 0.5f), whiteColor, -posX,

		vec3(-0.5f,-0.5f,-0.5f), whiteColor, -posX,
		vec3(-0.5f, 0.5f, 0.5f), whiteColor, -posX,
		vec3(-0.5f, 0.5f,-0.5f), whiteColor, -posX,

		vec3(0.5f, 0.5f,-0.5f), whiteColor, -posZ, // far
		vec3(-0.5f,-0.5f,-0.5f), whiteColor, -posZ,
		vec3(-0.5f, 0.5f,-0.5f), whiteColor, -posZ,

		vec3(0.5f, 0.5f,-0.5f),whiteColor, -posZ,
		vec3(0.5f,-0.5f,-0.5f),whiteColor, -posZ,
		vec3(-0.5f,-0.5f,-0.5f), whiteColor, -posZ,

		vec3(0.5f,-0.5f, 0.5f), whiteColor, -posY, // bottom 
		vec3(-0.5f,-0.5f,-0.5f), whiteColor, -posY,
		vec3(0.5f,-0.5f,-0.5f), whiteColor, -posY,

		vec3(0.5f,-0.5f, 0.5f), whiteColor, -posY,
		vec3(-0.5f,-0.5f, 0.5f), whiteColor, -posY,
		vec3(-0.5f,-0.5f,-0.5f), whiteColor, -posY,

		vec3(-0.5f, 0.5f, 0.5f), whiteColor, posZ, // near
		vec3(-0.5f,-0.5f, 0.5f), whiteColor, posZ,
		vec3(0.5f,-0.5f, 0.5f), whiteColor, posZ,

		vec3(0.5f, 0.5f, 0.5f),whiteColor, posZ,
		vec3(-0.5f, 0.5f, 0.5f), whiteColor, posZ,
		vec3(0.5f,-0.5f, 0.5f), whiteColor, posZ,

		vec3(0.5f, 0.5f, 0.5f), whiteColor, posX, // right 
		vec3(0.5f,-0.5f,-0.5f), whiteColor, posX,
		vec3(0.5f, 0.5f,-0.5f), whiteColor, posX,

		vec3(0.5f,-0.5f,-0.5f), whiteColor, posX,
		vec3(0.5f, 0.5f, 0.5f),whiteColor, posX,
		vec3(0.5f,-0.5f, 0.5f), whiteColor, posX,

		vec3(0.5f, 0.5f, 0.5f), whiteColor, posY, // top 
		vec3(0.5f, 0.5f,-0.5f), whiteColor, posY,
		vec3(-0.5f, 0.5f,-0.5f), whiteColor, posY,

		vec3(0.5f, 0.5f, 0.5f), whiteColor, posY,
		vec3(-0.5f, 0.5f,-0.5f), whiteColor, posY,
		vec3(-0.5f, 0.5f, 0.5f), whiteColor, posY
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
		3 * sizeof(vec3), // stride - each vertex contain 3 vec3 (position, color, normal)
		(void*)0             // array buffer offset
	);
	glEnableVertexAttribArray(0);


	glVertexAttribPointer(1,                            // attribute 1 matches aColor in Vertex Shader
		3,
		GL_FLOAT,
		GL_FALSE,
		3 * sizeof(vec3),
		(void*)sizeof(vec3)      // color is offseted a vec3 (comes after position)
	);
	glEnableVertexAttribArray(1);


	glVertexAttribPointer(2,                            // attribute 2 matches aNormal in Vertex Shader
		3,
		GL_FLOAT,
		GL_FALSE,
		3 * sizeof(vec3),
		(void*)(2 * sizeof(vec3))      // normal is offseted 2 vec3 (comes after position and color)
	);
	glEnableVertexAttribArray(2);

	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);

	return vertexArrayObject;
}

int createVertexArrayObjectU3()
{
	vec3 vertexArray[] = {
		vec3(-0.5f,-0.5f,-0.5f), vec3(1.0f, 0.0f, 1.0f),
		vec3(-0.5f,-0.5f, 0.5f), vec3(0.0f, 1.0f, 1.0f),
		vec3(-0.5f, 0.5f, 0.5f), vec3(1.0f, 0.0f, 1.0f),

		vec3(-0.5f,-0.5f,-0.5f), vec3(1.0f, 0.0f, 1.0f),
		vec3(-0.5f, 0.5f, 0.5f), vec3(0.0f, 1.0f, 1.0f),
		vec3(-0.5f, 0.5f,-0.5f), vec3(1.0f, 0.0f, 0.0f),

		vec3(0.5f, 0.5f,-0.5f), vec3(0.2f, 0.0f, 0.0f),
		vec3(-0.5f,-0.5f,-0.5f), vec3(0.2f, 0.0f, 0.0f),
		vec3(-0.5f, 0.5f,-0.5f), vec3(0.2f, 0.0f, 0.0f),

		vec3(0.5f, 0.5f,-0.5f), vec3(0.2f, 0.0f, 0.0f),
		vec3(0.5f,-0.5f,-0.5f), vec3(0.2f, 0.0f, 0.0f),
		vec3(-0.5f,-0.5f,-0.5f), vec3(0.2f, 0.0f, 0.0f),

		vec3(0.5f,-0.5f, 0.5f), vec3(1.0f, 0.0f, 0.0f),
		vec3(-0.5f,-0.5f,-0.5f), vec3(1.0f, 0.0f, 1.0f),
		vec3(0.5f,-0.5f,-0.5f), vec3(0.0f, 0.0f, 1.0f),

		vec3(0.5f,-0.5f, 0.5f), vec3(0.0f, 1.0f, 1.0f),
		vec3(-0.5f,-0.5f, 0.5f), vec3(1.0f, 1.0f, 1.0f),
		vec3(-0.5f,-0.5f,-0.5f), vec3(1.0f, 0.0f, 1.0f),

		vec3(-0.5f, 0.5f, 0.5f), vec3(1.0f, 1.0f, 1.0f),
		vec3(-0.5f,-0.5f, 0.5f), vec3(1.0f, 1.0f, 1.0f),
		vec3(0.5f,-0.5f, 0.5f), vec3(1.0f, 1.0f, 1.0f),

		vec3(0.5f, 0.5f, 0.5f), vec3(1.0f, 1.0f, 1.0f),
		vec3(-0.5f, 0.5f, 0.5f), vec3(1.0f, 1.0f, 1.0f),
		vec3(0.5f,-0.5f, 0.5f), vec3(1.0f, 1.0f, 1.0f),

		vec3(0.5f, 0.5f, 0.5f), vec3(1.0f, 0.0f, 1.0f),
		vec3(0.5f,-0.5f,-0.5f), vec3(0.0f, 0.1f, 0.0f),
		vec3(0.5f, 0.5f,-0.5f), vec3(1.0f, 0.0f, 1.0f),

		vec3(0.5f,-0.5f,-0.5f), vec3(1.0f, 0.0f, 1.0f),
		vec3(0.5f, 0.5f, 0.5f), vec3(0.0f, 0.1f, 0.0f),
		vec3(0.5f,-0.5f, 0.5f), vec3(0.0f, 1.0f, 1.0f),

		vec3(0.5f, 0.5f, 0.5f), vec3(1.0f, 0.0f, 1.0f),
		vec3(0.5f, 0.5f,-0.5f), vec3(0.0f, 0.1f, 0.0f),
		vec3(-0.5f, 0.5f,-0.5f), vec3(0.0f, 1.0f, 1.0f),

		vec3(0.5f, 0.5f, 0.5f), vec3(0.0f, 1.0f, 0.0f),
		vec3(-0.5f, 0.5f,-0.5f), vec3(0.0f, 0.0f, 1.0f),
		vec3(-0.5f, 0.5f, 0.5f), vec3(0.0f, 1.0f, 0.0f)
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
	vec3 vertexArray[] = {
		vec3(0.0f, 0.0f, 0.0f), vec3(1.0f, 0.0f, 0.0f), // middle, red
		vec3(5.0f, 0.0f, 0.0f), vec3(1.0f, 0.0f, 0.0f), // right, red
		vec3(0.0f, 0.0f, 0.0f), vec3(0.0f, 1.0f, 0.0f), // middle, green
		vec3(0.0f, 5.0f, 0.0f), vec3(0.0f, 1.0f, 0.0f), // up, green
		vec3(0.0f, 0.0f, 0.0f), vec3(0.0f, 0.0f, 1.0f), // middle, blue
		vec3(0.0f, 0.0f, 5.0f), vec3(0.0f, 0.0f, 1.0f) // near, blue
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
	vec3 vertexArray[gridSize * 8];

	// For loops to add every vertex of position and color of X lines to the grid in the vertex array
	for (int i = 0; i < (gridSize * 8) / 2.0f; ++i)
	{
		if (i % 4 == 1 || i % 4 == 3) {
			vertexArray[i] = {
				vec3(1.0f, 1.0f, 0.0f) // Color vertex (yellow)
			};
		}
		else if (i % 4 == 0) {
			vertexArray[i] = {
				vec3(0.0f, 0.0f, 1.0f * i / 4.0f) // First vertex of position
			};
		}
		else {
			vertexArray[i] = {
				vec3(1.0f * lineLength, 0.0f, 1.0f * ((i - 2) / 4.0f)) // Last vertex of position
			};
		}
	}

	// For loops to add every vertex of position and color of Z lines to the grid in the vertex array starting at index 400
	for (int i = 0; i < (gridSize * 8) / 2.0f; ++i)
	{
		if (i % 4 == 1 || i % 4 == 3) {
			vertexArray[i + (gridSize * 8) / 2] = {
				vec3(1.0f, 1.0f, 0.0f) // Color vertex (yellow)
			};
		}
		else if (i % 4 == 0) {
			vertexArray[i + (gridSize * 8) / 2] = {
				vec3(1.0f * i / 4, 0.0f, 0.0f) // First vertex of position
			};
		}
		else {
			vertexArray[i + (gridSize * 8) / 2] = {
				vec3(1.0f * ((i - 2) / 4), 0.0f, 1.0f * lineLength) // Last vertex of position
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
vec3 cameraPosition(0.0f, 15.0f, 30.0f);
int xShift = 12, zShift = 4;
vec3 presetCameraPositions[] = { vec3(0.0f, 15.0f, 30.0f), vec3(-xShift, 5.0f, -zShift),
vec3(xShift, 5.0f, -zShift), vec3(0.0f, 5.0f, 8.0f), vec3(-xShift, 5.0f, 5 * zShift),
vec3(xShift, 5.0f, 5 * zShift) };
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
map<char, char*> alphabet = {
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
			mat4 Lpart = scale(mat4(1.0f), vec3(1.0f, 5.0f, 1.0f));
			mat4 modelLbottomBar = L9Matrix * LMatrix * Lpart;
	The recursive calls in draw() now do the above matrix multiplication without us having to specify them for each piece.
	*/


	// Draw L9 using hierarchical modeling, start at the lowest model(s) in the hierarchy
	mat4 setUpScaling = mat4(1.0f);
	mat4 setUpRotation = mat4(1.0f);
	mat4 setUpTranslation = mat4(1.0f);


	// Creating left-part of the letter L
	setUpTranslation = translate(mat4(1.0f), vec3(-1.0f, 0.0f, 0.0f));
	setUpScaling = scale(mat4(1.0f), vec3(1.0f, 5.0f, 1.0f));
	Model* modelLbottomBar = new Model(vao, numVerticesPerCube, vector<Model*>(), setUpTranslation, mat4(1.0f), setUpScaling);


	// Creating right-part of the letter L
	setUpScaling = scale(mat4(1.0f), vec3(2.0f, 1.0f, 1.0f));
	setUpTranslation = translate(mat4(1.0f), vec3(0.5f, -2.0f, 0.0f));
	Model* modelLverticalBar = new Model(vao, numVerticesPerCube, vector<Model*>(), setUpTranslation, mat4(1.0f), setUpScaling);

	// Setting up the letter L
	vector<Model*> LChildren = vector<Model*>();
	LChildren.push_back(modelLbottomBar);
	LChildren.push_back(modelLverticalBar);
	//The pieces of the L are placed such that the entire L is centered at origin on all axes
	//We can then very simply manipulate this modelL to transform the entire L
	//for example, to scoot the L left to make room for the number, making the entire L9 centered.
	setUpTranslation = translate(mat4(1.0f), vec3(-2.0f, 0.0f, 0.0f));
	Model* modelL = new Model(vao, 0, LChildren, setUpTranslation, mat4(1.0f), mat4(1.0f));


	// Creating top-part of the number 9
	setUpTranslation = translate(mat4(1.0f), vec3(0.0f, 2.0f, 0.0f));
	setUpScaling = scale(mat4(1.0f), vec3(2.0f, 1.0f, 1.0f));
	Model* model9top = new Model(vao, numVerticesPerCube, vector<Model*>(), setUpTranslation, mat4(1.0f), setUpScaling);

	// Creating right-part of the number 9
	setUpTranslation = translate(mat4(1.0f), vec3(1.5f, 0.0f, 0.0f));
	setUpScaling = scale(mat4(1.0f), vec3(1.0f, 5.0f, 1.0f));
	Model* model9right = new Model(vao, numVerticesPerCube, vector<Model*>(), setUpTranslation, mat4(1.0f), setUpScaling);

	// Creating left-part of the number 9
	setUpTranslation = translate(mat4(1.0f), vec3(-0.5f, 0.5f, 0.0f));
	setUpScaling = scale(mat4(1.0f), vec3(1.0f, 2.0f, 1.0f));
	Model* model9left = new Model(vao, numVerticesPerCube, vector<Model*>(), setUpTranslation, mat4(1.0f), setUpScaling);

	// Creating bottom-part of the number 9
	setUpTranslation = translate(mat4(1.0f), vec3(0.5f, 0.0f, 0.0f));
	Model* model9bottom = new Model(vao, numVerticesPerCube, vector<Model*>(), setUpTranslation, mat4(1.0f), mat4(1.0f));

	// Setting up the number 9
	vector<Model*> nineChildren = vector<Model*>();
	nineChildren.push_back(model9top);
	nineChildren.push_back(model9right);
	nineChildren.push_back(model9left);
	nineChildren.push_back(model9bottom);
	setUpTranslation = translate(mat4(1.0f), vec3(1.5f, 0.0f, 0.0f));
	Model* model9 = new Model(vao, 0, nineChildren, setUpTranslation, mat4(1.0f), mat4(1.0f));


	// Setting up the entire L9
	// This will be the root, and will be provided with the current world and sharedModel matrices in draw() from main()
	vector<Model*> L9Children = vector<Model*>();
	L9Children.push_back(modelL);
	L9Children.push_back(model9);
	Model* modelL9 = new Model(vao, 0, L9Children, mat4(1.0f), mat4(1.0f), mat4(1.0f));

	return modelL9;
}

Model* makeI9Model(int vao) {
	// Draw I9 using hierarchical modeling, start at the lowest model(s) in the hierarchy
	mat4 setUpScaling = mat4(1.0f);
	mat4 setUpRotation = mat4(1.0f);
	mat4 setUpTranslation = mat4(1.0f);

	// Creating top-part of the letter I
	setUpTranslation = translate(mat4(1.0f), vec3(-3.0f, 4.5f, 0.0f));
	setUpScaling = scale(mat4(1.0f), vec3(4.0f, 1.0f, 1.0f));
	Model* modelItopBar = new Model(vao, numVerticesPerCube, vector<Model*>(), setUpTranslation, mat4(1.0f), setUpScaling);


	// Creating middlet-part of the letter I
	setUpScaling = scale(mat4(1.0f), vec3(1.0f, 3.0f, 1.0f));
	setUpTranslation = translate(mat4(1.0f), vec3(-3.0f, 2.5f, 0.0f));
	Model* modelImiddleBar = new Model(vao, numVerticesPerCube, vector<Model*>(), setUpTranslation, mat4(1.0f), setUpScaling);

	// Creating bottom-part of the letter I
	setUpScaling = scale(mat4(1.0f), vec3(4.0f, 1.0f, 1.0f));
	setUpTranslation = translate(mat4(1.0f), vec3(-3.0f, 0.5f, 0.0f));
	Model* modelIbottomBar = new Model(vao, numVerticesPerCube, vector<Model*>(), setUpTranslation, mat4(1.0f), setUpScaling);

	// Setting up the letter I
	vector<Model*> IChildren = vector<Model*>();
	IChildren.push_back(modelItopBar);
	IChildren.push_back(modelImiddleBar);
	IChildren.push_back(modelIbottomBar);
	//The pieces of the I are placed such that the entire I is centered at origin on all axes
	//We can then very simply manipulate this modelI to transform the entire I
	//for example, to scoot the I left to make room for the number, making the entire I9 centered.
	setUpTranslation = translate(mat4(1.0f), vec3(-2.0f, -2.0f, 0.0f));
	Model* modelI = new Model(vao, 0, IChildren, setUpTranslation, mat4(1.0f), mat4(1.0f));


	// Creating top-part of the number 9
	setUpTranslation = translate(mat4(1.0f), vec3(2.75f, 4.5f, 0.0f));
	setUpScaling = scale(mat4(1.0f), vec3(1.5f, 1.0f, 1.0f));
	Model* model9top = new Model(vao, numVerticesPerCube, vector<Model*>(), setUpTranslation, mat4(1.0f), setUpScaling);

	// Creating left-part of the number 9
	setUpTranslation = translate(mat4(1.0f), vec3(1.5f, 3.25f, 0.0f));
	setUpScaling = scale(mat4(1.0f), vec3(1.0f, 3.5f, 1.0f));
	Model* model9left = new Model(vao, numVerticesPerCube, vector<Model*>(), setUpTranslation, mat4(1.0f), setUpScaling);

	// Creating bottom-part of the number 9
	setUpTranslation = translate(mat4(1.0f), vec3(2.75f, 2.0f, 0.0f));
	setUpScaling = scale(mat4(1.0f), vec3(2.0f, 1.0f, 1.0f));
	Model* model9bottom = new Model(vao, numVerticesPerCube, vector<Model*>(), setUpTranslation, mat4(1.0f), setUpScaling);

	// Creating right-part of the number 9
	setUpTranslation = translate(mat4(1.0f), vec3(4.0f, 2.51f, 0.0f));
	setUpScaling = scale(mat4(1.0f), vec3(1.0f, 5.0f, 1.0f));
	Model* model9right = new Model(vao, numVerticesPerCube, vector<Model*>(), setUpTranslation, mat4(1.0f), setUpScaling);


	// Setting up the number 9
	vector<Model*> nineChildren = vector<Model*>();
	nineChildren.push_back(model9top);
	nineChildren.push_back(model9right);
	nineChildren.push_back(model9left);
	nineChildren.push_back(model9bottom);
	setUpTranslation = translate(mat4(1.0f), vec3(-2.0f, -2.0f, 0.0f));
	Model* model9 = new Model(vao, 0, nineChildren, setUpTranslation, mat4(1.0f), mat4(1.0f));


	// Setting up the entire I9
	// This will be the root, and will be provided with the current world and sharedModel matrices in draw() from main()
	vector<Model*> I9Children = vector<Model*>();
	I9Children.push_back(modelI);
	I9Children.push_back(model9);
	Model* modelI9 = new Model(vao, 0, I9Children, mat4(1.0f), mat4(1.0f), mat4(1.0f));

	return modelI9; 
}

Model* makeU3Model(int vao) {
	// Draw U3 using hierarchical modeling, start at the lowest model(s) in the hierarchy
	mat4 setUpScaling = mat4(1.0f);
	mat4 setUpRotation = mat4(1.0f);
	mat4 setUpTranslation = mat4(1.0f);

	// Creating left-part of the letter U
	setUpTranslation = translate(mat4(1.0f), vec3(-3.0f, 0.0f, 0.0f));
	setUpScaling = scale(mat4(1.0f), vec3(1.0f, 5.0f, 1.0f));
	Model* modelUleftBar = new Model(vao, numVerticesPerCube, vector<Model*>(), setUpTranslation, mat4(1.0f), setUpScaling);


	// Creating bottom-part of the letter U
	setUpScaling = scale(mat4(1.0f), vec3(2.0f, 1.0f, 1.0f));
	setUpTranslation = translate(mat4(1.0f), vec3(-1.5f, -2.0f, 0.0f));
	Model* modelUbottomBar = new Model(vao, numVerticesPerCube, vector<Model*>(), setUpTranslation, mat4(1.0f), setUpScaling);

	// Creating right-part of the letter U
	setUpScaling = scale(mat4(1.0f), vec3(1.0f, 5.0f, 1.0f));
	setUpTranslation = translate(mat4(1.0f), vec3(0.0f, 0.0f, 0.0f));
	Model* modelUrightBar = new Model(vao, numVerticesPerCube, vector<Model*>(), setUpTranslation, mat4(1.0f), setUpScaling);

	// Setting up the letter U
	vector<Model*> UChildren = vector<Model*>();
	UChildren.push_back(modelUleftBar);
	UChildren.push_back(modelUbottomBar);
	UChildren.push_back(modelUrightBar);
	//The pieces of the U are placed such that the entire U is centered at origin on all axes
	//We can then very simply manipulate this modelU to transform the entire U
	//for example, to scoot the U left to make room for the number, making the entire U3 centered.
	setUpTranslation = translate(mat4(1.0f), vec3(-2.0f, 0.0f, 0.0f));
	Model* modelU = new Model(vao, 0, UChildren, setUpTranslation, mat4(1.0f), mat4(1.0f));


	// Creating base-part of the number 3
	setUpTranslation = translate(mat4(1.0f), vec3(4.0f, 0.0f, 0.0f));
	setUpScaling = scale(mat4(1.0f), vec3(1.0f, 5.0f, 1.0f));
	Model* model3base = new Model(vao, numVerticesPerCube, vector<Model*>(), setUpTranslation, mat4(1.0f), setUpScaling);

	// Creating topArm-part of the number 3
	setUpTranslation = translate(mat4(1.0f), vec3(2.5f, 2.0f, 0.0f));
	setUpScaling = scale(mat4(1.0f), vec3(2.0f, 1.0f, 1.0f));
	Model* model3topArm = new Model(vao, numVerticesPerCube, vector<Model*>(), setUpTranslation, mat4(1.0f), setUpScaling);

	// Creating middleArm-part of the number 3
	setUpTranslation = translate(mat4(1.0f), vec3(2.5f, -2.0f, 0.0f));
	setUpScaling = scale(mat4(1.0f), vec3(2.0f, 1.0f, 1.0f));
	Model* model3middleArm = new Model(vao, numVerticesPerCube, vector<Model*>(), setUpTranslation, mat4(1.0f), setUpScaling);

	// Creating bottomArm-part of the number 3
	setUpTranslation = translate(mat4(1.0f), vec3(3.0f, 0.0f, 0.0f));
	setUpScaling = scale(mat4(1.0f), vec3(1.0f, 1.0f, 1.0f));
	Model* model3bottomArm = new Model(vao, numVerticesPerCube, vector<Model*>(), setUpTranslation, mat4(1.0f), setUpScaling);


	// Setting up the number 3
	vector<Model*> threeChildren = vector<Model*>();
	threeChildren.push_back(model3base);
	threeChildren.push_back(model3topArm);
	threeChildren.push_back(model3middleArm);
	threeChildren.push_back(model3bottomArm);
	setUpTranslation = translate(mat4(1.0f), vec3(0.0f, 0.0f, 0.0f));
	Model* model3 = new Model(vao, 0, threeChildren, setUpTranslation, mat4(1.0f), mat4(1.0f));


	// Setting up the entire U3
	// This will be the root, and will be provided with the current world and sharedModel matrices in draw() from main()
	vector<Model*> U3Children = vector<Model*>();
	U3Children.push_back(modelU);
	U3Children.push_back(model3);
	Model* modelU3 = new Model(vao, 0, U3Children, mat4(1.0f), mat4(1.0f), mat4(1.0f));

	return modelU3;
}

Model* makeC4Model(int vao) {
	// Draw C4 using hierarchical modeling, start at the lowest model(s) in the hierarchy
	mat4 setUpScaling = mat4(1.0f);
	mat4 setUpRotation = mat4(1.0f);
	mat4 setUpTranslation = mat4(1.0f);

	// Creating left-part of the letter C
	setUpTranslation = translate(mat4(1.0f), vec3(0.0f, 0.0f, 0.0f));
	setUpScaling = scale(mat4(1.0f), vec3(1.0f, 5.0f, 1.0f));
	Model* modelCleftBar = new Model(vao, numVerticesPerCube, vector<Model*>(), setUpTranslation, mat4(1.0f), setUpScaling);

	// Creating bottom-part of the letter C
	setUpScaling = scale(mat4(1.0f), vec3(3.0f, 1.0f, 1.0f));
	setUpTranslation = translate(mat4(1.0f), vec3(1.0f, 0.0f, 0.0f));
	Model* modelCbottomBar = new Model(vao, numVerticesPerCube, vector<Model*>(), setUpTranslation, mat4(1.0f), setUpScaling);

	// Creating top-part of the letter C
	setUpScaling = scale(mat4(1.0f), vec3(3.0f, 1.0f, 1.0f));
	setUpTranslation = translate(mat4(1.0f), vec3(1.0f, 4.0f, 0.0f));
	Model* modelCtopBar = new Model(vao, numVerticesPerCube, vector<Model*>(), setUpTranslation, mat4(1.0f), setUpScaling);

	// Setting up the letter C
	vector<Model*> CChildren = vector<Model*>();
	CChildren.push_back(modelCleftBar);
	CChildren.push_back(modelCbottomBar);
	CChildren.push_back(modelCtopBar);
	//The pieces of the C are placed such that the entire C is centered at origin on all axes
	//We can then very simply manipulate this modelC to transform the entire C
	//for example, to scoot the C left to make room for the number, making the entire C4 centered.
	setUpTranslation = translate(mat4(1.0f), vec3(-9.0f, 0.0f, 0.0f));
	Model* modelC = new Model(vao, 0, CChildren, setUpTranslation, mat4(1.0f), mat4(1.0f));

	
	// Creating right-part of the number 4
	setUpTranslation = translate(mat4(1.0f), vec3(0.0f, 0.0f, 0.0f));
	setUpScaling = scale(mat4(1.0f), vec3(1.0f, 5.0f, 1.0f));
	Model* model4right = new Model(vao, numVerticesPerCube, vector<Model*>(), setUpTranslation, mat4(1.0f), setUpScaling);

	// Creating middle-part of the number 4
	setUpTranslation = translate(mat4(1.0f), vec3(-3.0f, 2.0f, 0.0f));
	setUpScaling = scale(mat4(1.0f), vec3(3.0f, 1.0f, 1.0f));
	Model* model4middle = new Model(vao, numVerticesPerCube, vector<Model*>(), setUpTranslation, mat4(1.0f), setUpScaling);

	// Creating left-part of the number 4
	setUpTranslation = translate(mat4(1.0f), vec3(-3.0f, 3.0f, 0.0f));
	setUpScaling = scale(mat4(1.0f), vec3(1.0f, 2.0f, 1.0f));
	Model* model4left = new Model(vao, numVerticesPerCube, vector<Model*>(), setUpTranslation, mat4(1.0f), setUpScaling);

	// Setting up the number 4
	vector<Model*> fourChildren = vector<Model*>();
	fourChildren.push_back(model4left);
	fourChildren.push_back(model4middle);
	fourChildren.push_back(model4right);
	setUpTranslation = translate(mat4(1.0f), vec3(0.0f, 0.0f, 0.0f));
	Model* model4 = new Model(vao, 0, fourChildren, setUpTranslation, mat4(1.0f), mat4(1.0f));


	// Setting up the entire C4
	// This will be the root, and will be provided with the current world and sharedModel matrices in draw() from main()
	vector<Model*> C4Children = vector<Model*>();
	C4Children.push_back(modelC);
	C4Children.push_back(model4);
	Model* modelC4 = new Model(vao, 0, C4Children, mat4(1.0f), mat4(1.0f), mat4(1.0f));

	return modelC4; 
}


void handleExitInput(GLFWwindow* window) {
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
		glfwSetWindowShouldClose(window, true);
}

void checkErrors() {
	int errorValue = glGetError();
	if (errorValue != 0) {
		cout << "error of some sort: " << errorValue << endl;
	}
}

GLuint worldMatrixLocation;
void useShader(int shaderProgram, mat4 projectionMatrix, mat4 viewMatrix) {
	glUseProgram(shaderProgram);

	worldMatrixLocation = glGetUniformLocation(shaderProgram, "worldMatrix");

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

	GLuint depthVPLocation = glGetUniformLocation(phongLightShaderProgram, "depthVP");
	mat4 lightProjectionMatrix = perspective(70.0f, (float)depthWidth / depthHeight, 0.01f, 100.0f);
	mat4 lightViewMatrix = lookAt(vec3(0.0f, 30.0f, 0.0f),  // eye
		vec3(0.0f, 0.0f, 0.0f),  // center
		vec3(1.0f, 0.0f, 0.0f)); // up
	mat4 depthVP = lightProjectionMatrix * lightViewMatrix;
	glUniformMatrix4fv(depthVPLocation, 1, GL_FALSE, &depthVP[0][0]);


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

void useShadowShader(int shadowShaderProgram) {
	float aspectRatio = (float)depthWidth / depthHeight;
	mat4 lightProjectionMatrix = perspective(70.0f, aspectRatio, 0.01f, 100.0f);
	mat4 lightViewMatrix = lookAt(vec3(0.0f, 30.0f, 0.0f),  // eye
		vec3(0.0f, 0.0f, 0.0f),  // center
		vec3(1.0f, 0.0f, 0.0f)); // up
	useShader(shadowShaderProgram, lightProjectionMatrix, lightViewMatrix);
}


//The buffer is the memory that backs up the shadowMap texture, like how the VBO is the memory that backs up the VAO
// referenced this tutorial which was suggested in the labs https://learnopengl.com/Getting-started/Textures
// which lead to this tutorial https://learnopengl.com/Advanced-Lighting/Shadows/Shadow-Mapping
// referenced the opengl docs for framebuffer info https://www.khronos.org/registry/OpenGL-Refpages/es2.0/xhtml/glFramebufferTexture2D.xml
GLuint createShadowMapBuffer(GLuint& shadowMap)
{
	//create a frame buffer to hold the shadow map data
	GLuint frameBufferObject;
	glGenFramebuffers(1, &frameBufferObject);

	glGenTextures(1, &shadowMap);
	glBindTexture(GL_TEXTURE_2D, shadowMap);

	glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT, depthWidth, depthHeight, 0, GL_DEPTH_COMPONENT, GL_FLOAT, 0);
	float borderColor[] = { 1.0f, 1.0f, 1.0f, 1.0f };
	glTexParameterfv(GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, borderColor);	//Set up an empty border, instead of having the default repeating texture
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);	//which lead to duplicate shadows out of place!
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	glBindFramebuffer(GL_FRAMEBUFFER, frameBufferObject);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, shadowMap, 0);
	glDrawBuffer(GL_NONE);
	glReadBuffer(GL_NONE);

	if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE) {
		cerr << "error generating shadow map buffer" << endl;
	}

	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	glBindTexture(GL_TEXTURE_2D, 0);

	return frameBufferObject;
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
	GLFWwindow* window = glfwCreateWindow(windowWidth, windowHeight, "Comp371 - Group 14 P1", NULL, NULL);
	if (window == NULL)
	{
		cerr << "Failed to create GLFW window" << endl;
		glfwTerminate();
		return -1;
	}
	glfwMakeContextCurrent(window);

	// Initialize GLEW
	glewExperimental = true; // Needed for core profile
	if (glewInit() != GLEW_OK) {
		cerr << "Failed to create GLEW" << endl;
		glfwTerminate();
		return -1;
	}

	// Black background
	glClearColor(0.0f, 0.0f, 25 / 225.0f, 1.0f);

	// Compile and link shaders here ...
	GLuint defaultShaderProgram = shader("../Source/COMP371-Group14-Project/modelShader.vs", "../Source/COMP371-Group14-Project/modelShader.fs");
	GLuint phongLightShaderProgram = shader("../Source/COMP371-Group14-Project/lightShader.vs", "../Source/COMP371-Group14-Project/lightShader.fs");
	GLuint shadowShaderProgram = shader("../Source/COMP371-Group14-Project/shadowShader.vs", "../Source/COMP371-Group14-Project/shadowShader.fs");

#pragma endregion windowSetUp

	// Camera parameters for view transform
	vec3 cameraLookAt(0.0f, 0.0f, 0.0f);
	vec3 cameraUp(0.0f, 1.0f, 0.0f);

	// Set projection matrix for shader, this won't change
	mat4 projectionMatrix = perspective(70.0f, // field of view in degrees
		(float)windowWidth / windowHeight,  // aspect ratio
		0.01f, 100.0f);   // near and far (near > 0)

		// Set initial view matrix
	mat4 viewMatrix = lookAt(cameraPosition,  // eye
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
	int planeVAO = createPlaneVertexArrayObject();

	//Create hierarchical models
	mat4 L9BaseTranslation = translate(mat4(1.0f), vec3(-halfGridSize, 2.5f, -halfGridSize));	//Model's start pos doesn't change
	Model* l9Model = makeL9Model(unitCubeVAO);
	
	mat4 I9BaseTranslation = translate(mat4(1.0f), vec3(halfGridSize -1, 2.5f, -halfGridSize));	//Model's start pos doesn't change
	Model* i9Model = makeI9Model(unitCubeVAO);

	mat4 U3BaseTranslation = translate(mat4(1.0f), vec3(0, 2.5f, 0));	//Model's start pos doesn't change
	Model* u3Model = makeU3Model(rainbowCubeVAO);

	//mat4 T9BaseTranslation = translate(mat4(1.0f), vec3(-halfGridSize, 2.5f, halfGridSize));	//Model's start pos doesn't change
	//Model* t9Model = makeT9Model(vao);

	//mat4 C4BaseTranslation = translate(mat4(1.0f), vec3(halfGridSize - 1, 2.5f, halfGridSize));	//Model's start pos doesn't change
	mat4 C4BaseTranslation = translate(mat4(1.0f), vec3(0.0f, 2.5f, 0.0f));	//Model's start pos doesn't change
	Model* c4Model = makeC4Model(vao);

	// For frame time
	float lastFrameTime = glfwGetTime();
	int lastMouseLeftState = GLFW_RELEASE;
	double lastMousePosX, lastMousePosY;
	glfwGetCursorPos(window, &lastMousePosX, &lastMousePosY);

	// Enable Backface culling and depth test
	glEnable(GL_CULL_FACE);
	glEnable(GL_DEPTH_TEST);

	glPointSize(3.0f);

	//Shadow setup
	GLuint shadowMap;
	GLuint shadowMapBuffer = createShadowMapBuffer(shadowMap);

	// Entering Main Loop
	while (!glfwWindowShouldClose(window))
	{
		// Frame time calculation
		float dt = glfwGetTime() - lastFrameTime;
		lastFrameTime += dt;

		// Model Matrices - they control the transformations of the letters model
		modelScalingMatrix = scale(mat4(1.0f), vec3(1.0f, 1.0f, 1.0f) * modelScaleFactor);
		modelRotationMatrix = rotate(mat4(1.0f), radians(modelYRotationAngle), vec3(0.0f, 1.0f, 0.0f)) * rotate(mat4(1.0f), radians(modelXRotationAngle), vec3(1.0f, 0.0f, 0.0f));
		modelTranslationMatrix = translate(mat4(1.0f), modelPosition);
		sharedModelMatrix = modelTranslationMatrix * modelScalingMatrix * modelRotationMatrix;


		//bind and clear the shadow buffer, set viewport to shadowMap dimensions
		glViewport(0, 0, depthWidth, depthHeight);
		glBindFramebuffer(GL_FRAMEBUFFER, shadowMapBuffer);
		glClear(GL_DEPTH_BUFFER_BIT);

		//use the shadow shader, draw all objects
		useShadowShader(shadowShaderProgram);

		//Draw plane and L9 for the shadow map
		glBindVertexArray(planeVAO);
		mat4 planeStretcher = worldOrientationModelMatrix * translate(mat4(1.0f), vec3(0.0f, 0.1f, 0.0f)) * scale(mat4(1.0f), vec3(50.0f, 1.0f, 50.0f));
		glUniformMatrix4fv(worldMatrixLocation, 1, GL_FALSE, &planeStretcher[0][0]);
		glDrawArrays(renderingMode, 0, 6);

		mat4 L9Matrix = worldOrientationModelMatrix * L9BaseTranslation * sharedModelMatrix;
		l9Model->draw(L9Matrix, renderingMode, worldMatrixLocation);


		//bind and clear the default (screen) framebuffer
		glBindFramebuffer(GL_FRAMEBUFFER, 0);
		glViewport(0, 0, windowWidth, windowHeight);

		// Each frame, reset color of each pixel to glClearColor
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		useLightingShader(phongLightShaderProgram, projectionMatrix, viewMatrix);
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, shadowMap);

		
		//Draw plane
		glBindVertexArray(planeVAO);
		glUniformMatrix4fv(worldMatrixLocation, 1, GL_FALSE, &planeStretcher[0][0]);
		glDrawArrays(renderingMode, 0, 6);

		//Draw L9
		L9Matrix = worldOrientationModelMatrix * L9BaseTranslation * sharedModelMatrix;
		l9Model->draw(L9Matrix, renderingMode, worldMatrixLocation);
		
		//Draw I9
		mat4 I9Matrix = worldOrientationModelMatrix * I9BaseTranslation * sharedModelMatrix;
		i9Model->draw(I9Matrix, renderingMode, worldMatrixLocation);

		//Draw U3
		mat4 U3Matrix = worldOrientationModelMatrix * U3BaseTranslation * sharedModelMatrix;
		u3Model->draw(U3Matrix, renderingMode, worldMatrixLocation);

		useStandardShader(defaultShaderProgram, projectionMatrix, viewMatrix);

#pragma region Grid and Coordinate Axis
		// Draw ground using Hierarchical Modeling
		glBindVertexArray(gridVAO);

		// Initialize variables for grid size
		mat4 GridX = worldOrientationModelMatrix * translate(mat4(1.0f), vec3(-1.0f * gridSize / 2.0f, 0.0f, -1.0f * gridSize / 2.0f));
		glUniformMatrix4fv(worldMatrixLocation, 1, GL_FALSE, &GridX[0][0]);
		glDrawArrays(GL_LINES, 0, 2 * (gridSize * 2));


		// Set up Coordinate Axis Matrix using Hierarchical Modeling
		mat4 Coordinates = worldOrientationModelMatrix * translate(mat4(1.0f), vec3(0.0f, 0.01f, 0.01f));

		int numLines = 3;
		glBindVertexArray(xyzVAO);

		glUniformMatrix4fv(worldMatrixLocation, 1, GL_FALSE, &Coordinates[0][0]);
		glLineWidth(5.0f);
		glDrawArrays(GL_LINES, 0, 2 * numLines);

		glLineWidth(1.0f);
#pragma endregion

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

		cameraVerticalAngle = glm::max(-85.0f, glm::min(85.0f, cameraVerticalAngle));
		if (cameraHorizontalAngle > 360)
		{
			cameraHorizontalAngle -= 360;
		}
		else if (cameraHorizontalAngle < -360)
		{
			cameraHorizontalAngle += 360;
		}

		float theta = radians(cameraHorizontalAngle);
		float phi = radians(cameraVerticalAngle);

		cameraLookAt = vec3(cosf(phi) * cosf(theta), sinf(phi), -cosf(phi) * sinf(theta));
		vec3 cameraSideVector = cross(cameraLookAt, vec3(0.0f, 1.0f, 0.0f));

		normalize(cameraSideVector);

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
		viewMatrix = scale(viewMatrix, vec3(magnificationFactor, magnificationFactor, magnificationFactor));

#pragma endregion

	}

	// Shutdown GLFW
	glfwTerminate();

	return 0;
}

