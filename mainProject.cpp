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
#include "texture.h"
#include <time.h>
#include <algorithm>
#include <list>
#include <vector>
#include "OBJloader.h"  //For loading .obj files using a polygon list format


//define namespaces for glm and c++ std
using namespace glm;
using namespace std;

int windowWidth = 1024, windowHeight = 764;
const float shadowMapWidth = 2048, shadowMapHeight = 2048;

GLuint brickTexture, woodTexture, metalTexture, boxTexture, floorTilesTexture;

Material brick, wood, metal, box, floorTiles;
vec3 getShearMovement(float shearRotationAngle);

#pragma region unitCubes
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

int createTextureCubeVertexArrayObject() {

	// Cube model (position, colors, normals, texture coordinates)
	float vertexArray[] = {
		-0.5f, -0.5f, -0.5f,        1.0f, 1.0f, 1.0f,     -1.0f, 0.0f, 0.0f,     0.0f, 0.0f, //left 
		-0.5f,-0.5f, 0.5f,          1.0f, 1.0f, 1.0f,     -1.0f, 0.0f, 0.0f,     0.0f, 1.0f,
		-0.5f, 0.5f, 0.5f,          1.0f, 1.0f, 1.0f,     -1.0f, 0.0f, 0.0f,     1.0f, 1.0f,

		-0.5f,-0.5f,-0.5f,          1.0f, 1.0f, 1.0f,     -1.0f, 0.0f, 0.0f,     0.0f, 0.0f,
		-0.5f, 0.5f, 0.5f,          1.0f, 1.0f, 1.0f,     -1.0f, 0.0f, 0.0f,     1.0f, 1.0f,
		-0.5f, 0.5f,-0.5f,          1.0f, 1.0f, 1.0f,     -1.0f, 0.0f, 0.0f,     1.0f, 0.0f,

		0.5f, 0.5f,-0.5f,           1.0f, 1.0f, 1.0f,     0.0f, 0.0f, -1.0f,     1.0f, 1.0f, // far
		-0.5f,-0.5f,-0.5f,          1.0f, 1.0f, 1.0f,     0.0f, 0.0f, -1.0f,     0.0f, 0.0f,
		-0.5f, 0.5f,-0.5f,          1.0f, 1.0f, 1.0f,     0.0f, 0.0f, -1.0f,     0.0f, 1.0f,

		0.5f, 0.5f,-0.5f,           1.0f, 1.0f, 1.0f,     0.0f, 0.0f, -1.0f,     1.0f, 1.0f,
		0.5f,-0.5f,-0.5f,           1.0f, 1.0f, 1.0f,     0.0f, 0.0f, -1.0f,     1.0f, 0.0f,
		-0.5f,-0.5f,-0.5f,          1.0f, 1.0f, 1.0f,     0.0f, 0.0f, -1.0f,     0.0f, 0.0f,

		0.5f,-0.5f, 0.5f,           1.0f, 1.0f, 1.0f,     0.0f, -1.0f, 0.0f,     1.0f, 1.0f, // bottom 
		-0.5f,-0.5f,-0.5f,          1.0f, 1.0f, 1.0f,     0.0f, -1.0f, 0.0f,     0.0f, 0.0f,
		0.5f,-0.5f,-0.5f,           1.0f, 1.0f, 1.0f,     0.0f, -1.0f, 0.0f,     1.0f, 0.0f,

		0.5f,-0.5f, 0.5f,           1.0f, 1.0f, 1.0f,     0.0f, -1.0f, 0.0f,     1.0f, 1.0f,
		-0.5f,-0.5f, 0.5f,          1.0f, 1.0f, 1.0f,     0.0f, -1.0f, 0.0f,     0.0f, 1.0f,
		-0.5f,-0.5f,-0.5f,          1.0f, 1.0f, 1.0f,     0.0f, -1.0f, 0.0f,     0.0f, 0.0f,

		-0.5f, 0.5f, 0.5f,          1.0f, 1.0f, 1.0f,     0.0f, 0.0f, 1.0f,     0.0f, 1.0f, // near
		-0.5f,-0.5f, 0.5f,          1.0f, 1.0f, 1.0f,     0.0f, 0.0f, 1.0f,     0.0f, 0.0f,
		0.5f,-0.5f, 0.5f,           1.0f, 1.0f, 1.0f,     0.0f, 0.0f, 1.0f,     1.0f, 0.0f,

		0.5f, 0.5f, 0.5f,           1.0f, 1.0f, 1.0f,     0.0f, 0.0f, 1.0f,     1.0f, 1.0f,
		-0.5f, 0.5f, 0.5f,          1.0f, 1.0f, 1.0f,     0.0f, 0.0f, 1.0f,     0.0f, 1.0f,
		0.5f,-0.5f, 0.5f,           1.0f, 1.0f, 1.0f,     0.0f, 0.0f, 1.0f,     1.0f, 0.0f,

		0.5f, 0.5f, 0.5f,           1.0f, 1.0f, 1.0f,     1.0f, 0.0f, 0.0f,     1.0f, 1.0f, // right 
		0.5f,-0.5f,-0.5f,           1.0f, 1.0f, 1.0f,     1.0f, 0.0f, 0.0f,     0.0f, 0.0f,
		0.5f, 0.5f,-0.5f,           1.0f, 1.0f, 1.0f,     1.0f, 0.0f, 0.0f,     1.0f, 0.0f,

		0.5f,-0.5f,-0.5f,           1.0f, 1.0f, 1.0f,     1.0f, 0.0f, 0.0f,     0.0f, 0.0f,
		0.5f, 0.5f, 0.5f,           1.0f, 1.0f, 1.0f,     1.0f, 0.0f, 0.0f,     1.0f, 1.0f,
		0.5f,-0.5f, 0.5f,           1.0f, 1.0f, 1.0f,     1.0f, 0.0f, 0.0f,     0.0f, 1.0f,

		0.5f, 0.5f, 0.5f,           1.0f, 1.0f, 1.0f,     0.0f, 1.0f, 0.0f,     1.0f, 1.0f, // top 
		0.5f, 0.5f,-0.5f,           1.0f, 1.0f, 1.0f,     0.0f, 1.0f, 0.0f,     1.0f, 0.0f,
		-0.5f, 0.5f,-0.5f,          1.0f, 1.0f, 1.0f,     0.0f, 1.0f, 0.0f,     0.0f, 0.0f,

		0.5f, 0.5f, 0.5f,           1.0f, 1.0f, 1.0f,     0.0f, 1.0f, 0.0f,     1.0f, 1.0f,
		-0.5f, 0.5f,-0.5f,          1.0f, 1.0f, 1.0f,     0.0f, 1.0f, 0.0f,     0.0f, 0.0f,
		-0.5f, 0.5f, 0.5f,          1.0f, 1.0f, 1.0f,     0.0f, 1.0f, 0.0f,     0.0f, 1.0f
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
		11 * sizeof(float), // stride - each vertex contain 3 vec3 (position, color, normal)
		(void*)0             // array buffer offset
	);
	glEnableVertexAttribArray(0);


	glVertexAttribPointer(1,                            // attribute 1 matches aColor in Vertex Shader
		3,
		GL_FLOAT,
		GL_FALSE,
		11 * sizeof(float),
		(void*) (3 * sizeof(float))      // color is offseted a vec3 (comes after position)
	);
	glEnableVertexAttribArray(1);


	glVertexAttribPointer(2,                            // attribute 2 matches aNormal in Vertex Shader
		3,
		GL_FLOAT,
		GL_FALSE,
		11 * sizeof(float),
		(void*)(2 * 3 * sizeof(float))      // normal is offseted 2 vec3 (comes after position and color)
	);
	glEnableVertexAttribArray(2);

  glVertexAttribPointer(3,                            // attribute 3 matches aText in Vertex Shader
		2,
		GL_FLOAT,
		GL_FALSE,
		11 * sizeof(float),
		(void*)(3 * 3 * sizeof(float))      // texture is offseted 2 vec3 (comes after position and color)
	);
	glEnableVertexAttribArray(3);

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

int sphereVertices;

GLuint createSphereObjectVAO(string path, int& vertexCount) {
	std::vector<glm::vec3> vertices;
	std::vector<glm::vec3> colors;
	std::vector<glm::vec3> normals;
	std::vector<glm::vec2> UVs;

	//read the vertex data from the model's OBJ file
	loadOBJ(path.c_str(), vertices, colors, normals, UVs);

	GLuint VAO;
	glGenVertexArrays(1, &VAO);
	glBindVertexArray(VAO); //Becomes active VAO
							// Bind the Vertex Array Object first, then bind and set vertex buffer(s) and attribute pointer(s).

							//Vertex VBO setup
	GLuint vertices_VBO;
	glGenBuffers(1, &vertices_VBO);
	glBindBuffer(GL_ARRAY_BUFFER, vertices_VBO);
	glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(glm::vec3), &vertices.front(), GL_STATIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), (GLvoid*)0);
	glEnableVertexAttribArray(0);

	//Colors VBO setup
	GLuint colors_VBO;
	glGenBuffers(1, &colors_VBO);
	glBindBuffer(GL_ARRAY_BUFFER, colors_VBO);
	glBufferData(GL_ARRAY_BUFFER, colors.size() * sizeof(glm::vec3), &colors.front(), GL_STATIC_DRAW);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), (GLvoid*)0);
	glEnableVertexAttribArray(1);

	//Normals VBO setup
	GLuint normals_VBO;
	glGenBuffers(1, &normals_VBO);
	glBindBuffer(GL_ARRAY_BUFFER, normals_VBO);
	glBufferData(GL_ARRAY_BUFFER, normals.size() * sizeof(glm::vec3), &normals.front(), GL_STATIC_DRAW);
	glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), (GLvoid*)0);
	glEnableVertexAttribArray(2);

	//UVs VBO setup
	GLuint uvs_VBO;
	glGenBuffers(1, &uvs_VBO);
	glBindBuffer(GL_ARRAY_BUFFER, uvs_VBO);
	glBufferData(GL_ARRAY_BUFFER, UVs.size() * sizeof(glm::vec2), &UVs.front(), GL_STATIC_DRAW);
	glVertexAttribPointer(3, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(GLfloat), (GLvoid*)0);
	glEnableVertexAttribArray(3);

	glBindVertexArray(0); // Unbind VAO (it's always a good thing to unbind any buffer/array to prevent strange bugs, as we are using multiple VAOs)
	vertexCount = vertices.size();
	return VAO;
}

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

vec3 gridColor = vec3(0.25f, 0.25f, 0.25f);

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
				gridColor
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
				gridColor
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

int createGridSquareVertexArrayObject() {
	// Cube model (position, colors, normals, texture coordinates)
	float vertexArray[] = {
		-lineLength/2, -0.5f, -lineLength/2,        	1.0f, 1.0f, 1.0f,     0.0f, 1.0f, 0.0f,     0.0f, 0.0f, //left 
		-lineLength/2, -0.5f, lineLength/2,          1.0f, 1.0f, 1.0f,     0.0f, 1.0f, 0.0f,     0.0f, 8.0f,
		lineLength/2, -0.5f, lineLength/2,         	1.0f, 1.0f, 1.0f,     0.0f, 1.0f, 0.0f,     8.0f, 8.0f,

		-lineLength/2, -0.5f,-lineLength/2,          1.0f, 1.0f, 1.0f,     0.0f, 1.0f, 0.0f,     0.0f, 0.0f, //right
		lineLength/2, -0.5f, lineLength/2,          	1.0f, 1.0f, 1.0f,     0.0f, 1.0f, 0.0f,     8.0f, 8.0f,
		lineLength/2, -0.5f,-lineLength/2,          	1.0f, 1.0f, 1.0f,     0.0f, 1.0f, 0.0f,     8.0f, 0.0f,
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
		11 * sizeof(float), // stride - each vertex contain 3 vec3 (position, color, normal)
		(void*)0             // array buffer offset
	);
	glEnableVertexAttribArray(0);

	glVertexAttribPointer(1,                            // attribute 1 matches aColor in Vertex Shader
		3,
		GL_FLOAT,
		GL_FALSE,
		11 * sizeof(float),
		(void*) (3 * sizeof(float))      // color is offseted a vec3 (comes after position)
	);
	glEnableVertexAttribArray(1);

	glVertexAttribPointer(2,                            // attribute 2 matches aNormal in Vertex Shader
		3,
		GL_FLOAT,
		GL_FALSE,
		11 * sizeof(float),
		(void*)(2 * 3 * sizeof(float))      // normal is offseted 2 vec3 (comes after position and color)
	);
	glEnableVertexAttribArray(2);

  glVertexAttribPointer(3,                            // attribute 3 matches aText in Vertex Shader
		2,
		GL_FLOAT,
		GL_FALSE,
		11 * sizeof(float),
		(void*)(3 * 3 * sizeof(float))      // texture is offseted 2 vec3 (comes after position and color)
	);
	glEnableVertexAttribArray(3);

	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);

	return vertexArrayObject;
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
bool panMoveMode = false, angleMoveMode = false, zoomMoveMode = false, fastCam = false;

// Camera parameters for view transform
vec3 cameraLookAt(0.0f, 0.0f, 0.0f);
vec3 cameraUp(0.0f, 1.0f, 0.0f);
vec3 cameraPosition(0.0f, 15.0f, 30.0f);

// Set projection matrix for shader, this won't change
mat4 projectionMatrix = perspective(70.0f, // field of view in degrees
	(float)windowWidth / windowHeight,  // aspect ratio
	0.01f, 100.0f);   // near and far (near > 0)

	// Set initial view matrix
mat4 viewMatrix = lookAt(cameraPosition,  // eye
	cameraPosition + cameraLookAt,  // center
	cameraUp); // up

void handleCameraFlagInputs(GLFWwindow* window) {
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
bool renderShadows = true;
bool renderTextures = true;
static bool BPressed = false;
static bool MPressed = false;
static bool NPressed = false;
static bool XPressed = false;
void handleRenderingModeInput(GLFWwindow* window) {
	//----------------------------------------------------------------------------------
	//User can change the rendering mode
	if (glfwGetKey(window, GLFW_KEY_P) == GLFW_PRESS) //change to points
	{
		renderingMode = GL_POINTS;
	}

	if (glfwGetKey(window, GLFW_KEY_L) == GLFW_PRESS) //change to lines
	{
		renderingMode = GL_LINE_LOOP;
	}

	if (glfwGetKey(window, GLFW_KEY_T) == GLFW_PRESS) //change to trianges
	{
		renderingMode = GL_TRIANGLES;
	}

	if (glfwGetKey(window, GLFW_KEY_B) == GLFW_PRESS && BPressed == false) //toggle shadow rendering
	{
		renderShadows = !renderShadows;
		BPressed = true;
	}
	if (glfwGetKey(window, GLFW_KEY_B) == GLFW_RELEASE && BPressed == true) //toggle shadow rendering
	{
		BPressed = false;
	}

  	if (glfwGetKey(window, GLFW_KEY_X) == GLFW_PRESS && XPressed == false) //toggle shadow rendering
	{
		renderTextures = !renderTextures;
		XPressed = true;
	}
	if (glfwGetKey(window, GLFW_KEY_X) == GLFW_RELEASE && XPressed == true) //toggle shadow rendering
	{
		XPressed = false;
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
float modelShearFactor = 0.0f;
bool shearForward = true;
bool shearStepping = false;
bool shearSteppingBackward = false;
bool shearWalking = false;
int shearDirection = 0;

// Declaring model matrices
mat4 modelShearingMatrix = mat4(1.0f);
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


mat4 L9StartTranslation = translate(glm::mat4(1.0f), glm::vec3(-halfGridSize, 2.5f, -halfGridSize));
mat4 I9StartTranslation = translate(glm::mat4(1.0f), glm::vec3(halfGridSize - 1, 2.5f, -halfGridSize));
mat4 U3StartTranslation = translate(glm::mat4(1.0f), glm::vec3(0, 2.5f, 0));
mat4 C4StartTranslation = translate(glm::mat4(1.0f), glm::vec3(halfGridSize - 1, 2.5f, halfGridSize));
mat4 T9StartTranslation = translate(glm::mat4(1.0f), glm::vec3(-halfGridSize, 2.5f, halfGridSize));
mat4 L9BaseTranslation = L9StartTranslation;
mat4 I9BaseTranslation = I9StartTranslation;
mat4 U3BaseTranslation = U3StartTranslation;
mat4 T9BaseTranslation = T9StartTranslation;
mat4 C4BaseTranslation = C4StartTranslation;

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
		modelShearFactor = 0.0f;
		l9ModelYRotationAngle = 0.0f;
		t9ModelYRotationAngle = 0.0f;
		u3ModelYRotationAngle = 0.0f;
		i9ModelYRotationAngle = 0.0f;
		c4ModelYRotationAngle = 0.0f;
		l9ModelXRotationAngle = 0.0f;
		t9ModelXRotationAngle = 0.0f;
		u3ModelXRotationAngle = 0.0f;
		i9ModelXRotationAngle = 0.0f;
		c4ModelXRotationAngle = 0.0f;

		L9BaseTranslation = L9StartTranslation;
		I9BaseTranslation = I9StartTranslation;
		U3BaseTranslation = U3StartTranslation;
		T9BaseTranslation = T9StartTranslation;
		C4BaseTranslation = C4StartTranslation;
	}
	// Move/Shear model forward
	if (glfwGetKey(window, GLFW_KEY_M) == GLFW_PRESS && MPressed == false)
	{
		shearWalking = !shearWalking;
		shearDirection = 1;
		MPressed = true;
	}
	if (glfwGetKey(window, GLFW_KEY_M) == GLFW_RELEASE && MPressed == true)
	{
		MPressed = false;
	}

	//ONE STEP FORWARD
	if (glfwGetKey(window, GLFW_KEY_V) == GLFW_PRESS || shearStepping)
	{
		if (!shearStepping) {
			shearStepping = true;
			shearWalking = false;
			shearForward = true;
			modelShearFactor = 0.0f;
		}
		if (shearStepping) {
			float shearRotationAngle = (int)modelYRotationAngle % 360;

			modelPosition += getShearMovement(shearRotationAngle) * moveSpeed * dt;

			if (shearForward)
			{
				if (modelShearFactor < -1)
				{
					shearForward = false;
					modelShearFactor += 0.1;
				}
				else
				{
					modelShearFactor -= 0.1;
				}
				if (modelShearFactor > -0.1 && modelShearFactor < 0.1) {
					shearStepping = false;
				}
			}
			else
			{
				if (modelShearFactor > 1)
				{
					shearForward = true;
					modelShearFactor -= 0.1;
				}
				else {
					modelShearFactor += 0.1;
				}

			}
		}
	}

	//ONE STEP BACKWARD
	if (glfwGetKey(window, GLFW_KEY_C) == GLFW_PRESS || shearSteppingBackward)
	{
		if (!shearSteppingBackward) {
			shearSteppingBackward = true;
			shearWalking = false;
			shearForward = false;
			modelShearFactor = 0.0f;
		}
		if (shearSteppingBackward) {

			float shearRotationAngle = (int)modelYRotationAngle % 360;

			modelPosition -= getShearMovement(shearRotationAngle) * moveSpeed * dt;

			if (shearForward)
			{
				if (modelShearFactor < -1)
				{
					shearForward = false;
					modelShearFactor += 0.1;
				}
				else
				{
					modelShearFactor -= 0.1;
				}
			}
			else
			{
				if (modelShearFactor > 1)
				{
					shearForward = true;
					modelShearFactor -= 0.1;
				}
				else {
					modelShearFactor += 0.1;
				}
				if (modelShearFactor > -0.1 && modelShearFactor < 0.1) {
					shearSteppingBackward = false;
				}

			}
		}
	}

	// Move/Shear model backwards
	if (glfwGetKey(window, GLFW_KEY_N) == GLFW_PRESS && NPressed == false)
	{
		shearWalking = !shearWalking;
		shearDirection = -1;
		NPressed = true;
	}
	if (glfwGetKey(window, GLFW_KEY_N) == GLFW_RELEASE && NPressed == true)
	{
		NPressed = false;
	}

	/* INDIVIDUAL MOVEMENT CONTROLS */

	if ((glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_RELEASE && glfwGetKey(window, GLFW_KEY_RIGHT_SHIFT) == GLFW_RELEASE))
	{
		float directionOfChange = 0.0f;
		if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) {
			directionOfChange = -1.0f;
		}
		else if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) {
			directionOfChange = 1.0f;
		}
		if (glfwGetKey(window, GLFW_KEY_1) == GLFW_PRESS)
		{
			l9ModelXRotationAngle += rotationSpeed * dt * directionOfChange;
		}
		else if (glfwGetKey(window, GLFW_KEY_2) == GLFW_PRESS)
		{
			t9ModelXRotationAngle += rotationSpeed * dt * directionOfChange;
		}
		else if (glfwGetKey(window, GLFW_KEY_3) == GLFW_PRESS)
		{
			u3ModelXRotationAngle += rotationSpeed * dt * directionOfChange;
		}
		else if (glfwGetKey(window, GLFW_KEY_4) == GLFW_PRESS)
		{
			i9ModelXRotationAngle += rotationSpeed * dt * directionOfChange;
		}
		else if (glfwGetKey(window, GLFW_KEY_5) == GLFW_PRESS)
		{
			c4ModelXRotationAngle += rotationSpeed * dt * directionOfChange;
		}
		else {
			modelXRotationAngle += rotationSpeed * dt * directionOfChange;
		}

	}

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

	/* Simultaneously pressing SHIFT + (1 OR 2 OR 3 OR 4 OR 5) will translate the chosen character on Y*/
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


	if ((glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS && glfwGetKey(window, GLFW_KEY_RIGHT_SHIFT) == GLFW_PRESS))
	{
		float directionOfChange = 0.0f;
		if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS) {
			directionOfChange = -1.0f;
		}
		else if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS) {
			directionOfChange = 1.0f;
		}
		if (glfwGetKey(window, GLFW_KEY_1) == GLFW_PRESS)
		{
			l9ModelPosition += vec3(directionOfChange, 0.0f, 0.0f) * moveSpeed * dt;
		}
		else if (glfwGetKey(window, GLFW_KEY_2) == GLFW_PRESS)
		{
			t9ModelPosition += vec3(directionOfChange, 0.0f, 0.0f) * moveSpeed * dt;
		}
		else if (glfwGetKey(window, GLFW_KEY_3) == GLFW_PRESS)
		{
			u3ModelPosition += vec3(directionOfChange, 0.0f, 0.0f) * moveSpeed * dt;
		}
		else if (glfwGetKey(window, GLFW_KEY_4) == GLFW_PRESS)
		{
			i9ModelPosition += vec3(directionOfChange, 0.0f, 0.0f) * moveSpeed * dt;
		}
		else if (glfwGetKey(window, GLFW_KEY_5) == GLFW_PRESS)
		{
			c4ModelPosition += vec3(directionOfChange, 0.0f, 0.0f) * moveSpeed * dt;
		}
		else {
			modelPosition += vec3(directionOfChange, 0.0f, 0.0f) * moveSpeed * dt;
		}

	}

	/* Simultaneously pressing SPACE + (1 OR 2 OR 3 OR 4 OR 5) will change chars location to a random spot*/
	if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS)
	{
		float randl9_x = (rand() % (90 - 0 + 1) + 0);
		float randl9_z = (rand() % (90 - 0 + 1) + 0);
		mat4 newLocation = translate(mat4(1.0f), vec3(randl9_x - halfGridSize, 2.5f, randl9_z - halfGridSize));
		if (glfwGetKey(window, GLFW_KEY_1) == GLFW_PRESS)
		{
			l9ModelPosition = vec3(0.0f);
			L9BaseTranslation = newLocation;
		}
		else if (glfwGetKey(window, GLFW_KEY_2) == GLFW_PRESS)
		{
			t9ModelPosition = vec3(0.0f);
			T9BaseTranslation = newLocation;
		}
		else if (glfwGetKey(window, GLFW_KEY_3) == GLFW_PRESS)
		{
			u3ModelPosition = vec3(0.0f);
			U3BaseTranslation = newLocation;
		}
		else if (glfwGetKey(window, GLFW_KEY_4) == GLFW_PRESS)
		{
			i9ModelPosition = vec3(0.0f);
			I9BaseTranslation = newLocation;
		}
		else if (glfwGetKey(window, GLFW_KEY_5) == GLFW_PRESS)
		{
			c4ModelPosition = vec3(0.0f);
			C4BaseTranslation = newLocation;
		}
	}
}
#pragma endregion

#pragma region makeModels
unsigned int uboWorldMatrixBlock;
int numVerticesPerCube = 36;

Model* makeL9Model(int vao, int sphereVAO) {

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
	setUpTranslation = translate(glm::mat4(1.0f), glm::vec3(-1.0f, 1.5f, 0.0f));
	setUpScaling = scale(glm::mat4(1.0f), glm::vec3(1.0f, 2.5f, 1.0f));
	Model* modelLbottomBar = new Model(vao, numVerticesPerCube, uboWorldMatrixBlock, vector<Model*>(), setUpTranslation, mat4(1.0f), setUpScaling, box);

	// Setting up the letter L
	vector<Model*> LChildren = vector<Model*>();
	LChildren.push_back(modelLbottomBar);
	//The pieces of the L are placed such that the entire L is centered at origin on all axes
	//We can then very simply manipulate this modelL to transform the entire L
	//for example, to scoot the L left to make room for the number, making the entire L9 centered.
	setUpTranslation = translate(mat4(1.0f), vec3(-2.0f, 0.0f, 0.0f));
	Model* modelL = new Model(vao, 0, uboWorldMatrixBlock, LChildren, setUpTranslation, mat4(1.0f), mat4(1.0f), box);


	// Creating top-part of the number 9
	setUpTranslation = translate(mat4(1.0f), vec3(0.0f, 2.0f, 0.0f));
	setUpScaling = scale(mat4(1.0f), vec3(2.0f, 1.0f, 1.0f));
	Model* model9top = new Model(vao, numVerticesPerCube, uboWorldMatrixBlock, vector<Model*>(), setUpTranslation, mat4(1.0f), setUpScaling, metal);

	// Creating right-part of the number 9
	setUpTranslation = translate(glm::mat4(1.0f), glm::vec3(1.5f, 1.25f, 0.0f));
	setUpScaling = scale(glm::mat4(1.0f), glm::vec3(1.0f, 2.5f, 1.0f));
	Model* model9right = new Model(vao, numVerticesPerCube, uboWorldMatrixBlock, vector<Model*>(), setUpTranslation, mat4(1.0f), setUpScaling, metal);

	// Creating left-part of the number 9
	setUpTranslation = translate(mat4(1.0f), vec3(-0.5f, 0.5f, 0.0f));
	setUpScaling = scale(mat4(1.0f), vec3(1.0f, 2.0f, 1.0f));
	Model* model9left = new Model(vao, numVerticesPerCube, uboWorldMatrixBlock, vector<Model*>(), setUpTranslation, mat4(1.0f), setUpScaling, metal);

	// Creating bottom-part of the number 9
	setUpTranslation = translate(mat4(1.0f), vec3(0.5f, 0.0f, 0.0f));
	Model* model9bottom = new Model(vao, numVerticesPerCube, uboWorldMatrixBlock, vector<Model*>(), setUpTranslation, mat4(1.0f), mat4(1.0f), metal);

	// Setting up the number 9
	vector<Model*> nineChildren = vector<Model*>();
	nineChildren.push_back(model9top);
	nineChildren.push_back(model9right);
	nineChildren.push_back(model9left);
	nineChildren.push_back(model9bottom);
	setUpTranslation = translate(mat4(1.0f), vec3(1.5f, 0.0f, 0.0f));
	Model* model9 = new Model(vao, 0, uboWorldMatrixBlock, nineChildren, setUpTranslation, mat4(1.0f), mat4(1.0f), metal);


	// Setting up the entire L9
	// This will be the root, and will be provided with the current world and sharedModel matrices in draw() from main()
	vector<Model*> L9Children = vector<Model*>();
	L9Children.push_back(modelL);
	L9Children.push_back(model9);
	Model* modelL9 = new Model(vao, 0, uboWorldMatrixBlock, L9Children, mat4(1.0f), mat4(1.0f), mat4(1.0f));

	// Setting up Sphere
	setUpTranslation = translate(glm::mat4(1.0f), glm::vec3(0.0f, 4.5f, 0.0f));
	setUpScaling = scale(glm::mat4(1.0f), glm::vec3(6.0f, 6.0f, 6.0f));
	Model* modelSphere = new Model(sphereVAO, sphereVertices, uboWorldMatrixBlock, vector<Model*>(), setUpTranslation, mat4(1.0f), setUpScaling, wood);

	// Setting up the entire model
	// This will be the root, and will be provided with the current world and sharedModel matrices in draw() from main()
	vector<Model*> entireModelChildren = vector<Model*>();
	entireModelChildren.push_back(modelL9);
	entireModelChildren.push_back(modelSphere);
	Model* entireModel = new Model(vao, 0, uboWorldMatrixBlock, entireModelChildren, mat4(1.0f), mat4(1.0f), mat4(1.0f));

	return entireModel;
}

Model* makeL9BottomModel(int vao) {

	// Draw L9 using hierarchical modeling, start at the lowest model(s) in the hierarchy
	glm::mat4 setUpScaling = mat4(1.0f);
	glm::mat4 setUpRotation = mat4(1.0f);
	glm::mat4 setUpTranslation = mat4(1.0f);

	// Creating bottom-part of the letter L
	setUpTranslation = translate(glm::mat4(1.0f), glm::vec3(-1.0f, -1.0f, 0.0f));
	setUpScaling = scale(glm::mat4(1.0f), glm::vec3(1.0f, 2.5f, 1.0f));
	Model* modelLbottomBar = new Model(vao, numVerticesPerCube, uboWorldMatrixBlock, vector<Model*>(), setUpTranslation, mat4(1.0f), setUpScaling, box);

	// Creating right-part of the letter L
	setUpScaling = scale(glm::mat4(1.0f), glm::vec3(2.0f, 1.0f, 1.0f));
	setUpTranslation = translate(glm::mat4(1.0f), glm::vec3(0.5f, -1.75f, 0.0f));
	Model* modelLverticalBar = new Model(vao, numVerticesPerCube, uboWorldMatrixBlock, vector<Model*>(), setUpTranslation, mat4(1.0f), setUpScaling, box);

	// Setting up the letter L
	vector<Model*> LChildren = vector<Model*>();
	LChildren.push_back(modelLbottomBar);
	LChildren.push_back(modelLverticalBar);
	//The pieces of the L are placed such that the entire L is centered at origin on all axes
	//We can then very simply manipulate this modelL to transform the entire L
	//for example, to scoot the L left to make room for the number, making the entire L9 centered.
	setUpTranslation = translate(glm::mat4(1.0f), glm::vec3(-2.0f, 0.0f, 0.0f));
	Model* modelL = new Model(vao, 0, uboWorldMatrixBlock, LChildren, setUpTranslation, mat4(1.0f), mat4(1.0f));

	// Creating right-part of the number 9
	setUpTranslation = translate(glm::mat4(1.0f), glm::vec3(1.5f, -1.0f, 0.0f));
	setUpScaling = scale(glm::mat4(1.0f), glm::vec3(1.0f, 2.5f, 1.0f));
	Model* model9bottom = new Model(vao, numVerticesPerCube, uboWorldMatrixBlock, vector<Model*>(), setUpTranslation, mat4(1.0f), setUpScaling, metal);

	// Setting up the number 9
	vector<Model*> nineChildren = vector<Model*>();
	nineChildren.push_back(model9bottom);
	setUpTranslation = translate(glm::mat4(1.0f), glm::vec3(1.5f, 0.0f, 0.0f));
	Model* model9 = new Model(vao, 0, uboWorldMatrixBlock, nineChildren, setUpTranslation, mat4(1.0f), mat4(1.0f));


	// Setting up the entire L9
	// This will be the root, and will be provided with the current world and sharedModel matrices in draw() from main()
	vector<Model*> L9Children = vector<Model*>();
	L9Children.push_back(modelL);
	L9Children.push_back(model9);
	Model* modelL9 = new Model(vao, 0, uboWorldMatrixBlock, L9Children, mat4(1.0f), mat4(1.0f), mat4(1.0f));

	// Setting up the entire model
	// This will be the root, and will be provided with the current world and sharedModel matrices in draw() from main()
	vector<Model*> entireModelChildren = vector<Model*>();
	entireModelChildren.push_back(modelL9);
	Model* entireModel = new Model(vao, 0, uboWorldMatrixBlock, entireModelChildren, mat4(1.0f), mat4(1.0f), mat4(1.0f));

	return entireModel;

}

Model* makeI9Model(int vao, int sphereVAO) {
	// Draw I9 using hierarchical modeling, start at the lowest model(s) in the hierarchy
	mat4 setUpScaling = mat4(1.0f);
	mat4 setUpRotation = mat4(1.0f);
	mat4 setUpTranslation = mat4(1.0f);

	// Creating top-part of the letter I
	setUpTranslation = translate(mat4(1.0f), vec3(-2.0f, 4.5f, 0.0f));
	setUpScaling = scale(mat4(1.0f), vec3(4.0f, 1.0f, 1.0f));
	Model* modelItopBar = new Model(vao, numVerticesPerCube, uboWorldMatrixBlock, vector<Model*>(), setUpTranslation, mat4(1.0f), setUpScaling, box);


	// Creating middlet-part of the letter I
	setUpTranslation = translate(glm::mat4(1.0f), glm::vec3(-2.0f, 2.75f, 0.0f));
	setUpScaling = scale(glm::mat4(1.0f), glm::vec3(1.0f, 2.5f, 1.0f));
	Model* modelImiddleBar = new Model(vao, numVerticesPerCube, uboWorldMatrixBlock, vector<Model*>(), setUpTranslation, mat4(1.0f), setUpScaling, box);

	// Setting up the letter I
	vector<Model*> IChildren = vector<Model*>();
	IChildren.push_back(modelItopBar);
	IChildren.push_back(modelImiddleBar);
	//The pieces of the I are placed such that the entire I is centered at origin on all axes
	//We can then very simply manipulate this modelI to transform the entire I
	//for example, to scoot the I left to make room for the number, making the entire I9 centered.
	setUpTranslation = translate(mat4(1.0f), vec3(-1.0f, -2.0f, 0.0f));
	Model* modelI = new Model(vao, 0, uboWorldMatrixBlock, IChildren, setUpTranslation, mat4(1.0f), mat4(1.0f));


	// Creating top-part of the number 9
	setUpTranslation = translate(mat4(1.0f), vec3(3.75f, 4.5f, 0.0f));
	setUpScaling = scale(mat4(1.0f), vec3(1.5f, 1.0f, 1.0f));
	Model* model9top = new Model(vao, numVerticesPerCube, uboWorldMatrixBlock, vector<Model*>(), setUpTranslation, mat4(1.0f), setUpScaling, metal);

	// Creating left-part of the number 9
	setUpTranslation = translate(mat4(1.0f), vec3(2.5f, 3.25f, 0.0f));
	setUpScaling = scale(mat4(1.0f), vec3(1.0f, 3.5f, 1.0f));
	Model* model9left = new Model(vao, numVerticesPerCube, uboWorldMatrixBlock, vector<Model*>(), setUpTranslation, mat4(1.0f), setUpScaling, metal);

	// Creating bottom-part of the number 9
	setUpTranslation = translate(mat4(1.0f), vec3(3.75f, 2.0f, 0.0f));
	setUpScaling = scale(mat4(1.0f), vec3(2.0f, 1.0f, 1.0f));
	Model* model9bottom = new Model(vao, numVerticesPerCube, uboWorldMatrixBlock, vector<Model*>(), setUpTranslation, mat4(1.0f), setUpScaling, metal);

	// Creating right-part of the number 9
	setUpTranslation = translate(glm::mat4(1.0f), glm::vec3(5.0f, 3.0f, 0.0f));
	setUpScaling = scale(glm::mat4(1.0f), glm::vec3(1.0f, 4.0f, 1.0f));
	Model* model9right = new Model(vao, numVerticesPerCube, uboWorldMatrixBlock, vector<Model*>(), setUpTranslation, mat4(1.0f), setUpScaling, metal);


	// Setting up the number 9
	vector<Model*> nineChildren = vector<Model*>();
	nineChildren.push_back(model9top);
	nineChildren.push_back(model9right);
	nineChildren.push_back(model9left);
	nineChildren.push_back(model9bottom);
	setUpTranslation = translate(mat4(1.0f), vec3(-1.0f, -2.0f, 0.0f));
	Model* model9 = new Model(vao, 0, uboWorldMatrixBlock, nineChildren, setUpTranslation, mat4(1.0f), mat4(1.0f));


	// Setting up the entire I9
	// This will be the root, and will be provided with the current world and sharedModel matrices in draw() from main()
	vector<Model*> I9Children = vector<Model*>();
	I9Children.push_back(modelI);
	I9Children.push_back(model9);
	Model* modelI9 = new Model(vao, 0, uboWorldMatrixBlock, I9Children, mat4(1.0f), mat4(1.0f), mat4(1.0f));

	// Setting up Sphere
	setUpTranslation = translate(glm::mat4(1.0f), glm::vec3(0.0f, 5.0f, 0.0f));
	setUpScaling = scale(glm::mat4(1.0f), glm::vec3(6.0f, 6.0f, 6.0f));
	Model* modelSphere = new Model(sphereVAO, sphereVertices, uboWorldMatrixBlock, vector<Model*>(), setUpTranslation, mat4(1.0f), setUpScaling, wood);

	// Setting up the entire model
	// This will be the root, and will be provided with the current world and sharedModel matrices in draw() from main()
	vector<Model*> entireModelChildren = vector<Model*>();
	entireModelChildren.push_back(modelI9);
	entireModelChildren.push_back(modelSphere);
	Model* entireModel = new Model(vao, 0, uboWorldMatrixBlock, entireModelChildren, mat4(1.0f), mat4(1.0f), mat4(1.0f));

	return entireModel;
}

Model* makeI9BottomModel(int vao) {
	// Draw I9 using hierarchical modeling, start at the lowest model(s) in the hierarchy
	glm::mat4 setUpScaling = mat4(1.0f);
	glm::mat4 setUpRotation = mat4(1.0f);
	glm::mat4 setUpTranslation = mat4(1.0f);

	// Creating middlet-part of the letter I
	setUpTranslation = translate(glm::mat4(1.0f), glm::vec3(-2.0f, 1.0f, 0.0f));
	setUpScaling = scale(glm::mat4(1.0f), glm::vec3(1.0f, 2.5f, 1.0f));
	Model* modelImiddleBar = new Model(vao, numVerticesPerCube, uboWorldMatrixBlock, vector<Model*>(), setUpTranslation, mat4(1.0f), setUpScaling, box);

	// Creating bottom-part of the letter I
	setUpTranslation = translate(glm::mat4(1.0f), glm::vec3(-2.0f, 0.0f, 0.0f));
	setUpScaling = scale(glm::mat4(1.0f), glm::vec3(4.0f, 1.0f, 1.0f));
	Model* modelIbottomBar = new Model(vao, numVerticesPerCube, uboWorldMatrixBlock, vector<Model*>(), setUpTranslation, mat4(1.0f), setUpScaling, box);

	// Setting up the letter I
	vector<Model*> IChildren = vector<Model*>();
	IChildren.push_back(modelImiddleBar);
	IChildren.push_back(modelIbottomBar);
	//The pieces of the I are placed such that the entire I is centered at origin on all axes
	//We can then very simply manipulate this modelI to transform the entire I
	//for example, to scoot the I left to make room for the number, making the entire I9 centered.
	setUpTranslation = translate(glm::mat4(1.0f), glm::vec3(-1.0f, -2.0f, 0.0f));
	Model* modelI = new Model(vao, 0, uboWorldMatrixBlock, IChildren, setUpTranslation, mat4(1.0f), mat4(1.0f));


	// Creating right-part of the number 9
	setUpTranslation = translate(glm::mat4(1.0f), glm::vec3(5.0f, 0.5f, 0.0f));
	setUpScaling = scale(glm::mat4(1.0f), glm::vec3(1.0f, 2.5f, 1.0f));
	Model* model9right = new Model(vao, numVerticesPerCube, uboWorldMatrixBlock, vector<Model*>(), setUpTranslation, mat4(1.0f), setUpScaling, metal);


	// Setting up the number 9
	vector<Model*> nineChildren = vector<Model*>();
	nineChildren.push_back(model9right);
	setUpTranslation = translate(glm::mat4(1.0f), glm::vec3(-1.0f, -2.0f, 0.0f));
	Model* model9 = new Model(vao, 0, uboWorldMatrixBlock, nineChildren, setUpTranslation, mat4(1.0f), mat4(1.0f));


	// Setting up the entire I9
	// This will be the root, and will be provided with the current world and sharedModel matrices in draw() from main()
	vector<Model*> I9Children = vector<Model*>();
	I9Children.push_back(modelI);
	I9Children.push_back(model9);
	Model* modelI9 = new Model(vao, 0, uboWorldMatrixBlock, I9Children, mat4(1.0f), mat4(1.0f), mat4(1.0f));

	// Setting up the entire model
	// This will be the root, and will be provided with the current world and sharedModel matrices in draw() from main()
	vector<Model*> entireModelChildren = vector<Model*>();
	entireModelChildren.push_back(modelI9);
	Model* entireModel = new Model(vao, 0, uboWorldMatrixBlock, entireModelChildren, mat4(1.0f), mat4(1.0f), mat4(1.0f));

	return entireModel;
}

Model* makeU3Model(int vao, int sphereVAO) {
	// Draw U3 using hierarchical modeling, start at the lowest model(s) in the hierarchy
	glm::mat4 setUpScaling = mat4(1.0f);
	glm::mat4 setUpRotation = mat4(1.0f);
	glm::mat4 setUpTranslation = mat4(1.0f);

	// Creating left-part of the letter U
	setUpTranslation = translate(glm::mat4(1.0f), glm::vec3(-3.0f, 2.5f, 0.0f));
	setUpScaling = scale(glm::mat4(1.0f), glm::vec3(1.0f, 2.5f, 1.0f));
	Model* modelUleftBar = new Model(vao, numVerticesPerCube, uboWorldMatrixBlock, vector<Model*>(), setUpTranslation, mat4(1.0f), setUpScaling, box);

	// Creating right-part of the letter U
	setUpTranslation = translate(glm::mat4(1.0f), glm::vec3(0.0f, 2.5f, 0.0f));
	setUpScaling = scale(glm::mat4(1.0f), glm::vec3(1.0f, 2.5f, 1.0f));
	Model* modelUrightBar = new Model(vao, numVerticesPerCube, uboWorldMatrixBlock, vector<Model*>(), setUpTranslation, mat4(1.0f), setUpScaling, box);

	// Setting up the letter U
	vector<Model*> UChildren = vector<Model*>();
	UChildren.push_back(modelUleftBar);
	UChildren.push_back(modelUrightBar);
	//The pieces of the U are placed such that the entire U is centered at origin on all axes
	//We can then very simply manipulate this modelU to transform the entire U
	//for example, to scoot the U left to make room for the number, making the entire U3 centered.
	setUpTranslation = translate(glm::mat4(1.0f), glm::vec3(-2.0f, 0.0f, 0.0f));
	Model* modelU = new Model(vao, 0, uboWorldMatrixBlock, UChildren, setUpTranslation, mat4(1.0f), mat4(1.0f));

	// Creating base-part of the number 3
	setUpTranslation = translate(glm::mat4(1.0f), glm::vec3(4.0f, 2.0f, 0.0f));
	setUpScaling = scale(glm::mat4(1.0f), glm::vec3(1.0f, 3.0f, 1.0f));
	Model* model3base = new Model(vao, numVerticesPerCube, uboWorldMatrixBlock, vector<Model*>(), setUpTranslation, mat4(1.0f), setUpScaling, metal);

	// Creating topArm-part of the number 3
	setUpTranslation = translate(glm::mat4(1.0f), glm::vec3(2.5f, 3.0f, 0.0f));
	setUpScaling = scale(glm::mat4(1.0f), glm::vec3(2.5f, 1.0f, 1.0f));
	Model* model3topArm = new Model(vao, numVerticesPerCube, uboWorldMatrixBlock, vector<Model*>(), setUpTranslation, mat4(1.0f), setUpScaling, metal);

	// Creating middleArm-part of the number 3
	setUpTranslation = translate(glm::mat4(1.0f), glm::vec3(3.0f, 0.75f, 0.0f));
	setUpScaling = scale(glm::mat4(1.0f), glm::vec3(1.5f, 1.0f, 1.0f));
	Model* model3middleArm = new Model(vao, numVerticesPerCube, uboWorldMatrixBlock, vector<Model*>(), setUpTranslation, mat4(1.0f), setUpScaling, metal);

	// Setting up the number 3
	vector<Model*> threeChildren = vector<Model*>();
	threeChildren.push_back(model3base);
	threeChildren.push_back(model3topArm);
	threeChildren.push_back(model3middleArm);
	//threeChildren.push_back(model3bottomArm);
	setUpTranslation = translate(glm::mat4(1.0f), glm::vec3(0.0f, 0.0f, 0.0f));
	Model* model3 = new Model(vao, 0, uboWorldMatrixBlock, threeChildren, setUpTranslation, mat4(1.0f), mat4(1.0f));

	// Setting up the entire U3
	// This will be the root, and will be provided with the current world and sharedModel matrices in draw() from main()
	vector<Model*> U3Children = vector<Model*>();
	U3Children.push_back(modelU);
	U3Children.push_back(model3);
	Model* modelU3 = new Model(vao, 0, uboWorldMatrixBlock, U3Children, mat4(1.0f), mat4(1.0f), mat4(1.0f));

	// Setting up Sphere
	setUpTranslation = translate(glm::mat4(1.0f), glm::vec3(0.0f, 4.5f, 0.0f));
	setUpScaling = scale(glm::mat4(1.0f), glm::vec3(6.0f, 6.0f, 6.0f));
	Model* modelSphere = new Model(sphereVAO, sphereVertices, uboWorldMatrixBlock, vector<Model*>(), setUpTranslation, mat4(1.0f), setUpScaling, wood);

	// Setting up the entire model
	// This will be the root, and will be provided with the current world and sharedModel matrices in draw() from main()
	vector<Model*> entireModelChildren = vector<Model*>();
	entireModelChildren.push_back(modelU3);
	entireModelChildren.push_back(modelSphere);
	Model* entireModel = new Model(vao, 0, uboWorldMatrixBlock, entireModelChildren, mat4(1.0f), mat4(1.0f), mat4(1.0f));

	return entireModel;
}


Model* makeU3BottomModel(int vao) {
	// Draw U3 using hierarchical modeling, start at the lowest model(s) in the hierarchy
	mat4 setUpScaling = mat4(1.0f);
	mat4 setUpRotation = mat4(1.0f);
	mat4 setUpTranslation = mat4(1.0f);

	// Creating left-part of the letter U
	setUpTranslation = translate(glm::mat4(1.0f), glm::vec3(-3.0f, 0.0f, 0.0f));
	setUpScaling = scale(glm::mat4(1.0f), glm::vec3(1.0f, 2.5f, 1.0f));
	Model* modelUleftBar = new Model(vao, numVerticesPerCube, uboWorldMatrixBlock, vector<Model*>(), setUpTranslation, mat4(1.0f), setUpScaling, box);


	// Creating bottom-part of the letter U
	setUpTranslation = translate(glm::mat4(1.0f), glm::vec3(-1.5f, -1.25f, 0.0f));
	setUpScaling = scale(glm::mat4(1.0f), glm::vec3(4.0f, 1.0f, 1.0f));
	Model* modelUbottomBar = new Model(vao, numVerticesPerCube, uboWorldMatrixBlock, vector<Model*>(), setUpTranslation, mat4(1.0f), setUpScaling, box);

	// Creating right-part of the letter U
	setUpTranslation = translate(glm::mat4(1.0f), glm::vec3(0.0f, 0.0f, 0.0f));
	setUpScaling = scale(glm::mat4(1.0f), glm::vec3(1.0f, 2.5f, 1.0f));
	Model* modelUrightBar = new Model(vao, numVerticesPerCube, uboWorldMatrixBlock, vector<Model*>(), setUpTranslation, mat4(1.0f), setUpScaling, box);

	// Setting up the letter U
	vector<Model*> UChildren = vector<Model*>();
	UChildren.push_back(modelUleftBar);
	UChildren.push_back(modelUbottomBar);
	UChildren.push_back(modelUrightBar);
	//The pieces of the U are placed such that the entire U is centered at origin on all axes
	//We can then very simply manipulate this modelU to transform the entire U
	//for example, to scoot the U left to make room for the number, making the entire U3 centered.
	setUpTranslation = translate(mat4(1.0f), vec3(-2.0f, 0.0f, 0.0f));
	Model* modelU = new Model(vao, 0, uboWorldMatrixBlock, UChildren, setUpTranslation, mat4(1.0f), mat4(1.0f));


	// Creating base-part of the number 3
	setUpTranslation = translate(glm::mat4(1.0f), glm::vec3(4.0f, -0.5f, 0.0f));
	setUpScaling = scale(glm::mat4(1.0f), glm::vec3(1.0f, 2.5f, 1.0f));
	Model* model3base = new Model(vao, numVerticesPerCube, uboWorldMatrixBlock, vector<Model*>(), setUpTranslation, mat4(1.0f), setUpScaling, metal);


	// Creating bottomArm-part of the number 3
	setUpTranslation = translate(glm::mat4(1.0f), glm::vec3(2.5f, -1.25f, 0.0f));
	setUpScaling = scale(glm::mat4(1.0f), glm::vec3(2.5f, 1.0f, 1.0f));
	Model* model3bottomArm = new Model(vao, numVerticesPerCube, uboWorldMatrixBlock, vector<Model*>(), setUpTranslation, mat4(1.0f), setUpScaling, metal);

	// Setting up the number 3
	vector<Model*> threeChildren = vector<Model*>();
	threeChildren.push_back(model3base);
	threeChildren.push_back(model3bottomArm);
	setUpTranslation = translate(mat4(1.0f), vec3(0.0f, 0.0f, 0.0f));
	Model* model3 = new Model(vao, 0, uboWorldMatrixBlock, threeChildren, setUpTranslation, mat4(1.0f), mat4(1.0f));


	// Setting up the entire U3
	// This will be the root, and will be provided with the current world and sharedModel matrices in draw() from main()
	vector<Model*> U3Children = vector<Model*>();
	U3Children.push_back(modelU);
	U3Children.push_back(model3);
	Model* modelU3 = new Model(vao, 0, uboWorldMatrixBlock, U3Children, mat4(1.0f), mat4(1.0f), mat4(1.0f));

	// Setting up the entire model
	// This will be the root, and will be provided with the current world and sharedModel matrices in draw() from main()
	vector<Model*> entireModelChildren = vector<Model*>();
	entireModelChildren.push_back(modelU3);
	Model* entireModel = new Model(vao, 0, uboWorldMatrixBlock, entireModelChildren, mat4(1.0f), mat4(1.0f), mat4(1.0f));

	return entireModel;
}

Model* makeT9Model(int vao, int sphereVAO) {
	// Draw T9 using hierarchical modeling, start at the lowest model(s) in the hierarchy
	glm::mat4 setUpScaling = mat4(1.0f);
	glm::mat4 setUpRotation = mat4(1.0f);
	glm::mat4 setUpTranslation = mat4(1.0f);

	// Creating top-part of the letter T
	setUpTranslation = translate(glm::mat4(1.0f), glm::vec3(-1.5f, 4.25f, 0.0f));
	setUpScaling = scale(glm::mat4(1.0f), glm::vec3(4.0f, 1.0f, 1.0f));
	Model* modelTtopBar = new Model(vao, numVerticesPerCube, uboWorldMatrixBlock, vector<Model*>(), setUpTranslation, mat4(1.0f), setUpScaling, box);

	// Creating middle-part of the letter T
	setUpTranslation = translate(glm::mat4(1.0f), glm::vec3(-1.5f, 2.5f, 0.0f));
	setUpScaling = scale(glm::mat4(1.0f), glm::vec3(1.0f, 2.5f, 1.0f));
	Model* modelTmiddleBar = new Model(vao, numVerticesPerCube, uboWorldMatrixBlock, vector<Model*>(), setUpTranslation, mat4(1.0f), setUpScaling, box);

	// Setting up the letter T
	vector<Model*> TChildren = vector<Model*>();
	TChildren.push_back(modelTtopBar);
	TChildren.push_back(modelTmiddleBar);
	//The pieces of the I are placed such that the entire T is centered at origin on all axes
	//We can then very simply manipulate this modelI to transform the entire T
	//for example, to scoot the T left to make room for the number, making the entire I9 centered.
	setUpTranslation = translate(glm::mat4(1.0f), glm::vec3(-1.0f, -2.0f, 0.0f));
	Model* modelT = new Model(vao, 0, uboWorldMatrixBlock, TChildren, setUpTranslation, mat4(1.0f), mat4(1.0f));

	// Creating top-part of the number 9
	setUpTranslation = translate(mat4(1.0f), vec3(3.75f, 4.5f, 0.0f));
	setUpScaling = scale(mat4(1.0f), vec3(1.5f, 1.0f, 1.0f));
	Model* model9top = new Model(vao, numVerticesPerCube, uboWorldMatrixBlock, vector<Model*>(), setUpTranslation, mat4(1.0f), setUpScaling, metal);

	// Creating left-part of the number 9
	setUpTranslation = translate(mat4(1.0f), vec3(2.5f, 3.25f, 0.0f));
	setUpScaling = scale(mat4(1.0f), vec3(1.0f, 3.5f, 1.0f));
	Model* model9left = new Model(vao, numVerticesPerCube, uboWorldMatrixBlock, vector<Model*>(), setUpTranslation, mat4(1.0f), setUpScaling, metal);

	// Creating bottom-part of the number 9
	setUpTranslation = translate(mat4(1.0f), vec3(3.75f, 2.0f, 0.0f));
	setUpScaling = scale(mat4(1.0f), vec3(2.0f, 1.0f, 1.0f));
	Model* model9bottom = new Model(vao, numVerticesPerCube, uboWorldMatrixBlock, vector<Model*>(), setUpTranslation, mat4(1.0f), setUpScaling, metal);

	// Creating right-part of the number 9
	setUpTranslation = translate(glm::mat4(1.0f), glm::vec3(5.0f, 3.0f, 0.0f));
	setUpScaling = scale(glm::mat4(1.0f), glm::vec3(1.0f, 4.0f, 1.0f));
	Model* model9right = new Model(vao, numVerticesPerCube, uboWorldMatrixBlock, vector<Model*>(), setUpTranslation, mat4(1.0f), setUpScaling, metal);


	// Setting up the number 9
	vector<Model*> nineChildren = vector<Model*>();
	nineChildren.push_back(model9top);
	nineChildren.push_back(model9right);
	nineChildren.push_back(model9left);
	nineChildren.push_back(model9bottom);
	setUpTranslation = translate(mat4(1.0f), vec3(-1.0f, -2.0f, 0.0f));
	Model* model9 = new Model(vao, 0, uboWorldMatrixBlock, nineChildren, setUpTranslation, mat4(1.0f), mat4(1.0f));


	// Setting up the entire T9
	// This will be the root, and will be provided with the current world and sharedModel matrices in draw() from main()
	vector<Model*> T9Children = vector<Model*>();
	T9Children.push_back(modelT);
	T9Children.push_back(model9);
	Model* modelT9 = new Model(vao, 0, uboWorldMatrixBlock, T9Children, mat4(1.0f), mat4(1.0f), mat4(1.0f));

	// Setting up Sphere
	setUpTranslation = translate(glm::mat4(1.0f), glm::vec3(0.0f, 5.0f, 0.0f));
	setUpScaling = scale(glm::mat4(1.0f), glm::vec3(6.0f, 6.0f, 6.0f));
	Model* modelSphere = new Model(sphereVAO, sphereVertices, uboWorldMatrixBlock, vector<Model*>(), setUpTranslation, mat4(1.0f), setUpScaling, wood);

	// Setting up the entire model
	// This will be the root, and will be provided with the current world and sharedModel matrices in draw() from main()
	vector<Model*> entireModelChildren = vector<Model*>();
	entireModelChildren.push_back(modelT9);
	entireModelChildren.push_back(modelSphere);
	Model* entireModel = new Model(vao, 0, uboWorldMatrixBlock, entireModelChildren, mat4(1.0f), mat4(1.0f), mat4(1.0f));

	return entireModel;
}

Model* makeT9BottomModel(int vao) {
	// Draw T9 using hierarchical modeling, start at the lowest model(s) in the hierarchy
	glm::mat4 setUpScaling = mat4(1.0f);
	glm::mat4 setUpRotation = mat4(1.0f);
	glm::mat4 setUpTranslation = mat4(1.0f);

	// Creating middlet-part of the letter T
	setUpTranslation = translate(glm::mat4(1.0f), glm::vec3(-1.5f, 0.5f, 0.0f));
	setUpScaling = scale(glm::mat4(1.0f), glm::vec3(1.0f, 2.0f, 1.0f));
	Model* modelTmiddleBar = new Model(vao, numVerticesPerCube, uboWorldMatrixBlock, vector<Model*>(), setUpTranslation, mat4(1.0f), setUpScaling, box);

	// Setting up the letter T
	vector<Model*> TChildren = vector<Model*>();
	TChildren.push_back(modelTmiddleBar);
	//The pieces of the I are placed such that the entire I is centered at origin on all axes
	//We can then very simply manipulate this modelI to transform the entire I
	//for example, to scoot the I left to make room for the number, making the entire I9 centered.
	setUpTranslation = translate(glm::mat4(1.0f), glm::vec3(-1.0f, -2.0f, 0.0f));
	Model* modelI = new Model(vao, 0, uboWorldMatrixBlock, TChildren, setUpTranslation, mat4(1.0f), mat4(1.0f));


	// Creating right-part of the number 9
	setUpTranslation = translate(glm::mat4(1.0f), glm::vec3(5.0f, 0.5f, 0.0f));
	setUpScaling = scale(glm::mat4(1.0f), glm::vec3(1.0f, 2.5f, 1.0f));
	Model* model9right = new Model(vao, numVerticesPerCube, uboWorldMatrixBlock, vector<Model*>(), setUpTranslation, mat4(1.0f), setUpScaling, metal);


	// Setting up the number 9
	vector<Model*> nineChildren = vector<Model*>();
	nineChildren.push_back(model9right);
	setUpTranslation = translate(glm::mat4(1.0f), glm::vec3(-1.0f, -2.0f, 0.0f));
	Model* model9 = new Model(vao, 0, uboWorldMatrixBlock, nineChildren, setUpTranslation, mat4(1.0f), mat4(1.0f));


	// Setting up the entire T9
	// This will be the root, and will be provided with the current world and sharedModel matrices in draw() from main()
	vector<Model*> T9Children = vector<Model*>();
	T9Children.push_back(modelI);
	T9Children.push_back(model9);
	Model* modelT9 = new Model(vao, 0, uboWorldMatrixBlock, T9Children, mat4(1.0f), mat4(1.0f), mat4(1.0f));

	// Setting up the entire model
	// This will be the root, and will be provided with the current world and sharedModel matrices in draw() from main()
	vector<Model*> entireModelChildren = vector<Model*>();
	entireModelChildren.push_back(modelT9);
	Model* entireModel = new Model(vao, 0, uboWorldMatrixBlock, entireModelChildren, mat4(1.0f), mat4(1.0f), mat4(1.0f));

	return entireModel;
}

Model* makeC4Model(int vao, int sphereVAO) {
	// Draw C4 using hierarchical modeling, start at the lowest model(s) in the hierarchy
	mat4 setUpScaling = mat4(1.0f);
	mat4 setUpRotation = mat4(1.0f);
	mat4 setUpTranslation = mat4(1.0f);

	// Creating left-part of the letter C
	setUpTranslation = translate(glm::mat4(1.0f), glm::vec3(0.0f, 2.5f, 0.0f));
	setUpScaling = scale(glm::mat4(1.0f), glm::vec3(1.0f, 2.5f, 1.0f));
	Model* modelCleftBar = new Model(vao, numVerticesPerCube, uboWorldMatrixBlock, vector<Model*>(), setUpTranslation, mat4(1.0f), setUpScaling, box);

	// Creating top-part of the letter C
	setUpScaling = scale(glm::mat4(1.0f), glm::vec3(3.0f, 1.0f, 1.0f));
	setUpTranslation = translate(glm::mat4(1.0f), glm::vec3(1.0f, 4.0f, 0.0f));
	Model* modelCtopBar = new Model(vao, numVerticesPerCube, uboWorldMatrixBlock, vector<Model*>(), setUpTranslation, mat4(1.0f), setUpScaling, box);

	// Setting up the letter C
	vector<Model*> CChildren = vector<Model*>();
	CChildren.push_back(modelCleftBar);
	CChildren.push_back(modelCtopBar);
	//The pieces of the C are placed such that the entire C is centered at origin on all axes
	//We can then very simply manipulate this modelC to transform the entire C
	//for example, to scoot the C left to make room for the number, making the entire C4 centered.
	setUpTranslation = translate(glm::mat4(1.0f), glm::vec3(-5.0f, -2.0f, 0.0f));
	Model* modelC = new Model(vao, 0, uboWorldMatrixBlock, CChildren, setUpTranslation, mat4(1.0f), mat4(1.0f));

	// Creating right-part of the number 4
	setUpTranslation = translate(glm::mat4(1.0f), glm::vec3(2.0f, 2.5f, 0.0f));
	setUpScaling = scale(glm::mat4(1.0f), glm::vec3(1.0f, 2.75f, 1.0f));
	Model* model4right = new Model(vao, numVerticesPerCube, uboWorldMatrixBlock, vector<Model*>(), setUpTranslation, mat4(1.0f), setUpScaling, metal);

	// Creating middle-part of the number 4
	setUpTranslation = translate(glm::mat4(1.0f), glm::vec3(0.0f, 1.5f, 0.0f));
	setUpScaling = scale(glm::mat4(1.0f), glm::vec3(3.0f, 1.0f, 1.0f));
	Model* model4middle = new Model(vao, numVerticesPerCube, uboWorldMatrixBlock, vector<Model*>(), setUpTranslation, mat4(1.0f), setUpScaling, metal);

	// Creating left-part of the number 4
	setUpTranslation = translate(glm::mat4(1.0f), glm::vec3(-1.0f, 3.0f, 0.0f));
	setUpScaling = scale(glm::mat4(1.0f), glm::vec3(1.0f, 2.0f, 1.0f));
	Model* model4left = new Model(vao, numVerticesPerCube, uboWorldMatrixBlock, vector<Model*>(), setUpTranslation, mat4(1.0f), setUpScaling, metal);

	// Setting up the number 4
	vector<Model*> fourChildren = vector<Model*>();
	fourChildren.push_back(model4left);
	fourChildren.push_back(model4middle);
	fourChildren.push_back(model4right);
	setUpTranslation = translate(glm::mat4(1.0f), glm::vec3(2.0f, -2.0f, 0.0f));
	Model* model4 = new Model(vao, 0, uboWorldMatrixBlock, fourChildren, setUpTranslation, mat4(1.0f), mat4(1.0f));

	// Setting up the entire C4
	// This will be the root, and will be provided with the current world and sharedModel matrices in draw() from main()
	vector<Model*> C4Children = vector<Model*>();
	C4Children.push_back(modelC);
	C4Children.push_back(model4);
	Model* modelC4 = new Model(vao, 0, uboWorldMatrixBlock, C4Children, mat4(1.0f), mat4(1.0f), mat4(1.0f));

	// Setting up Sphere
	setUpTranslation = translate(glm::mat4(1.0f), glm::vec3(0.0f, 5.0f, 0.0f));
	setUpScaling = scale(glm::mat4(1.0f), glm::vec3(6.0f, 6.0f, 6.0f));
	Model* modelSphere = new Model(sphereVAO, sphereVertices, uboWorldMatrixBlock, vector<Model*>(), setUpTranslation, mat4(1.0f), setUpScaling, wood);

	// Setting up the entire model
	// This will be the root, and will be provided with the current world and sharedModel matrices in draw() from main()
	vector<Model*> entireModelChildren = vector<Model*>();
	entireModelChildren.push_back(modelC4);
	entireModelChildren.push_back(modelSphere);
	Model* entireModel = new Model(vao, 0, uboWorldMatrixBlock, entireModelChildren, mat4(1.0f), mat4(1.0f), mat4(1.0f));

	return entireModel;
}

Model* makeC4BottomModel(int vao) {
	// Draw C4 using hierarchical modeling, start at the lowest model(s) in the hierarchy
	glm::mat4 setUpScaling = mat4(1.0f);
	glm::mat4 setUpRotation = mat4(1.0f);
	glm::mat4 setUpTranslation = mat4(1.0f);

	// Creating left-part of the letter C
	setUpTranslation = translate(glm::mat4(1.0f), glm::vec3(2.0f, 0.0f, 0.0f));
	setUpScaling = scale(glm::mat4(1.0f), glm::vec3(1.0f, 2.5f, 1.0f));
	Model* modelCleftBar = new Model(vao, numVerticesPerCube, uboWorldMatrixBlock, vector<Model*>(), setUpTranslation, mat4(1.0f), setUpScaling, box);

	// Creating bottom-part of the letter C
	setUpScaling = scale(glm::mat4(1.0f), glm::vec3(3.0f, 1.0f, 1.0f));
	setUpTranslation = translate(glm::mat4(1.0f), glm::vec3(3.0f, -0.75f, 0.0f));
	Model* modelCbottomBar = new Model(vao, numVerticesPerCube, uboWorldMatrixBlock, vector<Model*>(), setUpTranslation, mat4(1.0f), setUpScaling, box);

	// Setting up the letter C
	vector<Model*> CChildren = vector<Model*>();
	CChildren.push_back(modelCleftBar);
	CChildren.push_back(modelCbottomBar);
	//The pieces of the C are placed such that the entire C is centered at origin on all axes
	//We can then very simply manipulate this modelC to transform the entire C
	//for example, to scoot the C left to make room for the number, making the entire C4 centered.
	setUpTranslation = translate(glm::mat4(1.0f), glm::vec3(-7.0f, -2.0f, 0.0f));
	Model* modelC = new Model(vao, 0, uboWorldMatrixBlock, CChildren, setUpTranslation, mat4(1.0f), mat4(1.0f));


	// Creating right-part of the number 4
	setUpTranslation = translate(glm::mat4(1.0f), glm::vec3(2.0f, 0.0f, 0.0f));
	setUpScaling = scale(glm::mat4(1.0f), glm::vec3(1.0f, 2.5f, 1.0f));
	Model* model4right = new Model(vao, numVerticesPerCube, uboWorldMatrixBlock, vector<Model*>(), setUpTranslation, mat4(1.0f), setUpScaling, metal);


	// Setting up the number 4
	vector<Model*> fourChildren = vector<Model*>();
	fourChildren.push_back(model4right);
	setUpTranslation = translate(glm::mat4(1.0f), glm::vec3(2.0f, -2.0f, 0.0f));
	Model* model4 = new Model(vao, 0, uboWorldMatrixBlock, fourChildren, setUpTranslation, mat4(1.0f), mat4(1.0f));

	// Setting up the entire C4
	// This will be the root, and will be provided with the current world and sharedModel matrices in draw() from main()

	vector<Model*> C4Children = vector<Model*>();
	C4Children.push_back(modelC);
	C4Children.push_back(model4);
	Model* modelC4 = new Model(vao, 0, uboWorldMatrixBlock, C4Children, mat4(1.0f), mat4(1.0f), mat4(1.0f));

	// Setting up the entire model
	// This will be the root, and will be provided with the current world and sharedModel matrices in draw() from main()
	vector<Model*> entireModelChildren = vector<Model*>();
	entireModelChildren.push_back(modelC4);
	Model* entireModel = new Model(vao, 0, uboWorldMatrixBlock, entireModelChildren, mat4(1.0f), mat4(1.0f), mat4(1.0f), metal);

	return entireModel;
}

Model* makeT9Model(int vao) {
	// Draw T9 using hierarchical modeling, start at the lowest model(s) in the hierarchy
	mat4 setUpScaling = mat4(1.0f);
	mat4 setUpRotation = mat4(1.0f);
	mat4 setUpTranslation = mat4(1.0f);

	// Creating hat of the letter T
	setUpTranslation = translate(mat4(1.0f), vec3(-1.0f, 4.5f, 0.0f));
	setUpScaling = scale(mat4(1.0f), vec3(4.0f, 1.0f, 1.0f));
	Model* modelThat = new Model(vao, numVerticesPerCube, uboWorldMatrixBlock, vector<Model*>(), setUpTranslation, mat4(1.0f), setUpScaling, box);


	// Creating leg of the letter T
	setUpScaling = scale(mat4(1.0f), vec3(1.0f, 4.0f, 1.0f));
	setUpTranslation = translate(mat4(1.0f), vec3(-1.0f, 2.0f, 0.0f));
	Model* modelTleg = new Model(vao, numVerticesPerCube, uboWorldMatrixBlock, vector<Model*>(), setUpTranslation, mat4(1.0f), setUpScaling, box);

	// Setting up the letter T
	vector<Model*> TChildren = vector<Model*>();
	TChildren.push_back(modelThat);
	TChildren.push_back(modelTleg);

	//The pieces of the T are placed such that the entire T is centered at origin on all axes
	//We can then very simply manipulate this modelT to transform the entire T
	//for example, to scoot the T left to make room for the number, making the entire T9 centered.
	setUpTranslation = translate(mat4(1.0f), vec3(0.0f, -2.0f, 0.0f));
	Model* modelT = new Model(vao, 0, uboWorldMatrixBlock, TChildren, setUpTranslation, mat4(1.0f), mat4(1.0f));


	// Creating top-part of the number 9
	setUpTranslation = translate(mat4(1.0f), vec3(4.75f, 4.5f, 0.0f));
	setUpScaling = scale(mat4(1.0f), vec3(1.5f, 1.0f, 1.0f));
	Model* model9top = new Model(vao, numVerticesPerCube, uboWorldMatrixBlock, vector<Model*>(), setUpTranslation, mat4(1.0f), setUpScaling, metal);

	// Creating left-part of the number 9
	setUpTranslation = translate(mat4(1.0f), vec3(3.5f, 3.25f, 0.0f));
	setUpScaling = scale(mat4(1.0f), vec3(1.0f, 3.5f, 1.0f));
	Model* model9left = new Model(vao, numVerticesPerCube, uboWorldMatrixBlock, vector<Model*>(), setUpTranslation, mat4(1.0f), setUpScaling, metal);

	// Creating bottom-part of the number 9
	setUpTranslation = translate(mat4(1.0f), vec3(4.75f, 2.0f, 0.0f));
	setUpScaling = scale(mat4(1.0f), vec3(2.0f, 1.0f, 1.0f));
	Model* model9bottom = new Model(vao, numVerticesPerCube, uboWorldMatrixBlock, vector<Model*>(), setUpTranslation, mat4(1.0f), setUpScaling, metal);

	// Creating right-part of the number 9
	setUpTranslation = translate(mat4(1.0f), vec3(6.0f, 2.51f, 0.0f));
	setUpScaling = scale(mat4(1.0f), vec3(1.0f, 5.0f, 1.0f));
	Model* model9right = new Model(vao, numVerticesPerCube, uboWorldMatrixBlock, vector<Model*>(), setUpTranslation, mat4(1.0f), setUpScaling, metal);


	// Setting up the number 9
	vector<Model*> nineChildren = vector<Model*>();
	nineChildren.push_back(model9top);
	nineChildren.push_back(model9right);
	nineChildren.push_back(model9left);
	nineChildren.push_back(model9bottom);
	setUpTranslation = translate(mat4(1.0f), vec3(0.0f, -2.0f, 0.0f));
	Model* model9 = new Model(vao, 0, uboWorldMatrixBlock, nineChildren, setUpTranslation, mat4(1.0f), mat4(1.0f));


	// Setting up the entire T9
	// This will be the root, and will be provided with the current world and sharedModel matrices in draw() from main()
	vector<Model*> T9Children = vector<Model*>();
	T9Children.push_back(modelT);
	T9Children.push_back(model9);
	Model* modelT9 = new Model(vao, 0, uboWorldMatrixBlock, T9Children, mat4(1.0f), mat4(1.0f), mat4(1.0f));

	return modelT9;
}

Model* makeFloorModel(int vao) {
	// Draw floor using hierarchical modeling, start at the lowest model(s) in the hierarchy
	mat4 setUpScaling = scale(mat4(1.0f), vec3(1.0f));
	mat4 setUpRotation = rotate(mat4(1.0f), 0.0f, vec3(1.0f));
	mat4 setUpTranslation = translate(mat4(1.0f), vec3(0.0f));
	
	// This will be the root, and will be provided with the current world and sharedModel matrices in draw() from main()
	vector<Model*> children = vector<Model*>();
	Model* floorModel = new Model(vao, 6, uboWorldMatrixBlock, children, setUpTranslation, setUpRotation, setUpScaling, wood);

	return floorModel;
}

#pragma endregion

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
GLuint defaultShaderProgram;
GLuint phongLightShaderProgram;
GLuint shadowShaderProgram;
void useShader(int shaderProgram, mat4 projectionMatrix, mat4 viewMatrix) {
	glUseProgram(shaderProgram);

	worldMatrixLocation = glGetUniformLocation(shaderProgram, "worldMatrix");

	GLuint viewMatrixLocation = glGetUniformLocation(shaderProgram, "viewMatrix");
	glUniformMatrix4fv(viewMatrixLocation, 1, GL_FALSE, &viewMatrix[0][0]);

	GLuint projectionMatrixLocation = glGetUniformLocation(shaderProgram, "projectionMatrix");
	glUniformMatrix4fv(projectionMatrixLocation, 1, GL_FALSE, &projectionMatrix[0][0]);
}

void useStandardShader() {
	projectionMatrix = perspective(70.0f, // field of view in degrees
		(float)windowWidth / windowHeight,  // aspect ratio
		0.01f, 100.0f);
	useShader(defaultShaderProgram, projectionMatrix, viewMatrix);
}

mat4 lightProjectionMatrix = perspective(180.0f, 1.0f, 0.01f, 100.0f);
mat4 lightViewMatrix = lookAt(vec3(0.0f, 30.0f, 0.0f),  // eye
	vec3(0.0f, 0.0f, 0.0f),  // center
	vec3(1.0f, 0.0f, 0.0f)); // up
void useShadowShader() {
	useShader(shadowShaderProgram, lightProjectionMatrix, lightViewMatrix);
}

void useLightingShader() {
	projectionMatrix = perspective(70.0f, // field of view in degrees
		(float)windowWidth / windowHeight,  // aspect ratio
		0.01f, 100.0f);
	useShader(phongLightShaderProgram, projectionMatrix, viewMatrix);

	//Set up vertex shader uniforms
	GLuint depthVPLocation = glGetUniformLocation(phongLightShaderProgram, "depthVP");
	mat4 depthVP = lightProjectionMatrix * lightViewMatrix;
	glUniformMatrix4fv(depthVPLocation, 1, GL_FALSE, &depthVP[0][0]);

	GLuint lightPosition = glGetUniformLocation(phongLightShaderProgram, "lightPosition");
	glUniform3f(lightPosition, 0.0f, 30.0f, 0.0f);

  GLuint texture = glGetUniformLocation(phongLightShaderProgram, "someTexture");
  glUniform1i(texture, brickTexture);

  GLuint lightCoefficients = glGetUniformLocation(phongLightShaderProgram, "lightCoefficients");
  glUniform4f(lightCoefficients,  0.3f, 0.8f, 0.5f, 256);

  GLuint lightColor = glGetUniformLocation(phongLightShaderProgram, "lightColor");
  glUniform3f(lightColor, 1.0f, 1.0f, 1.0f);

	GLuint camPosition = glGetUniformLocation(phongLightShaderProgram, "cameraPosition");
	glUniform3f(camPosition, cameraPosition.x, cameraPosition.y, cameraPosition.z);

	//Set up fragment shader uniforms
	GLuint shouldRenderShadowsLocation = glGetUniformLocation(phongLightShaderProgram, "shouldRenderShadows");
	glUniform1f(shouldRenderShadowsLocation, renderShadows);

  GLuint shouldRenderTexturesLocation = glGetUniformLocation(phongLightShaderProgram, "shouldRenderTextures");
  glUniform1f(shouldRenderTexturesLocation, renderTextures);
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

	glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT, shadowMapWidth, shadowMapHeight, 0, GL_DEPTH_COMPONENT, GL_FLOAT, 0);
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

// reference https://www.glfw.org/docs/latest/group__window.html#gae49ee6ebc03fa2da024b89943a331355
void windowResizeCallback(GLFWwindow* window, int width, int height)
{
	windowHeight = height;
	windowWidth = width;
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
	GLFWwindow* window = glfwCreateWindow(windowWidth, windowHeight, "Comp371 - Group 14 P2", NULL, NULL);
	if (window == NULL)
	{
		cerr << "Failed to create GLFW window" << endl;
		glfwTerminate();
		return -1;
	}
	glfwMakeContextCurrent(window);
	glfwSetWindowSizeCallback(window, windowResizeCallback);

	// Initialize GLEW
	glewExperimental = true; // Needed for core profile
	if (glewInit() != GLEW_OK) {
		cerr << "Failed to create GLEW" << endl;
		glfwTerminate();
		return -1;
	}

	// Black background
	glClearColor(0.0f, 0.0f, 25 / 225.0f, 1.0f);

  // Load Textures
  #if defined(PLATFORM_OSX) || __linux__
  brickTexture = loadTexture("brick.jpg");
  woodTexture = loadTexture("wood.jpg");
  metalTexture = loadTexture("metal2.jpg");
  boxTexture = loadTexture("box.jpg");
  floorTilesTexture = loadTexture("floortiles.jpg");
  #else
  brickTexture = loadTexture("../Source/COMP371-Group14-Project/brick.jpg");
  woodTexture = loadTexture("../Source/COMP371-Group14-Project/wood.jpg");
  metalTexture = loadTexture("../Source/COMP371-Group14-Project/metal2.jpg");
  boxTexture = loadTexture("../Source/COMP371-Group14-Project/box.jpg");
  floorTilesTexture = loadTexture("../Source/COMP371-Group14-Project/floortiles.jpg");
  #endif

  std::cout << brickTexture << std::endl;
  std::cout << woodTexture << std::endl;
  std::cout << metalTexture << std::endl;

  brick = {};
  brick.texture = brickTexture;
  brick.lightCoefficients = vec4(0.4f, 0.4f, 0.4f, 0);
  brick.lightColor = vec3(0.9f);

  wood = {};
  wood.texture = woodTexture;
  wood.lightCoefficients = vec4(0.4f, 0.8f, 0.9f, 52);
  wood.lightColor = vec3(252.0f/255.0f, 244.0f/255.0f, 202.0f/255.0f);

  metal = {};
  metal.texture = metalTexture;
  metal.lightCoefficients = vec4(0.3f, 0.8f, 0.5f, 256);
  metal.lightColor = vec3(1.0f, 1.0f, 0.0f);

  box = {};
  box.texture = boxTexture;
  box.lightCoefficients = vec4(0.4f, 0.4f, 0.4f, 0);
  box.lightColor = vec3(0.9f);

  floorTiles = {};
  floorTiles.texture = floorTilesTexture;
  floorTiles.lightCoefficients = vec4(0.3f, 0.6f, 0.9f, 256);
  floorTiles.lightColor = vec3(1.0f, 1.0f, 1.0f);


	// Compile and link shaders here ...
  #if defined(PLATFORM_OSX) || __linux__
	defaultShaderProgram = shader("modelShader.vs", "modelShader.fs");
	phongLightShaderProgram = shader("lightShader.vs", "lightShader.fs");
	shadowShaderProgram = shader("shadowShader.vs", "shadowShader.fs");
  #else
	defaultShaderProgram = shader("../Source/COMP371-Group14-Project/modelShader.vs", "../Source/COMP371-Group14-Project/modelShader.fs");
	phongLightShaderProgram = shader("../Source/COMP371-Group14-Project/lightShader.vs", "../Source/COMP371-Group14-Project/lightShader.fs");
	shadowShaderProgram = shader("../Source/COMP371-Group14-Project/shadowShader.vs", "../Source/COMP371-Group14-Project/shadowShader.fs");
  #endif


#pragma endregion windowSetUp

	// Set up shared buffers for shaders
	glGenBuffers(1, &uboWorldMatrixBlock);
	glBindBuffer(GL_UNIFORM_BUFFER, uboWorldMatrixBlock);
	glBufferData(GL_UNIFORM_BUFFER, 64, NULL, GL_STATIC_DRAW); // allocate 64 bytes of memory
	glBindBuffer(GL_UNIFORM_BUFFER, 0);
	glBindBufferBase(GL_UNIFORM_BUFFER, 0, uboWorldMatrixBlock);  // bind world matrix info to block 0
	//bind shaders' locations for the world matrix block to the same location, block 0:
	unsigned int world_matrix_index_light = glGetUniformBlockIndex(phongLightShaderProgram, "WorldMatrix");
	glUniformBlockBinding(phongLightShaderProgram, world_matrix_index_light, 0);
	unsigned int world_matrix_index_shadow = glGetUniformBlockIndex(shadowShaderProgram, "WorldMatrix");
	glUniformBlockBinding(shadowShaderProgram, world_matrix_index_shadow, 0);

	// Define and upload geometry to the GPU here ...
	//We have a few different cubes since some people did fun colors,
	//And some are centered on origin while others have origin in a corner
	int unitCubeVAO = createUnitCubeVertexArrayObject();
	int rainbowCubeVAO = createVertexArrayObjectU3();
  int texturedCubeVAO = createTextureCubeVertexArrayObject();
	int vao = createVertexArrayObjectR4();
	int gridVAO = createVertexArrayObjectGridLine();
	int gridSquare = createGridSquareVertexArrayObject();
	int xyzVAO = createVertexArrayObjectCoordinateXYZ();
  #if defined(PLATFORM_OSX) || __linux__
	int sphereVAO = createSphereObjectVAO("sphere.obj", sphereVertices);
  #else
	int sphereVAO = createSphereObjectVAO("../Source/COMP371-Group14-Project/sphere.obj", sphereVertices);
  #endif


	//Create hierarchical models
	Model* l9Model = makeL9Model(texturedCubeVAO, sphereVAO);
	Model* l9BottomModel = makeL9BottomModel(texturedCubeVAO);

	Model* i9Model = makeI9Model(texturedCubeVAO, sphereVAO);
	Model* I9BottomModel = makeI9BottomModel(texturedCubeVAO);

	Model* u3Model = makeU3Model(texturedCubeVAO, sphereVAO);
	Model* U3BottomModel = makeU3BottomModel(texturedCubeVAO);

	Model* t9Model = makeT9Model(texturedCubeVAO, sphereVAO);
	Model* t9BottomModel = makeT9BottomModel(texturedCubeVAO);

	Model* c4Model = makeC4Model(texturedCubeVAO, sphereVAO);
	Model* C4BottomModel = makeC4BottomModel(texturedCubeVAO);

  mat4 floorBaseTranslation = translate(mat4(1.0f), vec3(0.0f));
	Model* floorModel = makeFloorModel(gridSquare);

	// For frame time
	float lastFrameTime = glfwGetTime();
	int lastMouseLeftState = GLFW_RELEASE;
	double lastMousePosX, lastMousePosY;
	glfwGetCursorPos(window, &lastMousePosX, &lastMousePosY);

	// Enable Backface culling and depth test
	glEnable(GL_CULL_FACE);
	glEnable(GL_DEPTH_TEST);
    glEnable(GL_TEXTURE_2D);

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
		modelShearingMatrix = {
			1.0f, 0.0f, 0.0f, 0.0f,
			0.0f, 1.0f, modelShearFactor, 0.0f,
			0.0f, 0.0f,  1.0f, 0.0f,
			0.0f, 0.0f, 0.0f, 1.0f
		};

		sharedModelMatrix = modelTranslationMatrix * modelScalingMatrix * modelRotationMatrix;

#pragma region shadowPass1
		//bind and clear the shadow buffer, set viewport to shadowMap dimensions
		glViewport(0, 0, shadowMapWidth, shadowMapHeight);
		glBindFramebuffer(GL_FRAMEBUFFER, shadowMapBuffer);
		glClear(GL_DEPTH_BUFFER_BIT);

		//use the shadow shader, draw all objects
		useShadowShader();

#pragma region buildTransformMatrices
		// Building L9 scalable/translatable/rotateable matrix for individual letter
		l9ModelScalingMatrix = scale(mat4(1.0f), vec3(1.0f, 1.0f, 1.0f) * l9ModelScaleFactor);
		l9ModelRotationMatrix = rotate(mat4(1.0f), radians(l9ModelYRotationAngle), vec3(0.0f, 1.0f, 0.0f)) * rotate(mat4(1.0f), radians(l9ModelXRotationAngle), vec3(1.0f, 0.0f, 0.0f));
		l9ModelTranslationMatrix = translate(mat4(1.0f), l9ModelPosition);
		l9ModelMatrix = l9ModelTranslationMatrix * l9ModelScalingMatrix * l9ModelRotationMatrix;

		// Building I9 scalable/translatable/rotateable matrix for individual letter
		i9ModelScalingMatrix = scale(mat4(1.0f), vec3(1.0f, 1.0f, 1.0f) * i9ModelScaleFactor);
		i9ModelRotationMatrix = rotate(mat4(1.0f), radians(i9ModelYRotationAngle), vec3(0.0f, 1.0f, 0.0f)) * rotate(mat4(1.0f), radians(i9ModelXRotationAngle), vec3(1.0f, 0.0f, 0.0f));
		i9ModelTranslationMatrix = translate(mat4(1.0f), i9ModelPosition);
		i9ModelMatrix = i9ModelTranslationMatrix * i9ModelScalingMatrix * i9ModelRotationMatrix;

		// Building U3 scalable/translatable/rotateable matrix for individual letter
		u3ModelScalingMatrix = scale(mat4(1.0f), vec3(1.0f, 1.0f, 1.0f) * u3ModelScaleFactor);
		u3ModelRotationMatrix = rotate(mat4(1.0f), radians(u3ModelYRotationAngle), vec3(0.0f, 1.0f, 0.0f)) * rotate(mat4(1.0f), radians(u3ModelXRotationAngle), vec3(1.0f, 0.0f, 0.0f));
		u3ModelTranslationMatrix = translate(mat4(1.0f), u3ModelPosition);
		u3ModelMatrix = u3ModelTranslationMatrix * u3ModelScalingMatrix * u3ModelRotationMatrix;

		// Building T9 scalable/translatable/rotateable matrix for individual letter
		t9ModelScalingMatrix = scale(mat4(1.0f), vec3(1.0f, 1.0f, 1.0f) * t9ModelScaleFactor);
		t9ModelRotationMatrix = rotate(mat4(1.0f), radians(t9ModelYRotationAngle), vec3(0.0f, 1.0f, 0.0f)) * rotate(mat4(1.0f), radians(t9ModelXRotationAngle), vec3(1.0f, 0.0f, 0.0f));
		t9ModelTranslationMatrix = translate(mat4(1.0f), t9ModelPosition);
		t9ModelMatrix = t9ModelTranslationMatrix * t9ModelScalingMatrix * t9ModelRotationMatrix;

		// Building C4 scalable/translatable/rotateable matrix for individual letter
		c4ModelScalingMatrix = scale(mat4(1.0f), vec3(1.0f, 1.0f, 1.0f) * c4ModelScaleFactor);
		c4ModelRotationMatrix = rotate(mat4(1.0f), radians(c4ModelYRotationAngle), vec3(0.0f, 1.0f, 0.0f)) * rotate(mat4(1.0f), radians(c4ModelXRotationAngle), vec3(1.0f, 0.0f, 0.0f));
		c4ModelTranslationMatrix = translate(mat4(1.0f), c4ModelPosition);
		c4ModelMatrix = c4ModelTranslationMatrix * c4ModelScalingMatrix * c4ModelRotationMatrix;

		mat4 floorModelMatrix = translate(mat4(1.0f), vec3(0.0f)) * scale(mat4(1.0f), vec3(1.0f)) * rotate(mat4(1.0f), 0.0f, vec3(1.0f));
	
		mat4 L9Matrix = worldOrientationModelMatrix * L9BaseTranslation * sharedModelMatrix * l9ModelMatrix;
		mat4 L9BottomMatrix = worldOrientationModelMatrix * L9BaseTranslation * sharedModelMatrix * l9ModelMatrix * modelShearingMatrix;
		mat4 I9Matrix = worldOrientationModelMatrix * I9BaseTranslation * sharedModelMatrix * i9ModelMatrix;
		mat4 I9BottomMatrix = worldOrientationModelMatrix * I9BaseTranslation * sharedModelMatrix * i9ModelMatrix * modelShearingMatrix;
		mat4 U3Matrix = worldOrientationModelMatrix * U3BaseTranslation * sharedModelMatrix * u3ModelMatrix;
		mat4 U3BottomMatrix = worldOrientationModelMatrix * U3BaseTranslation * sharedModelMatrix * u3ModelMatrix * modelShearingMatrix;
		mat4 T9Matrix = worldOrientationModelMatrix * T9BaseTranslation * sharedModelMatrix * t9ModelMatrix;
		mat4 t9BottomMatrix = worldOrientationModelMatrix * T9BaseTranslation * sharedModelMatrix * t9ModelMatrix * modelShearingMatrix;
		mat4 C4Matrix = worldOrientationModelMatrix * C4BaseTranslation * sharedModelMatrix * c4ModelMatrix;
		mat4 C4BottomMatrix = worldOrientationModelMatrix * C4BaseTranslation * sharedModelMatrix * c4ModelMatrix * modelShearingMatrix;
		mat4 floorMatrix = worldOrientationModelMatrix * floorBaseTranslation * translate(mat4(1.0f), vec3(0.0f));
#pragma endregion

		//Draw scene for the shadow map
		l9Model->draw(L9Matrix, renderingMode, glGetUniformLocation(phongLightShaderProgram, "lightCoefficients"), glGetUniformLocation(phongLightShaderProgram, "lightColor"));
		l9BottomModel->draw(L9BottomMatrix, renderingMode, glGetUniformLocation(phongLightShaderProgram, "lightCoefficients"), glGetUniformLocation(phongLightShaderProgram, "lightColor"));
		i9Model->draw(I9Matrix, renderingMode, glGetUniformLocation(phongLightShaderProgram, "lightCoefficients"), glGetUniformLocation(phongLightShaderProgram, "lightColor"));
		I9BottomModel->draw(I9BottomMatrix, renderingMode, glGetUniformLocation(phongLightShaderProgram, "lightCoefficients"), glGetUniformLocation(phongLightShaderProgram, "lightColor"));
		u3Model->draw(U3Matrix, renderingMode, glGetUniformLocation(phongLightShaderProgram, "lightCoefficients"), glGetUniformLocation(phongLightShaderProgram, "lightColor"));
		U3BottomModel->draw(U3BottomMatrix, renderingMode, glGetUniformLocation(phongLightShaderProgram, "lightCoefficients"), glGetUniformLocation(phongLightShaderProgram, "lightColor"));
		t9Model->draw(T9Matrix, renderingMode, glGetUniformLocation(phongLightShaderProgram, "lightCoefficients"), glGetUniformLocation(phongLightShaderProgram, "lightColor"));
		t9BottomModel->draw(t9BottomMatrix, renderingMode, glGetUniformLocation(phongLightShaderProgram, "lightCoefficients"), glGetUniformLocation(phongLightShaderProgram, "lightColor"));
		c4Model->draw(C4Matrix, renderingMode, glGetUniformLocation(phongLightShaderProgram, "lightCoefficients"), glGetUniformLocation(phongLightShaderProgram, "lightColor"));
		C4BottomModel->draw(C4BottomMatrix, renderingMode, glGetUniformLocation(phongLightShaderProgram, "lightCoefficients"), glGetUniformLocation(phongLightShaderProgram, "lightColor"));
    floorModel->draw(floorMatrix, renderingMode, glGetUniformLocation(phongLightShaderProgram, "lightCoefficients"), glGetUniformLocation(phongLightShaderProgram, "lightColor"));

#pragma endRegion

#pragma region shadowPass2
		//bind and clear the default (screen) framebuffer
		glBindFramebuffer(GL_FRAMEBUFFER, 0);
		glViewport(0, 0, windowWidth, windowHeight);

		// Each frame, reset color of each pixel to glClearColor
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		useLightingShader();
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, shadowMap);

		l9Model->draw(L9Matrix, renderingMode, glGetUniformLocation(phongLightShaderProgram, "lightCoefficients"), glGetUniformLocation(phongLightShaderProgram, "lightColor"));
		l9BottomModel->draw(L9BottomMatrix, renderingMode, glGetUniformLocation(phongLightShaderProgram, "lightCoefficients"), glGetUniformLocation(phongLightShaderProgram, "lightColor"));
		i9Model->draw(I9Matrix, renderingMode, glGetUniformLocation(phongLightShaderProgram, "lightCoefficients"), glGetUniformLocation(phongLightShaderProgram, "lightColor"));
		I9BottomModel->draw(I9BottomMatrix, renderingMode, glGetUniformLocation(phongLightShaderProgram, "lightCoefficients"), glGetUniformLocation(phongLightShaderProgram, "lightColor"));
		u3Model->draw(U3Matrix, renderingMode, glGetUniformLocation(phongLightShaderProgram, "lightCoefficients"), glGetUniformLocation(phongLightShaderProgram, "lightColor"));
		U3BottomModel->draw(U3BottomMatrix, renderingMode, glGetUniformLocation(phongLightShaderProgram, "lightCoefficients"), glGetUniformLocation(phongLightShaderProgram, "lightColor"));
		t9Model->draw(T9Matrix, renderingMode, glGetUniformLocation(phongLightShaderProgram, "lightCoefficients"), glGetUniformLocation(phongLightShaderProgram, "lightColor"));
		t9BottomModel->draw(t9BottomMatrix, renderingMode, glGetUniformLocation(phongLightShaderProgram, "lightCoefficients"), glGetUniformLocation(phongLightShaderProgram, "lightColor"));
		c4Model->draw(C4Matrix, renderingMode, glGetUniformLocation(phongLightShaderProgram, "lightCoefficients"), glGetUniformLocation(phongLightShaderProgram, "lightColor"));
		C4BottomModel->draw(C4BottomMatrix, renderingMode, glGetUniformLocation(phongLightShaderProgram, "lightCoefficients"), glGetUniformLocation(phongLightShaderProgram, "lightColor"));
    floorModel->draw(floorMatrix, renderingMode, glGetUniformLocation(phongLightShaderProgram, "lightCoefficients"), glGetUniformLocation(phongLightShaderProgram, "lightColor"));

#pragma endregion

		useStandardShader();

#pragma region Grid and Coordinate Axis
		/*
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
		*/
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

		if (shearWalking)
		{
			float shearRotationAngle = (int)modelYRotationAngle % 360;

			if (shearDirection > 0)
			{
				modelPosition += getShearMovement(shearRotationAngle) * moveSpeed * dt;
			}
			else if (shearDirection < 0)
			{
				modelPosition -= getShearMovement(shearRotationAngle) * moveSpeed * dt;
			}

			if (shearForward)
			{
				if (modelShearFactor < -0.5)
				{
					shearForward = false;
					modelShearFactor += 0.1;
				}
				else
				{
					modelShearFactor -= 0.1;
				}
			}
			else
			{
				if (modelShearFactor > 0.5)
				{
					shearForward = true;
					modelShearFactor -= 0.1;
				}
				else {
					modelShearFactor += 0.1;
				}

			}
		}
	}

	// Shutdown GLFW
	glfwTerminate();

	return 0;
}

vec3 getShearMovement(float shearRotationAngle) {
	if (shearRotationAngle == 0) {
		return(vec3(0.0f, 0.0f, 1.0f));
	}
	else if (shearRotationAngle == -180 || shearRotationAngle == 180) {
		return(vec3(0.0f, 0.0f, -1.0f));
	}
	else if (shearRotationAngle == -270 || shearRotationAngle == 90) {
		return(vec3(1.0f, 0.0f, 0.0f));
	}
	else if (shearRotationAngle == -90 || shearRotationAngle == 270) {
		return(vec3(-1.0f, 0.0f, 0.0f));
	}
	//first quadrant to left
	else if (shearRotationAngle < 0 && shearRotationAngle > -90 || shearRotationAngle > 270 && shearRotationAngle < 360) {
		return(vec3(-1.0 * abs(sin(shearRotationAngle)), 0.0f, 1.0 * abs(cos(shearRotationAngle))));
	}
	//second quadrant
	else if (shearRotationAngle <= -90 && shearRotationAngle > -180 || shearRotationAngle > 180 && shearRotationAngle < 270) {
		return(vec3(-1.0 * abs(sin(shearRotationAngle)), 0.0f, -1.0 * abs(cos(shearRotationAngle))));
	}
	//third quadrant
	else if (shearRotationAngle < -180 && shearRotationAngle > -270 || shearRotationAngle > 90 && shearRotationAngle < 180) {
		return(vec3(1.0 * abs(cos(shearRotationAngle)), 0.0f, -1.0 * abs(sin(shearRotationAngle))));
	}
	//fourth quadrant
	else if (shearRotationAngle <= -270 && shearRotationAngle > -360 || shearRotationAngle > 0 && shearRotationAngle <= 90) {
		return(vec3(1.0 * abs(sin(shearRotationAngle)), 0.0f, 1.0 * abs(cos(shearRotationAngle))));
	}
}
