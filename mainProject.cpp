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
	// Cube model (position, colors)
	glm::vec3 vertexArray[] = {
		glm::vec3(-0.5f,-0.5f,-0.5f), glm::vec3(0.7f, 0.7f, 0.7f), //left - grey
		glm::vec3(-0.5f,-0.5f, 0.5f), glm::vec3(0.7f, 0.7f, 0.7f),
		glm::vec3(-0.5f, 0.5f, 0.5f), glm::vec3(0.7f, 0.7f, 0.7f),

		glm::vec3(-0.5f,-0.5f,-0.5f), glm::vec3(0.7f, 0.7f, 0.7f),
		glm::vec3(-0.5f, 0.5f, 0.5f), glm::vec3(0.7f, 0.7f, 0.7f),
		glm::vec3(-0.5f, 0.5f,-0.5f), glm::vec3(0.7f, 0.7f, 0.7f),

		glm::vec3(0.5f, 0.5f,-0.5f), glm::vec3(1.0f, 1.0f, 1.0f), // far - white
		glm::vec3(-0.5f,-0.5f,-0.5f), glm::vec3(1.0f, 1.0f, 1.0f),
		glm::vec3(-0.5f, 0.5f,-0.5f), glm::vec3(1.0f, 1.0f, 1.0f),

		glm::vec3(0.5f, 0.5f,-0.5f), glm::vec3(1.0f, 1.0f, 1.0f),
		glm::vec3(0.5f,-0.5f,-0.5f), glm::vec3(1.0f, 1.0f, 1.0f),
		glm::vec3(-0.5f,-0.5f,-0.5f), glm::vec3(1.0f, 1.0f, 1.0f),

		glm::vec3(0.5f,-0.5f, 0.5f), glm::vec3(0.7f, 0.7f, 0.7f), // bottom - grey
		glm::vec3(-0.5f,-0.5f,-0.5f), glm::vec3(0.7f, 0.7f, 0.7f),
		glm::vec3(0.5f,-0.5f,-0.5f), glm::vec3(0.7f, 0.7f, 0.7f),

		glm::vec3(0.5f,-0.5f, 0.5f), glm::vec3(0.7f, 0.7f, 0.7f),
		glm::vec3(-0.5f,-0.5f, 0.5f), glm::vec3(0.7f, 0.7f, 0.7f),
		glm::vec3(-0.5f,-0.5f,-0.5f), glm::vec3(0.7f, 0.7f, 0.7f),

		glm::vec3(-0.5f, 0.5f, 0.5f), glm::vec3(1.0f, 1.0f, 1.0f), // near - white
		glm::vec3(-0.5f,-0.5f, 0.5f), glm::vec3(1.0f, 1.0f, 1.0f),
		glm::vec3(0.5f,-0.5f, 0.5f), glm::vec3(1.0f, 1.0f, 1.0f),

		glm::vec3(0.5f, 0.5f, 0.5f), glm::vec3(1.0f, 1.0f, 1.0f),
		glm::vec3(-0.5f, 0.5f, 0.5f), glm::vec3(1.0f, 1.0f, 1.0f),
		glm::vec3(0.5f,-0.5f, 0.5f), glm::vec3(1.0f, 1.0f, 1.0f),

		glm::vec3(0.5f, 0.5f, 0.5f), glm::vec3(0.7f, 0.7f, 0.7f), // right - grey
		glm::vec3(0.5f,-0.5f,-0.5f), glm::vec3(0.7f, 0.7f, 0.7f),
		glm::vec3(0.5f, 0.5f,-0.5f), glm::vec3(0.7f, 0.7f, 0.7f),

		glm::vec3(0.5f,-0.5f,-0.5f), glm::vec3(0.7f, 0.7f, 0.7f),
		glm::vec3(0.5f, 0.5f, 0.5f), glm::vec3(0.7f, 0.7f, 0.7f),
		glm::vec3(0.5f,-0.5f, 0.5f), glm::vec3(0.7f, 0.7f, 0.7f),

		glm::vec3(0.5f, 0.5f, 0.5f), glm::vec3(0.7f, 0.7f, 0.7f), // top - grey
		glm::vec3(0.5f, 0.5f,-0.5f), glm::vec3(0.7f, 0.7f, 0.7f),
		glm::vec3(-0.5f, 0.5f,-0.5f), glm::vec3(0.7f, 0.7f, 0.7f),

		glm::vec3(0.5f, 0.5f, 0.5f), glm::vec3(0.7f, 0.7f, 0.7f),
		glm::vec3(-0.5f, 0.5f,-0.5f), glm::vec3(0.7f, 0.7f, 0.7f),
		glm::vec3(-0.5f, 0.5f, 0.5f), glm::vec3(0.7f, 0.7f, 0.7f)
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

/* declaring individual CHAR matrices values to allow for individual movement */

// L9 PRESETS
float l9ModelXRotationAngle = 0.0f;
float l9ModelYRotationAngle = 0.0f;
float l9ModelScaleFactor = 1.0f;
mat4 l9ModelScalingMatrix = mat4(1.0f);
mat4 l9ModelRotationMatrix;
mat4 l9ModelTranslationMatrix;
mat4 l9ModelMatrix;
vec3 l9ModelPosition = vec3(0.0f, 1.0f, 0.0f);

// T9 PRESETS
float t9ModelXRotationAngle = 0.0f;
float t9ModelYRotationAngle = 0.0f;
float t9ModelScaleFactor = 1.0f;
mat4 t9ModelScalingMatrix = mat4(1.0f);
mat4 t9ModelRotationMatrix;
mat4 t9ModelTranslationMatrix;
mat4 t9ModelMatrix;
vec3 t9ModelPosition = vec3(0.0f, 1.0f, 0.0f);

// E PRESETS
float u3ModelXRotationAngle = 0.0f;
float u3ModelYRotationAngle = 0.0f;
float u3ModelScaleFactor = 1.0f;
mat4 u3ModelScalingMatrix = mat4(1.0f);
mat4 u3ModelRotationMatrix;
mat4 u3ModelTranslationMatrix;
mat4 u3ModelMatrix;
vec3 u3ModelPosition = vec3(0.0f, 1.0f, 0.0f);

// I9 PRESETS
float i9ModelXRotationAngle = 0.0f;
float i9ModelYRotationAngle = 0.0f;
float i9ModelScaleFactor = 1.0f;
mat4 i9ModelScalingMatrix = mat4(1.0f);
mat4 i9ModelRotationMatrix;
mat4 i9ModelTranslationMatrix;
mat4 i9ModelMatrix;
vec3 i9ModelPosition = vec3(0.0f, 1.0f, 0.0f);

// C4 PRESETS
float c4ModelXRotationAngle = 0.0f;
float c4ModelYRotationAngle = 0.0f;
float c4ModelScaleFactor = 1.0f;
mat4 c4ModelScalingMatrix = mat4(1.0f);
mat4 c4ModelRotationMatrix;
mat4 c4ModelTranslationMatrix;
mat4 c4ModelMatrix;
vec3 c4ModelPosition = vec3(0.0f, 1.0f, 0.0f);

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
		l9ModelScaleFactor = 1.0f;
		t9ModelScaleFactor = 1.0f;
		u3ModelScaleFactor = 1.0f;
		i9ModelScaleFactor = 1.0f;
		c4ModelScaleFactor = 1.0f;
		modelPosition = vec3(0.0f);
		l9ModelPosition = vec3(0.0f);
		t9ModelPosition = vec3(0.0f);
		u3ModelPosition = vec3(0.0f);
		i9ModelPosition = vec3(0.0f);
		c4ModelPosition = vec3(0.0f);
		modelXRotationAngle = 0.0f;
		modelYRotationAngle = 0.0f;
		l9ModelYRotationAngle = 0.0f;
		t9ModelYRotationAngle = 0.0f;
		u3ModelYRotationAngle = 0.0f;
		i9ModelYRotationAngle = 0.0f;
		c4ModelYRotationAngle = 0.0f;

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

	/* INDIVIDUAL MOVEMENT CONTROLS */

	if ((glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_RELEASE && glfwGetKey(window, GLFW_KEY_RIGHT_SHIFT) == GLFW_RELEASE)
		&& glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
	{
		if (glfwGetKey(window, GLFW_KEY_1) == GLFW_PRESS)
		{
			l9ModelYRotationAngle += rotationSpeed * dt;
		}
		else if (glfwGetKey(window, GLFW_KEY_2) == GLFW_PRESS)
		{
			t9ModelYRotationAngle += rotationSpeed * dt;
		}
		else if (glfwGetKey(window, GLFW_KEY_3) == GLFW_PRESS)
		{
			u3ModelYRotationAngle += rotationSpeed * dt;
		}
		else if (glfwGetKey(window, GLFW_KEY_4) == GLFW_PRESS)
		{
			i9ModelYRotationAngle += rotationSpeed * dt;
		}
		else if (glfwGetKey(window, GLFW_KEY_5) == GLFW_PRESS)
		{
			c4ModelYRotationAngle += rotationSpeed * dt;
		}
		else {
			modelYRotationAngle += rotationSpeed * dt;
		}

	}

	if ((glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_RELEASE && glfwGetKey(window, GLFW_KEY_RIGHT_SHIFT) == GLFW_RELEASE)
		&& glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
	{
		if (glfwGetKey(window, GLFW_KEY_1) == GLFW_PRESS)
		{
			l9ModelYRotationAngle -= rotationSpeed * dt;
		}
		else if (glfwGetKey(window, GLFW_KEY_2) == GLFW_PRESS)
		{
			t9ModelYRotationAngle -= rotationSpeed * dt;
		}
		else if (glfwGetKey(window, GLFW_KEY_3) == GLFW_PRESS)
		{
			u3ModelYRotationAngle -= rotationSpeed * dt;
		}
		else if (glfwGetKey(window, GLFW_KEY_4) == GLFW_PRESS)
		{
			i9ModelYRotationAngle -= rotationSpeed * dt;
		}
		else if (glfwGetKey(window, GLFW_KEY_5) == GLFW_PRESS)
		{
			c4ModelYRotationAngle -= rotationSpeed * dt;
		}
		else {
			modelYRotationAngle -= rotationSpeed * dt;
		}
	}

	// INDIVICDUAL CHARACTER SCALE CONTROLS

	/* Simultaneously pressing U + (1 OR 2 OR 3 OR 4 OR 5) will scale UP the chosen character*/

	if (glfwGetKey(window, GLFW_KEY_U) == GLFW_PRESS)
	{
		if (glfwGetKey(window, GLFW_KEY_1) == GLFW_PRESS)
		{
			l9ModelScaleFactor += scaleSpeed * dt;
		}
		else if (glfwGetKey(window, GLFW_KEY_2) == GLFW_PRESS)
		{
			t9ModelScaleFactor += scaleSpeed * dt;
		}
		else if (glfwGetKey(window, GLFW_KEY_3) == GLFW_PRESS)
		{
			u3ModelScaleFactor += scaleSpeed * dt;
		}
		else if (glfwGetKey(window, GLFW_KEY_4) == GLFW_PRESS)
		{
			i9ModelScaleFactor += scaleSpeed * dt;
		}
		else if (glfwGetKey(window, GLFW_KEY_5) == GLFW_PRESS)
		{
			c4ModelScaleFactor += scaleSpeed * dt;
		}
		else {
			modelScaleFactor += scaleSpeed * dt;
		}

	}

	/* Simultaneously pressing J + (1 OR 2 OR 3 OR 4 OR 5) will scale DOWN the chosen character*/
	if (glfwGetKey(window, GLFW_KEY_J) == GLFW_PRESS)
	{
		if (glfwGetKey(window, GLFW_KEY_1) == GLFW_PRESS)
		{
			l9ModelScaleFactor -= scaleSpeed * dt;
		}
		else if (glfwGetKey(window, GLFW_KEY_2) == GLFW_PRESS)
		{
			t9ModelScaleFactor -= scaleSpeed * dt;
		}
		else if (glfwGetKey(window, GLFW_KEY_3) == GLFW_PRESS)
		{
			u3ModelScaleFactor -= scaleSpeed * dt;
		}
		else if (glfwGetKey(window, GLFW_KEY_4) == GLFW_PRESS)
		{
			i9ModelScaleFactor -= scaleSpeed * dt;
		}
		else if (glfwGetKey(window, GLFW_KEY_5) == GLFW_PRESS)
		{
			c4ModelScaleFactor -= scaleSpeed * dt;
		}
		else {
			modelScaleFactor -= scaleSpeed * dt;
		}

	}

	/* Simultaneously pressing T + (1 OR 2 OR 3 OR 4 OR 5) will translate the chosen character on Y*/
	if ((glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS || glfwGetKey(window, GLFW_KEY_RIGHT_SHIFT) == GLFW_PRESS)
		&& glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) // move forwards
	{
		if (glfwGetKey(window, GLFW_KEY_1) == GLFW_PRESS)
		{
			l9ModelPosition += vec3(0.0f, 1.0f, 0.0f) * moveSpeed * dt;
		}
		else if (glfwGetKey(window, GLFW_KEY_2) == GLFW_PRESS)
		{
			t9ModelPosition += vec3(0.0f, 1.0f, 0.0f) * moveSpeed * dt;
		}
		else if (glfwGetKey(window, GLFW_KEY_3) == GLFW_PRESS)
		{
			u3ModelPosition += vec3(0.0f, 1.0f, 0.0f) * moveSpeed * dt;
		}
		else if (glfwGetKey(window, GLFW_KEY_4) == GLFW_PRESS)
		{
			i9ModelPosition += vec3(0.0f, 1.0f, 0.0f) * moveSpeed * dt;
		}
		else if (glfwGetKey(window, GLFW_KEY_5) == GLFW_PRESS)
		{
			c4ModelPosition += vec3(0.0f, 1.0f, 0.0f) * moveSpeed * dt;
		}
		else {
			modelPosition += vec3(0.0f, 1.0f, 0.0f) * moveSpeed * dt;
		}

	}

	/* Simultaneously pressing S + (1 OR 2 OR 3 OR 4 OR 5 OR 6 ) will translate the chosen character on Y*/
	if ((glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS || glfwGetKey(window, GLFW_KEY_RIGHT_SHIFT) == GLFW_PRESS)
		&& glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) // move backwards
	{
		if (glfwGetKey(window, GLFW_KEY_1) == GLFW_PRESS)
		{
			l9ModelPosition += vec3(0.0f, -1.0f, 0.0f) * moveSpeed * dt;
		}
		else if (glfwGetKey(window, GLFW_KEY_2) == GLFW_PRESS)
		{
			t9ModelPosition += vec3(0.0f, -1.0f, 0.0f) * moveSpeed * dt;
		}
		else if (glfwGetKey(window, GLFW_KEY_3) == GLFW_PRESS)
		{
			u3ModelPosition += vec3(0.0f, -1.0f, 0.0f) * moveSpeed * dt;
		}
		else if (glfwGetKey(window, GLFW_KEY_4) == GLFW_PRESS)
		{
			i9ModelPosition += vec3(0.0f, -1.0f, 0.0f) * moveSpeed * dt;
		}
		else if (glfwGetKey(window, GLFW_KEY_5) == GLFW_PRESS)
		{
			c4ModelPosition += vec3(0.0f, -1.0f, 0.0f) * moveSpeed * dt;
		}
		else {
			modelPosition += vec3(0.0f, -1.0f, 0.0f) * moveSpeed * dt;
		}

	}

	/* Simultaneously pressing SPACE + (1 OR 2 OR 3 OR 4 OR 5) will change chars location to a random spot*/
	if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS)
	{
		if (glfwGetKey(window, GLFW_KEY_1) == GLFW_PRESS)
		{
			float randl9_x = rand() % (90 - 0 + 1) + 0;
			float randl9_z = rand() % (90 - 0 + 1) + 0;
			l9ModelPosition = vec3(randl9_x, 1.0f, randl9_z);
		}
		else if (glfwGetKey(window, GLFW_KEY_2) == GLFW_PRESS)
		{
			float randt9_x = rand() % (90 - 0 + 1) + 0;
			float randt9_z = rand() % (90 - 0 + 1) + 0;
			t9ModelPosition = vec3(randt9_x, 1.0f, -randt9_z);
		}
		else if (glfwGetKey(window, GLFW_KEY_3) == GLFW_PRESS)
		{
			float randu3_x = rand() % (40 - (-40) + 1) + (-40);
			float randu3_z = rand() % (40 - (-40) + 1) + (-40);
			u3ModelPosition = vec3(randu3_x, 1.0f, randu3_z);
		}
		else if (glfwGetKey(window, GLFW_KEY_4) == GLFW_PRESS)
		{
			float randi9_x = rand() % (90 - 0 + 1) + 0;
			float randi9_z = rand() % (90 - 0 + 1) + 0;
			i9ModelPosition = vec3(-randi9_x, 1.0f, randi9_z);
		}
		else if (glfwGetKey(window, GLFW_KEY_5) == GLFW_PRESS)
		{
			float randc4_x = rand() % (90 - 0 + 1) + 0;
			float randc4_z = rand() % (90 - 0 + 1) + 0;
			c4ModelPosition = vec3(-randc4_x, 1.0f, -randc4_z);
		}
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
			primitiveTranslationMatrix = translate(mat4(1.0f), vec3(-3.5f + (index * 4.0f) + (i % 3) * 1.0f, 1.5 + (ceil(i / 3) * -1.0f), -0.5f));
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

Model* makeI9Model(int vao) {
	// Draw I9 using hierarchical modeling, start at the lowest model(s) in the hierarchy
	glm::mat4 setUpScaling = mat4(1.0f);
	glm::mat4 setUpRotation = mat4(1.0f);
	glm::mat4 setUpTranslation = mat4(1.0f);

	// Creating top-part of the letter I
	setUpTranslation = translate(glm::mat4(1.0f), glm::vec3(-3.0f, 4.5f, 0.0f));
	setUpScaling = scale(glm::mat4(1.0f), glm::vec3(4.0f, 1.0f, 1.0f));
	Model* modelItopBar = new Model(vao, numVerticesPerCube, vector<Model*>(), setUpTranslation, mat4(1.0f), setUpScaling);


	// Creating middlet-part of the letter I
	setUpScaling = scale(glm::mat4(1.0f), glm::vec3(1.0f, 3.0f, 1.0f));
	setUpTranslation = translate(glm::mat4(1.0f), glm::vec3(-3.0f, 2.5f, 0.0f));
	Model* modelImiddleBar = new Model(vao, numVerticesPerCube, vector<Model*>(), setUpTranslation, mat4(1.0f), setUpScaling);

	// Creating bottom-part of the letter I
	setUpScaling = scale(glm::mat4(1.0f), glm::vec3(4.0f, 1.0f, 1.0f));
	setUpTranslation = translate(glm::mat4(1.0f), glm::vec3(-3.0f, 0.5f, 0.0f));
	Model* modelIbottomBar = new Model(vao, numVerticesPerCube, vector<Model*>(), setUpTranslation, mat4(1.0f), setUpScaling);

	// Setting up the letter I
	vector<Model*> IChildren = vector<Model*>();
	IChildren.push_back(modelItopBar);
	IChildren.push_back(modelImiddleBar);
	IChildren.push_back(modelIbottomBar);
	//The pieces of the I are placed such that the entire I is centered at origin on all axes
	//We can then very simply manipulate this modelI to transform the entire I
	//for example, to scoot the I left to make room for the number, making the entire I9 centered.
	setUpTranslation = translate(glm::mat4(1.0f), glm::vec3(-2.0f, -2.0f, 0.0f));
	Model* modelI = new Model(vao, 0, IChildren, setUpTranslation, mat4(1.0f), mat4(1.0f));


	// Creating top-part of the number 9
	setUpTranslation = translate(glm::mat4(1.0f), glm::vec3(2.75f, 4.5f, 0.0f));
	setUpScaling = scale(glm::mat4(1.0f), glm::vec3(1.5f, 1.0f, 1.0f));
	Model* model9top = new Model(vao, numVerticesPerCube, vector<Model*>(), setUpTranslation, mat4(1.0f), setUpScaling);

	// Creating left-part of the number 9
	setUpTranslation = translate(glm::mat4(1.0f), glm::vec3(1.5f, 3.25f, 0.0f));
	setUpScaling = scale(glm::mat4(1.0f), glm::vec3(1.0f, 3.5f, 1.0f));
	Model* model9left = new Model(vao, numVerticesPerCube, vector<Model*>(), setUpTranslation, mat4(1.0f), setUpScaling);

	// Creating bottom-part of the number 9
	setUpTranslation = translate(glm::mat4(1.0f), glm::vec3(2.75f, 2.0f, 0.0f));
	setUpScaling = scale(glm::mat4(1.0f), glm::vec3(2.0f, 1.0f, 1.0f));
	Model* model9bottom = new Model(vao, numVerticesPerCube, vector<Model*>(), setUpTranslation, mat4(1.0f), setUpScaling);

	// Creating right-part of the number 9
	setUpTranslation = translate(glm::mat4(1.0f), glm::vec3(4.0f, 2.51f, 0.0f));
	setUpScaling = scale(glm::mat4(1.0f), glm::vec3(1.0f, 5.0f, 1.0f));
	Model* model9right = new Model(vao, numVerticesPerCube, vector<Model*>(), setUpTranslation, mat4(1.0f), setUpScaling);


	// Setting up the number 9
	vector<Model*> nineChildren = vector<Model*>();
	nineChildren.push_back(model9top);
	nineChildren.push_back(model9right);
	nineChildren.push_back(model9left);
	nineChildren.push_back(model9bottom);
	setUpTranslation = translate(glm::mat4(1.0f), glm::vec3(-2.0f, -2.0f, 0.0f));
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
	glm::mat4 setUpScaling = mat4(1.0f);
	glm::mat4 setUpRotation = mat4(1.0f);
	glm::mat4 setUpTranslation = mat4(1.0f);

	// Creating left-part of the letter U
	setUpTranslation = translate(glm::mat4(1.0f), glm::vec3(-3.0f, 0.0f, 0.0f));
	setUpScaling = scale(glm::mat4(1.0f), glm::vec3(1.0f, 5.0f, 1.0f));
	Model* modelUleftBar = new Model(vao, numVerticesPerCube, vector<Model*>(), setUpTranslation, mat4(1.0f), setUpScaling);


	// Creating bottom-part of the letter U
	setUpScaling = scale(glm::mat4(1.0f), glm::vec3(2.0f, 1.0f, 1.0f));
	setUpTranslation = translate(glm::mat4(1.0f), glm::vec3(-1.5f, -2.0f, 0.0f));
	Model* modelUbottomBar = new Model(vao, numVerticesPerCube, vector<Model*>(), setUpTranslation, mat4(1.0f), setUpScaling);

	// Creating right-part of the letter U
	setUpScaling = scale(glm::mat4(1.0f), glm::vec3(1.0f, 5.0f, 1.0f));
	setUpTranslation = translate(glm::mat4(1.0f), glm::vec3(0.0f, 0.0f, 0.0f));
	Model* modelUrightBar = new Model(vao, numVerticesPerCube, vector<Model*>(), setUpTranslation, mat4(1.0f), setUpScaling);

	// Setting up the letter U
	vector<Model*> UChildren = vector<Model*>();
	UChildren.push_back(modelUleftBar);
	UChildren.push_back(modelUbottomBar);
	UChildren.push_back(modelUrightBar);
	//The pieces of the U are placed such that the entire U is centered at origin on all axes
	//We can then very simply manipulate this modelU to transform the entire U
	//for example, to scoot the U left to make room for the number, making the entire U3 centered.
	setUpTranslation = translate(glm::mat4(1.0f), glm::vec3(-2.0f, 0.0f, 0.0f));
	Model* modelU = new Model(vao, 0, UChildren, setUpTranslation, mat4(1.0f), mat4(1.0f));


	// Creating base-part of the number 3
	setUpTranslation = translate(glm::mat4(1.0f), glm::vec3(4.0f, 0.0f, 0.0f));
	setUpScaling = scale(glm::mat4(1.0f), glm::vec3(1.0f, 5.0f, 1.0f));
	Model* model3base = new Model(vao, numVerticesPerCube, vector<Model*>(), setUpTranslation, mat4(1.0f), setUpScaling);

	// Creating topArm-part of the number 3
	setUpTranslation = translate(glm::mat4(1.0f), glm::vec3(2.5f, 2.0f, 0.0f));
	setUpScaling = scale(glm::mat4(1.0f), glm::vec3(2.0f, 1.0f, 1.0f));
	Model* model3topArm = new Model(vao, numVerticesPerCube, vector<Model*>(), setUpTranslation, mat4(1.0f), setUpScaling);

	// Creating middleArm-part of the number 3
	setUpTranslation = translate(glm::mat4(1.0f), glm::vec3(2.5f, -2.0f, 0.0f));
	setUpScaling = scale(glm::mat4(1.0f), glm::vec3(2.0f, 1.0f, 1.0f));
	Model* model3middleArm = new Model(vao, numVerticesPerCube, vector<Model*>(), setUpTranslation, mat4(1.0f), setUpScaling);

	// Creating bottomArm-part of the number 3
	setUpTranslation = translate(glm::mat4(1.0f), glm::vec3(3.0f, 0.0f, 0.0f));
	setUpScaling = scale(glm::mat4(1.0f), glm::vec3(1.0f, 1.0f, 1.0f));
	Model* model3bottomArm = new Model(vao, numVerticesPerCube, vector<Model*>(), setUpTranslation, mat4(1.0f), setUpScaling);


	// Setting up the number 3
	vector<Model*> threeChildren = vector<Model*>();
	threeChildren.push_back(model3base);
	threeChildren.push_back(model3topArm);
	threeChildren.push_back(model3middleArm);
	threeChildren.push_back(model3bottomArm);
	setUpTranslation = translate(glm::mat4(1.0f), glm::vec3(0.0f, 0.0f, 0.0f));
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
	glm::mat4 setUpScaling = mat4(1.0f);
	glm::mat4 setUpRotation = mat4(1.0f);
	glm::mat4 setUpTranslation = mat4(1.0f);

	// Creating left-part of the letter C
	setUpTranslation = translate(glm::mat4(1.0f), glm::vec3(0.0f, 0.0f, 0.0f));
	setUpScaling = scale(glm::mat4(1.0f), glm::vec3(1.0f, 5.0f, 1.0f));
	Model* modelCleftBar = new Model(vao, numVerticesPerCube, vector<Model*>(), setUpTranslation, mat4(1.0f), setUpScaling);

	// Creating bottom-part of the letter C
	setUpScaling = scale(glm::mat4(1.0f), glm::vec3(3.0f, 1.0f, 1.0f));
	setUpTranslation = translate(glm::mat4(1.0f), glm::vec3(1.0f, 0.0f, 0.0f));
	Model* modelCbottomBar = new Model(vao, numVerticesPerCube, vector<Model*>(), setUpTranslation, mat4(1.0f), setUpScaling);

	// Creating top-part of the letter C
	setUpScaling = scale(glm::mat4(1.0f), glm::vec3(3.0f, 1.0f, 1.0f));
	setUpTranslation = translate(glm::mat4(1.0f), glm::vec3(1.0f, 4.0f, 0.0f));
	Model* modelCtopBar = new Model(vao, numVerticesPerCube, vector<Model*>(), setUpTranslation, mat4(1.0f), setUpScaling);

	// Setting up the letter C
	vector<Model*> CChildren = vector<Model*>();
	CChildren.push_back(modelCleftBar);
	CChildren.push_back(modelCbottomBar);
	CChildren.push_back(modelCtopBar);
	//The pieces of the C are placed such that the entire C is centered at origin on all axes
	//We can then very simply manipulate this modelC to transform the entire C
	//for example, to scoot the C left to make room for the number, making the entire C4 centered.
	setUpTranslation = translate(glm::mat4(1.0f), glm::vec3(-9.0f, 0.0f, 0.0f));
	Model* modelC = new Model(vao, 0, CChildren, setUpTranslation, mat4(1.0f), mat4(1.0f));


	// Creating right-part of the number 4
	setUpTranslation = translate(glm::mat4(1.0f), glm::vec3(0.0f, 0.0f, 0.0f));
	setUpScaling = scale(glm::mat4(1.0f), glm::vec3(1.0f, 5.0f, 1.0f));
	Model* model4right = new Model(vao, numVerticesPerCube, vector<Model*>(), setUpTranslation, mat4(1.0f), setUpScaling);

	// Creating middle-part of the number 4
	setUpTranslation = translate(glm::mat4(1.0f), glm::vec3(-3.0f, 2.0f, 0.0f));
	setUpScaling = scale(glm::mat4(1.0f), glm::vec3(3.0f, 1.0f, 1.0f));
	Model* model4middle = new Model(vao, numVerticesPerCube, vector<Model*>(), setUpTranslation, mat4(1.0f), setUpScaling);

	// Creating left-part of the number 4
	setUpTranslation = translate(glm::mat4(1.0f), glm::vec3(-3.0f, 3.0f, 0.0f));
	setUpScaling = scale(glm::mat4(1.0f), glm::vec3(1.0f, 2.0f, 1.0f));
	Model* model4left = new Model(vao, numVerticesPerCube, vector<Model*>(), setUpTranslation, mat4(1.0f), setUpScaling);

	// Setting up the number 4
	vector<Model*> fourChildren = vector<Model*>();
	fourChildren.push_back(model4left);
	fourChildren.push_back(model4middle);
	fourChildren.push_back(model4right);
	setUpTranslation = translate(glm::mat4(1.0f), glm::vec3(0.0f, 0.0f, 0.0f));
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
		std::cout << "error of some sort: " << errorValue << std::endl;
	}
}

int main(int argc, char* argv[])
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
	GLuint shaderProgram = shader("../Source/COMP371-Group14-Project/modelShader.vs", "../Source/COMP371-Group14-Project/modelShader.fs");

	// We can set the shader once, since we have only one
	glUseProgram(shaderProgram);
#pragma endregion windowSetUp

	// Camera parameters for view transform
	glm::vec3 cameraLookAt(0.0f, 0.0f, 0.0f);
	glm::vec3 cameraUp(0.0f, 1.0f, 0.0f);

	// Set projection matrix for shader, this won't change
	glm::mat4 projectionMatrix = glm::perspective(70.0f, // field of view in degrees
		1024.0f / 768.0f,  // aspect ratio
		0.01f, 100.0f);   // near and far (near > 0)

	GLuint projectionMatrixLocation = glGetUniformLocation(shaderProgram, "projectionMatrix");
	glUniformMatrix4fv(projectionMatrixLocation, 1, GL_FALSE, &projectionMatrix[0][0]);

	// Set initial view matrix
	glm::mat4 viewMatrix = lookAt(cameraPosition,  // eye
		cameraPosition + cameraLookAt,  // center
		cameraUp); // up

	GLuint viewMatrixLocation = glGetUniformLocation(shaderProgram, "viewMatrix");
	glUniformMatrix4fv(viewMatrixLocation, 1, GL_FALSE, &viewMatrix[0][0]);

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

	mat4 I9BaseTranslation = translate(glm::mat4(1.0f), glm::vec3(halfGridSize - 1, 2.5f, -halfGridSize));	//Model's start pos doesn't change
	Model* i9Model = makeI9Model(unitCubeVAO);

	mat4 U3BaseTranslation = translate(glm::mat4(1.0f), glm::vec3(0, 2.5f, 0));	//Model's start pos doesn't change
	Model* u3Model = makeU3Model(rainbowCubeVAO);

	//mat4 T9BaseTranslation = translate(glm::mat4(1.0f), glm::vec3(-halfGridSize, 2.5f, halfGridSize));	//Model's start pos doesn't change
	//Model* t9Model = makeT9Model(vao);

	mat4 C4BaseTranslation = translate(glm::mat4(1.0f), glm::vec3(halfGridSize - 1, 2.5f, halfGridSize));	//Model's start pos doesn't change
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

	// Entering Main Loop
	while (!glfwWindowShouldClose(window))
	{
		// Frame time calculation
		float dt = glfwGetTime() - lastFrameTime;
		lastFrameTime += dt;

		// Each frame, reset color of each pixel to glClearColor
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		GLuint worldMatrixLocation = glGetUniformLocation(shaderProgram, "worldMatrix");

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

		// Building L9 scalable/translatable/rotateable matrix for individual letter
		l9ModelScalingMatrix = scale(mat4(1.0f), vec3(1.0f, 1.0f, 1.0f) * l9ModelScaleFactor);
		l9ModelRotationMatrix = rotate(mat4(1.0f), radians(l9ModelYRotationAngle), vec3(0.0f, 1.0f, 0.0f)) * rotate(mat4(1.0f), radians(l9ModelXRotationAngle), vec3(1.0f, 0.0f, 0.0f));
		l9ModelTranslationMatrix = translate(mat4(1.0f), l9ModelPosition);
		l9ModelMatrix = l9ModelTranslationMatrix * l9ModelScalingMatrix * l9ModelRotationMatrix;

		//Draw L9
		mat4 L9Matrix = worldOrientationModelMatrix * L9BaseTranslation * sharedModelMatrix * l9ModelMatrix;
		l9Model->draw(L9Matrix, renderingMode, worldMatrixLocation);

		// Building I9 scalable/translatable/rotateable matrix for individual letter
		i9ModelScalingMatrix = scale(mat4(1.0f), vec3(1.0f, 1.0f, 1.0f) * i9ModelScaleFactor);
		i9ModelRotationMatrix = rotate(mat4(1.0f), radians(i9ModelYRotationAngle), vec3(0.0f, 1.0f, 0.0f)) * rotate(mat4(1.0f), radians(i9ModelXRotationAngle), vec3(1.0f, 0.0f, 0.0f));
		i9ModelTranslationMatrix = translate(mat4(1.0f), i9ModelPosition);
		i9ModelMatrix = i9ModelTranslationMatrix * i9ModelScalingMatrix * i9ModelRotationMatrix;

		//Draw I9
		mat4 I9Matrix = worldOrientationModelMatrix * I9BaseTranslation * sharedModelMatrix * i9ModelMatrix;
		i9Model->draw(I9Matrix, renderingMode, worldMatrixLocation);

		// Building U3 scalable/translatable/rotateable matrix for individual letter
		u3ModelScalingMatrix = scale(mat4(1.0f), vec3(1.0f, 1.0f, 1.0f) * u3ModelScaleFactor);
		u3ModelRotationMatrix = rotate(mat4(1.0f), radians(u3ModelYRotationAngle), vec3(0.0f, 1.0f, 0.0f)) * rotate(mat4(1.0f), radians(u3ModelXRotationAngle), vec3(1.0f, 0.0f, 0.0f));
		u3ModelTranslationMatrix = translate(mat4(1.0f), u3ModelPosition);
		u3ModelMatrix = u3ModelTranslationMatrix * u3ModelScalingMatrix * u3ModelRotationMatrix;

		//Draw U3
		mat4 U3Matrix = worldOrientationModelMatrix * U3BaseTranslation * sharedModelMatrix * u3ModelMatrix;
		u3Model->draw(U3Matrix, renderingMode, worldMatrixLocation);

		// Building U3 scalable/translatable/rotateable matrix for individual letter
		t9ModelScalingMatrix = scale(mat4(1.0f), vec3(1.0f, 1.0f, 1.0f) * t9ModelScaleFactor);
		t9ModelRotationMatrix = rotate(mat4(1.0f), radians(t9ModelYRotationAngle), vec3(0.0f, 1.0f, 0.0f)) * rotate(mat4(1.0f), radians(t9ModelXRotationAngle), vec3(1.0f, 0.0f, 0.0f));
		t9ModelTranslationMatrix = translate(mat4(1.0f), u3ModelPosition);
		t9ModelMatrix = t9ModelTranslationMatrix * t9ModelScalingMatrix * t9ModelRotationMatrix;

		//Draw T9
		//mat4 T9Matrix = worldOrientationModelMatrix * T9BaseTranslation * sharedModelMatrix * t9ModelMatrix;
		//t9Model->draw(T9Matrix, renderingMode, worldMatrixLocation);

		// Building C4 scalable/translatable/rotateable matrix for individual letter
		c4ModelScalingMatrix = scale(mat4(1.0f), vec3(1.0f, 1.0f, 1.0f) * c4ModelScaleFactor);
		c4ModelRotationMatrix = rotate(mat4(1.0f), radians(c4ModelYRotationAngle), vec3(0.0f, 1.0f, 0.0f)) * rotate(mat4(1.0f), radians(c4ModelXRotationAngle), vec3(1.0f, 0.0f, 0.0f));
		c4ModelTranslationMatrix = translate(mat4(1.0f), c4ModelPosition);
		c4ModelMatrix = c4ModelTranslationMatrix * c4ModelScalingMatrix * c4ModelRotationMatrix;

		//Draw C4
		mat4 C4Matrix = worldOrientationModelMatrix * C4BaseTranslation * sharedModelMatrix * c4ModelMatrix;
		c4Model->draw(C4Matrix, renderingMode, worldMatrixLocation);

#pragma region T9
		// Draw geometry
		glBindVertexArray(vao);

		mat4 translateAAModel = worldOrientationModelMatrix * translate(mat4(1.0f), vec3(-halfGridSize, 2.5f, halfGridSize));

		drawLetter('T', 0, translateAAModel * sharedModelMatrix, worldMatrixLocation);
		drawLetter('9', 1, translateAAModel * sharedModelMatrix, worldMatrixLocation);
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

		glUniformMatrix4fv(projectionMatrixLocation, 1, GL_FALSE, &projectionMatrix[0][0]);

		// Update viewMatrix
		viewMatrix = lookAt(cameraPosition, cameraPosition + cameraLookAt, cameraUp);
		viewMatrix = glm::scale(viewMatrix, vec3(magnificationFactor, magnificationFactor, magnificationFactor));

		glUniformMatrix4fv(viewMatrixLocation, 1, GL_FALSE, &viewMatrix[0][0]);

		/* END Part 2 - SIMULTANEOUS MOUSE AND KEY movement */
#pragma endregion

	}

	// Shutdown GLFW
	glfwTerminate();

	return 0;
}

