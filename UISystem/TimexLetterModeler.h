#pragma once

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/common.hpp>
#include <GL/glew.h>
#include <vector>
#include "Models/Model.h"

using namespace glm;
using namespace std;

static vector<vec3> cornerVertexPositions, cubeVertexPos;
static int unitCubeVAO, pointedCornerVAO;
static unsigned int uboWorldMatrix;
static Material material;

void createPointedCornerVertexArrayObject()
{
	// Nicole modified or added code for Q2, part 1. added normals and UVs
	// Cube model (position, colors, normals, texture coordinates)
	float vertexArray[] = {
		0.0f, 0.0f, 0.5f,	1.0f, 1.0f, 1.0f,	0.0f, 0.0f, 1.0f,	0.5f, 0.25f, // near 
		-0.5f,-0.5f, 0.5f,	1.0f, 1.0f, 1.0f,	0.0f, 0.0f, 1.0f,	1.0f, 0.0f,
		0.5f, -0.5f, 0.5f,	1.0f, 1.0f, 1.0f,	0.0f, 0.0f, 1.0f,	0.0f, 0.0f,

		0.0f, 0.0f,-0.5f,	1.0f, 1.0f, 1.0f,	0.0f, 0.0f, -1.0f,	0.5f, 0.25f, // far 
		0.5f,-0.5f,-0.5f,	1.0f, 1.0f, 1.0f,	0.0f, 0.0f, -1.0f,	1.0f, 0.0f,
		-0.5f,-0.5f,-0.5f,	1.0f, 1.0f, 1.0f,	0.0f, 0.0f, -1.0f,	0.0f, 0.0f,

		0.5f,-0.5f, 0.5f,   1.0f, 1.0f, 1.0f,   0.0f, -1.0f, 0.0f,  1.0f, 1.0f, // bottom 
		-0.5f,-0.5f,-0.5f,  1.0f, 1.0f, 1.0f,   0.0f, -1.0f, 0.0f,  0.0f, 0.0f,
		0.5f,-0.5f,-0.5f,   1.0f, 1.0f, 1.0f,   0.0f, -1.0f, 0.0f,  1.0f, 0.0f,

		0.5f,-0.5f, 0.5f,   1.0f, 1.0f, 1.0f,   0.0f, -1.0f, 0.0f,  1.0f, 1.0f,
		-0.5f,-0.5f, 0.5f,  1.0f, 1.0f, 1.0f,   0.0f, -1.0f, 0.0f,  0.0f, 1.0f,
		-0.5f,-0.5f,-0.5f,  1.0f, 1.0f, 1.0f,   0.0f, -1.0f, 0.0f,  0.0f, 0.0f,

		0.0f, 0.0f, -0.5f,	1.0f, 1.0f, 1.0f, 	1.0f, 1.0f, 0.0f,  1.0f, 0.5f, // right angled face
		0.0f, 0.0f, 0.5f,	1.0f, 1.0f, 1.0f, 	1.0f, 1.0f, 0.0f,  0.0f, 0.5f,
		0.5f,-0.5f, -0.5f,	1.0f, 1.0f, 1.0f, 	1.0f, 1.0f, 0.0f,  1.0f, 0.0f,

		0.0f, 0.0f, 0.5f,	1.0f, 1.0f, 1.0f, 	1.0f, 1.0f, 0.0f,  0.0f, 0.5f,
		0.5f,-0.5f, 0.5f,	1.0f, 1.0f, 1.0f, 	1.0f, 1.0f, 0.0f,  0.0f, 0.0f,
		0.5f,-0.5f,-0.5f,	1.0f, 1.0f, 1.0f, 	1.0f, 1.0f, 0.0f,  1.0f, 0.0f,

		0.0f, 0.0f, -0.5f,	1.0f, 1.0f, 1.0f,  	-1.0f, 1.0f, 0.0f,  0.0f, 0.5f, // left angled face
		-0.5f,-0.5f, -0.5f,	1.0f, 1.0f, 1.0f,  	-1.0f, 1.0f, 0.0f,  0.0f, 0.0f,
		0.0f, 0.0f, 0.5f,	1.0f, 1.0f, 1.0f,  	-1.0f, 1.0f, 0.0f,  1.0f, 0.5f,

		0.0f, 0.0f, 0.5f,	1.0f, 1.0f, 1.0f,  	-1.0f, 1.0f, 0.0f,  1.0f, 0.5f,
		-0.5f,-0.5f,-0.5f,	1.0f, 1.0f, 1.0f,  	-1.0f, 1.0f, 0.0f,  0.0f, 0.0f,
		-0.5f,-0.5f, 0.5f,	1.0f, 1.0f, 1.0f,  	-1.0f, 1.0f, 0.0f,  1.0f, 0.0f
	};

	//Copy the vertex positions, for ray casting later on.
	for (int i = 0; i < sizeof(vertexArray) / sizeof(vertexArray[0]); )
	{
		cornerVertexPositions.push_back(vec3(vertexArray[i], vertexArray[i + 1], vertexArray[i + 2]));
		i += 11; //skip to the start of the next vertex
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
		11 * sizeof(float),	// stride
		(void*)0             // array buffer offset
	);
	glEnableVertexAttribArray(0);


	glVertexAttribPointer(1,                            // attribute 1 matches aColor in Vertex Shader
		3,
		GL_FLOAT,
		GL_FALSE,
		11 * sizeof(float),
		(void*)(3 * sizeof(float))      // color is offseted a vec3 (comes after position)
	);
	glEnableVertexAttribArray(1);

	glVertexAttribPointer(2,                            // attribute 2 matches aNormal in Vertex Shader
		3,
		GL_FLOAT,
		GL_FALSE,
		11 * sizeof(float),
		(void*)(2 * 3 * sizeof(float))
	);
	glEnableVertexAttribArray(2);

	glVertexAttribPointer(3,                            // attribute 3 matches aText in Vertex Shader
		2,
		GL_FLOAT,
		GL_FALSE,
		11 * sizeof(float),
		(void*)(3 * 3 * sizeof(float))
	);
	glEnableVertexAttribArray(3);

	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);

	pointedCornerVAO = vertexArrayObject;
}

#pragma region constants
int numFacesCube = 6, numTrianglesPerFace = 2, numVerticesPerTriangle = 3;
int numTrianglesCube = numFacesCube * numTrianglesPerFace;
int numTrianglesPointedCorner = numTrianglesPerFace * 3 + 2;	//3 square faces + 2 triangular faces

float sizeOfCorner = 0.5f;
float barHeight = 1.0f, barWidth = 0.5f;
float spacing = 0.03f;
float totalWidth = spacing + sizeOfCorner + barHeight + sizeOfCorner + spacing;
float totalHeight = spacing + sizeOfCorner + barHeight + spacing + sizeOfCorner + spacing + barHeight + sizeOfCorner + spacing;
float centeredVerticalEdgeX = (totalWidth / 2.0f) - (barWidth / 2.0f);
float centeredVerticalEdgeY = (totalHeight / 2.0f) - (barHeight / 2.0f) - sizeOfCorner - spacing;
float centeredHorizontalY = (totalHeight / 2.0f) - (barWidth / 2.0f);

//these matrices are used in building the timex block
mat4 timexScalingMatrix = scale(mat4(1.0f), vec3(0.5f, 0.25f, 1.0f));
mat4 timexVerticalRotationMatrix = rotate(mat4(1.0f), radians(90.0f), vec3(1.0f, 0.0f, 0.0f));
mat4 timexHorizontalRotationMatrix = rotate(mat4(1.0f), radians(90.0f), vec3(0.0f, 0.0f, 1.0f)) * rotate(mat4(1.0f), radians(90.0f), vec3(1.0f, 0.0f, 0.0f));

mat4 pointedCornerScalingMatrix = scale(mat4(1.0f), vec3(0.5f, 0.5f, 0.25f));
mat4 pointedCornerHorizontalRotationMatrix = rotate(mat4(1.0f), radians(90.0f), vec3(0.0f, 0.0f, 1.0f));

//Establish the basic positioning of the pointed bar (timex block)
mat4 barTopCornerTranslationMatrix = translate(mat4(1.0f), vec3(0.0f, 0.0f, 0.75f));
mat4 barTopCornerRotationMatrix = rotate(mat4(1.0f), radians(90.0f), vec3(1.0f, 0.0f, 0.0f));
mat4 barBottomCornerTranslationMatrix = translate(mat4(1.0f), vec3(0.0f, 0.0f, -0.75f));
mat4 barBottomCornerRotationMatrix = rotate(mat4(1.0f), radians(-90.0f), vec3(1.0f, 0.0f, 0.0f));

//The placement and angle for the diagonal blocks was found with a bit of math, a bit of trial-and-error
mat4 diagonalBarScaleMatrix = scale(mat4(1.0f), vec3(barHeight * 0.75f, 1.0f, 1.0f));
mat4 diagonalBarTopCornerTranslationMatrix = translate(mat4(1.0f), vec3(0.0f, 0.0f, 0.75f));
mat4 diagonalBarBottomCornerTranslationMatrix = translate(mat4(1.0f), vec3(0.0f, 0.0f, -0.75f));
mat4 diagonalBarBottomCornerRotationMatrix = rotate(mat4(1.0f), radians(180.0f), vec3(0.0f, 0.0f, 1.0f));

// Sizing and placement for the top of the 'M'
mat4 miniBarScaleMatrix = scale(mat4(1.0f), vec3(1.0f, 1.0f, 0.5f * barWidth));
mat4 miniBarTopCornerTranslationMatrix = translate(mat4(1.0f), vec3(0.0f, 0.0f, 0.75f / 2.0f));
mat4 miniBarBottomCornerTranslationMatrix = translate(mat4(1.0f), vec3(0.0f, 0.0f, -0.75f / 2.0f));
mat4 miniBarBottomCornerRotationMatrix = rotate(mat4(1.0f), radians(180.0f), vec3(0.0f, 0.0f, 1.0f));
#pragma endregion

//This is the functiona for drawing a timex block, with the relative placement being defined/determined in drawCharacter()
Model* makeShiftedTimexBlock(mat4 translation, mat4 rotation, mat4 timexRotationMatrix, bool mini = false, bool diagonal = false) {
	mat4 setUpScaling = mat4(1.0f);
	mat4 setUpRotation = mat4(1.0f);
	mat4 setUpTranslation = mat4(1.0f);

	setUpScaling = (mini ? miniBarScaleMatrix : mat4(1.0f));
	setUpScaling = (diagonal ? diagonalBarScaleMatrix : setUpScaling);
	setUpScaling = setUpScaling * timexScalingMatrix;
	Model* modelBar = new Model(unitCubeVAO, cubeVertexPos, uboWorldMatrix, std::vector<Model*>(), setUpTranslation, setUpRotation, setUpScaling, material);

	setUpScaling = mat4(1.0f);
	setUpScaling = (diagonal ? diagonalBarScaleMatrix : setUpScaling);
	setUpScaling *= pointedCornerScalingMatrix;
	setUpTranslation = (mini ? miniBarTopCornerTranslationMatrix : barTopCornerTranslationMatrix);
	setUpTranslation = (diagonal ? diagonalBarTopCornerTranslationMatrix : setUpTranslation);
	setUpRotation = barTopCornerRotationMatrix;
	Model* corner1Bar = new Model(pointedCornerVAO, cornerVertexPositions, uboWorldMatrix, vector<Model*>(), setUpTranslation, setUpRotation, setUpScaling, material);

	setUpTranslation = (mini ? miniBarBottomCornerTranslationMatrix : barBottomCornerTranslationMatrix);
	setUpTranslation = (diagonal ? diagonalBarBottomCornerTranslationMatrix : setUpTranslation);
	setUpRotation = (mini ? miniBarBottomCornerRotationMatrix : barBottomCornerRotationMatrix);
	setUpRotation = (diagonal ? diagonalBarBottomCornerRotationMatrix : setUpRotation);
	setUpRotation = barBottomCornerRotationMatrix;
	Model* corner2Bar = new Model(pointedCornerVAO, cornerVertexPositions, uboWorldMatrix, vector<Model*>(), setUpTranslation, setUpRotation, setUpScaling, material);

	vector<Model*> blockChildren = vector<Model*>();
	blockChildren.push_back(modelBar);
	blockChildren.push_back(corner1Bar);
	blockChildren.push_back(corner2Bar);
	setUpTranslation = translation;
	setUpRotation = timexRotationMatrix;
	Model* barModel = new Model(unitCubeVAO, vector<vec3>(), uboWorldMatrix, blockChildren, setUpTranslation, setUpRotation, mat4(1.0f), material);

	return barModel;
}

/*

The listed digits represent each timex block from left to right in each diagram
Together they cover the needed block specifications for most letters and numbers
To draw some character, simply specify which block indices to draw as 'true'.

	 __  0								_ _		9, 10
	|__| 1,2,3			 \   7			 |		11			/		14
	|__| 4,5,6			  \  8			 |\		12, 13		_ _		15, 16

The diagonals for 7 and 8 are used in N, and the diagonal 13 is used in R.
The placement and size values for these diagonals were tweaked by hand to look good.

*/
Model* makeCharacter(bool blocksToDraw[17], mat4 letterMatrix) {
	vector<Model*> characterChildren = vector<Model*>();

	mat4 translationMatrix;
	if (blocksToDraw[0]) {
		translationMatrix = translate(mat4(1.0f), vec3(0.0f, centeredHorizontalY, 0.0f));
		Model* block = makeShiftedTimexBlock(translationMatrix, pointedCornerHorizontalRotationMatrix, timexHorizontalRotationMatrix);
		characterChildren.push_back(block);
	}

	if (blocksToDraw[1]) {
		translationMatrix = translate(mat4(1.0f), vec3(-centeredVerticalEdgeX, centeredVerticalEdgeY, 0.0f));
		Model* block = makeShiftedTimexBlock(translationMatrix, mat4(1.0f), timexVerticalRotationMatrix);
		characterChildren.push_back(block);
	}

	if (blocksToDraw[2]) {
		translationMatrix = translate(mat4(1.0f), vec3(0.0f, 0.0f, 0.0f));
		Model* block = makeShiftedTimexBlock(translationMatrix, pointedCornerHorizontalRotationMatrix, timexHorizontalRotationMatrix);
		characterChildren.push_back(block);
	}

	if (blocksToDraw[3]) {
		translationMatrix = translate(mat4(1.0f), vec3(centeredVerticalEdgeX, centeredVerticalEdgeY, 0.0f));
		Model* block = makeShiftedTimexBlock(translationMatrix, mat4(1.0f), timexVerticalRotationMatrix);
		characterChildren.push_back(block);
	}

	if (blocksToDraw[4]) {
		translationMatrix = translate(mat4(1.0f), vec3(-centeredVerticalEdgeX, -centeredVerticalEdgeY, 0.0f));
		Model* block = makeShiftedTimexBlock(translationMatrix, mat4(1.0f), timexVerticalRotationMatrix);
		characterChildren.push_back(block);
	}

	if (blocksToDraw[5]) {
		translationMatrix = translate(mat4(1.0f), vec3(0.0f, -centeredHorizontalY, 0.0f));
		Model* block = makeShiftedTimexBlock(translationMatrix, pointedCornerHorizontalRotationMatrix, timexHorizontalRotationMatrix);
		characterChildren.push_back(block);
	}

	if (blocksToDraw[6]) {
		translationMatrix = translate(mat4(1.0f), vec3(centeredVerticalEdgeX, -centeredVerticalEdgeY, 0.0f));
		Model* block = makeShiftedTimexBlock(translationMatrix, mat4(1.0f), timexVerticalRotationMatrix);
		characterChildren.push_back(block);
	}

	if (blocksToDraw[7]) {
		//The placement and angle for the diagonal blocks was found with a bit of math, a bit of trial-and-error
		translationMatrix = translate(mat4(1.0f), vec3(-centeredVerticalEdgeX / 5.0f, centeredHorizontalY / 2.5f, 0.0f))
			* rotate(mat4(1.0f), radians(-65.0f), vec3(0.0f, 0.0f, 1.0f));
		Model* block = makeShiftedTimexBlock(translationMatrix, pointedCornerHorizontalRotationMatrix, timexHorizontalRotationMatrix, false, true);
		characterChildren.push_back(block);
	}

	if (blocksToDraw[8]) {
		translationMatrix = translate(mat4(1.0f), vec3(centeredVerticalEdgeX / 5.0f, -centeredHorizontalY / 2.5f, 0.0f))
			* rotate(mat4(1.0f), radians(-65.0f), vec3(0.0f, 0.0f, 1.0f));
		Model* block = makeShiftedTimexBlock(translationMatrix, pointedCornerHorizontalRotationMatrix, timexHorizontalRotationMatrix, false, true);
		characterChildren.push_back(block);
	}

	if (blocksToDraw[9]) {
		translationMatrix = translate(mat4(1.0f), vec3(-centeredVerticalEdgeX / 2.0f, centeredHorizontalY, 0.0f));
		Model* block = makeShiftedTimexBlock(translationMatrix, pointedCornerHorizontalRotationMatrix, timexHorizontalRotationMatrix, true);
		characterChildren.push_back(block);
	}

	if (blocksToDraw[10]) {
		translationMatrix = translate(mat4(1.0f), vec3(centeredVerticalEdgeX / 2.0f, centeredHorizontalY, 0.0f));
		Model* block = makeShiftedTimexBlock(translationMatrix, pointedCornerHorizontalRotationMatrix, timexHorizontalRotationMatrix, true);
		characterChildren.push_back(block);
	}

	if (blocksToDraw[11]) {
		translationMatrix = translate(mat4(1.0f), vec3(0.0f, centeredVerticalEdgeY, 0.0f));
		Model* block = makeShiftedTimexBlock(translationMatrix, mat4(1.0f), timexVerticalRotationMatrix);
		characterChildren.push_back(block);
	}

	if (blocksToDraw[12]) {
		translationMatrix = translate(mat4(1.0f), vec3(0.0f, -centeredVerticalEdgeY, 0.0f));
		Model* block = makeShiftedTimexBlock(translationMatrix, mat4(1.0f), timexVerticalRotationMatrix);
		characterChildren.push_back(block);
	}

	if (blocksToDraw[13]) {
		translationMatrix = translate(mat4(1.0f), vec3(centeredVerticalEdgeX / 2.0f, -centeredHorizontalY / 1.75f, 0.0f))
			* rotate(mat4(1.0f), radians(-65.0f), vec3(0.0f, 0.0f, 1.0f));
		Model* block = makeShiftedTimexBlock(translationMatrix, pointedCornerHorizontalRotationMatrix, timexHorizontalRotationMatrix, false, true);
		characterChildren.push_back(block);
	}

	if (blocksToDraw[14]) {
		//The placement and angle for the diagonal blocks was found with a bit of math, a bit of trial-and-error
		translationMatrix = translate(mat4(1.0f), vec3(centeredVerticalEdgeX / 5.0f, centeredHorizontalY / 2.5f, 0.0f))
			* rotate(mat4(1.0f), radians(180.0f), vec3(0.0f, 1.0f, 0.0f)) * rotate(mat4(1.0f), radians(-65.0f), vec3(0.0f, 0.0f, 1.0f));
		Model* block = makeShiftedTimexBlock(translationMatrix, pointedCornerHorizontalRotationMatrix, timexHorizontalRotationMatrix, false, true);
		characterChildren.push_back(block);
	}

	if (blocksToDraw[15]) {
		translationMatrix = translate(mat4(1.0f), vec3(-centeredVerticalEdgeX / 2.0f, -centeredHorizontalY, 0.0f));
		Model* block = makeShiftedTimexBlock(translationMatrix, pointedCornerHorizontalRotationMatrix, timexHorizontalRotationMatrix, true);
		characterChildren.push_back(block);
	}

	if (blocksToDraw[16]) {
		translationMatrix = translate(mat4(1.0f), vec3(centeredVerticalEdgeX / 2.0f, -centeredHorizontalY, 0.0f));
		Model* block = makeShiftedTimexBlock(translationMatrix, pointedCornerHorizontalRotationMatrix, timexHorizontalRotationMatrix, true);
		characterChildren.push_back(block);
	}

	Model* characterModel = new Model(unitCubeVAO, vector<vec3>(), uboWorldMatrix, characterChildren, letterMatrix, mat4(1.0f), mat4(1.0f), material);

	return characterModel;
}

Model* makeFlashlightUIModel(Material mat, int unitCube, vector<vec3> cubeVertexPosTemp, int uboWorldMatrixBlock) {
	material = mat;
	cubeVertexPos = cubeVertexPosTemp;
	unitCubeVAO = unitCube;
	uboWorldMatrix = uboWorldMatrixBlock;

	mat4 setUpScaling = mat4(1.0f);
	mat4 setUpRotation = mat4(1.0f);
	mat4 setUpTranslation = mat4(1.0f);

	// F - TOGGLE FLASHLIGHT
	// 
	float spacing = 3.0f;
	int index = -10;
	vector<Model*> entireModelChildren = vector<Model*>();

	//F
	setUpTranslation = translate(mat4(1.0f), vec3(index++ * spacing, 0.0f, 0.0f)); 
	bool fValues[17] = { true, true, true, false, true, false, false, false, false, false, false, false, false, false, false, false, false };
	entireModelChildren.push_back(makeCharacter(fValues, setUpTranslation));

	//space
	setUpTranslation = translate(mat4(1.0f), vec3(index++ * spacing, 0.0f, 0.0f));

	//dash -
	setUpTranslation = translate(mat4(1.0f), vec3(index++ * spacing, 0.0f, 0.0f));
	bool dashValues[17] = { false, false, true, false, false, false, false, false, false, false, false, false, false, false, false, false, false };
	entireModelChildren.push_back(makeCharacter(dashValues, setUpTranslation));

	//space
	setUpTranslation = translate(mat4(1.0f), vec3(index++ * spacing, 0.0f, 0.0f));

	//T
	setUpTranslation = translate(mat4(1.0f), vec3(index++ * spacing, 0.0f, 0.0f));
	bool tValues[17] = { false, false, false, false, false, false, false, false, false, true, true, true, true, false, false, false, false };
	entireModelChildren.push_back(makeCharacter(tValues, setUpTranslation));

	//O
	setUpTranslation = translate(mat4(1.0f), vec3(index++ * spacing, 0.0f, 0.0f));
	bool oValues[17] = { true, true, false, true, true, true, true, false, false, false, false, false, false, false, false, false, false };
	entireModelChildren.push_back(makeCharacter(oValues, setUpTranslation));

	//G
	setUpTranslation = translate(mat4(1.0f), vec3(index++ * spacing, 0.0f, 0.0f));
	bool gValues[17] = { true, true, false, false, true, true, true, false, false, false, false, false, false, false, false, false, false };
	entireModelChildren.push_back(makeCharacter(gValues, setUpTranslation));

	//G2
	setUpTranslation = translate(mat4(1.0f), vec3(index++ * spacing, 0.0f, 0.0f));
	entireModelChildren.push_back(makeCharacter(gValues, setUpTranslation));

	//L
	setUpTranslation = translate(mat4(1.0f), vec3(index++ * spacing, 0.0f, 0.0f));
	bool lValues[17] = { false, true, false, false, true, true, false, false, false, false, false, false, false, false, false, false, false };
	entireModelChildren.push_back(makeCharacter(lValues, setUpTranslation));

	//E
	setUpTranslation = translate(mat4(1.0f), vec3(index++ * spacing, 0.0f, 0.0f));
	bool eValues[17] = { true, true, true, false, true, true, false, false, false, false, false, false, false, false, false, false, false };
	entireModelChildren.push_back(makeCharacter(eValues, setUpTranslation));

	//space
	setUpTranslation = translate(mat4(1.0f), vec3(index++ * spacing, 0.0f, 0.0f));

	//F2
	setUpTranslation = translate(mat4(1.0f), vec3(index++ * spacing, 0.0f, 0.0f));
	entireModelChildren.push_back(makeCharacter(fValues, setUpTranslation));

	//L2
	setUpTranslation = translate(mat4(1.0f), vec3(index++ * spacing, 0.0f, 0.0f));
	entireModelChildren.push_back(makeCharacter(lValues, setUpTranslation));

	//A
	setUpTranslation = translate(mat4(1.0f), vec3(index++ * spacing, 0.0f, 0.0f));
	bool aValues[17] = { true, true, true, true, true, false, true, false, false, false, false, false, false, false, false, false, false };
	entireModelChildren.push_back(makeCharacter(aValues, setUpTranslation));

	//S
	setUpTranslation = translate(mat4(1.0f), vec3(index++ * spacing, 0.0f, 0.0f));
	bool sValues[17] = { true, true, true, false, false, true, true, false, false, false, false, false, false, false, false, false, false };
	entireModelChildren.push_back(makeCharacter(sValues, setUpTranslation));

	//H
	setUpTranslation = translate(mat4(1.0f), vec3(index++ * spacing, 0.0f, 0.0f));
	bool hValues[17] = { false, true, true, true, true, false, true, false, false, false, false, false, false, false, false, false, false };
	entireModelChildren.push_back(makeCharacter(hValues, setUpTranslation));

	//L3
	setUpTranslation = translate(mat4(1.0f), vec3(index++ * spacing, 0.0f, 0.0f));
	entireModelChildren.push_back(makeCharacter(lValues, setUpTranslation));

	//I
	setUpTranslation = translate(mat4(1.0f), vec3(index++ * spacing, 0.0f, 0.0f));
	bool iValues[17] = { false, false, false, false, false, false, false, false, false, true, true, true, true, false, false, true, true };
	entireModelChildren.push_back(makeCharacter(iValues, setUpTranslation));

	//G3
	setUpTranslation = translate(mat4(1.0f), vec3(index++ * spacing, 0.0f, 0.0f));
	entireModelChildren.push_back(makeCharacter(gValues, setUpTranslation));

	//H2
	setUpTranslation = translate(mat4(1.0f), vec3(index++ * spacing, 0.0f, 0.0f));
	entireModelChildren.push_back(makeCharacter(hValues, setUpTranslation));

	//T
	setUpTranslation = translate(mat4(1.0f), vec3(index++ * spacing, 0.0f, 0.0f));
	entireModelChildren.push_back(makeCharacter(tValues, setUpTranslation));


	mat4 translation = translate(mat4(1.0f), vec3(0.5f, 0.5f, 0.0f));
	Model* entireModel = new Model(0, vector<vec3>(), uboWorldMatrix, entireModelChildren, translation, mat4(1.0f), mat4(1.0f));

	return entireModel;
}

Model* makeMovementUIModel() {
	mat4 setUpScaling = mat4(1.0f);
	mat4 setUpRotation = mat4(1.0f);
	mat4 setUpTranslation = mat4(1.0f);

	// WASD - Move
	// 
	float spacing = 3.0f;
	int index = -10;
	vector<Model*> entireModelChildren = vector<Model*>();

	//W
	setUpTranslation = translate(mat4(1.0f), vec3(index++ * spacing, 0.0f, 0.0f));
	bool wValues[17] = { false, true, false, true, true, true, true, false, false, false, false, false, true, false, false, false, false };
	entireModelChildren.push_back(makeCharacter(wValues, setUpTranslation));

	//A
	setUpTranslation = translate(mat4(1.0f), vec3(index++ * spacing, 0.0f, 0.0f));
	bool aValues[17] = { true, true, true, true, true, false, true, false, false, false, false, false, false, false, false, false, false };
	entireModelChildren.push_back(makeCharacter(aValues, setUpTranslation));

	//S
	setUpTranslation = translate(mat4(1.0f), vec3(index++ * spacing, 0.0f, 0.0f));
	bool sValues[17] = { true, true, true, false, false, true, true, false, false, false, false, false, false, false, false, false, false };
	entireModelChildren.push_back(makeCharacter(sValues, setUpTranslation));

	//D (same as O)
	setUpTranslation = translate(mat4(1.0f), vec3(index++ * spacing, 0.0f, 0.0f));
	bool oValues[17] = { true, true, false, true, true, true, true, false, false, false, false, false, false, false, false, false, false };
	entireModelChildren.push_back(makeCharacter(oValues, setUpTranslation));

	//space
	setUpTranslation = translate(mat4(1.0f), vec3(index++ * spacing, 0.0f, 0.0f));

	//dash -
	setUpTranslation = translate(mat4(1.0f), vec3(index++ * spacing, 0.0f, 0.0f));
	bool dashValues[17] = { false, false, true, false, false, false, false, false, false, false, false, false, false, false, false, false, false };
	entireModelChildren.push_back(makeCharacter(dashValues, setUpTranslation));

	//space
	setUpTranslation = translate(mat4(1.0f), vec3(index++ * spacing, 0.0f, 0.0f));

	//M
	setUpTranslation = translate(mat4(1.0f), vec3(index++ * spacing, 0.0f, 0.0f));
	bool mValues[17] = { false, true, false, true, true, false, true, false, false, true, true, true, true, false, false, false, false };
	entireModelChildren.push_back(makeCharacter(mValues, setUpTranslation));
	
	//O
	setUpTranslation = translate(mat4(1.0f), vec3(index++ * spacing, 0.0f, 0.0f));
	entireModelChildren.push_back(makeCharacter(oValues, setUpTranslation));

	//V
	setUpTranslation = translate(mat4(1.0f), vec3(index++ * spacing, 0.0f, 0.0f));
	bool vValues[17] = { false, true, false, true, true, true, true, false, false, false, false, false, false, false, false, false, false };
	entireModelChildren.push_back(makeCharacter(vValues, setUpTranslation));

	//E
	setUpTranslation = translate(mat4(1.0f), vec3(index++ * spacing, 0.0f, 0.0f));
	bool eValues[17] = { true, true, true, false, true, true, false, false, false, false, false, false, false, false, false, false, false };
	entireModelChildren.push_back(makeCharacter(eValues, setUpTranslation));

	mat4 translation = translate(mat4(1.0f), vec3(0.5f, 0.5f, 0.0f));
	Model* entireModel = new Model(0, vector<vec3>(), uboWorldMatrix, entireModelChildren, translation, mat4(1.0f), mat4(1.0f));

	return entireModel;
}

Model* makeControlsUIModel() {
	mat4 setUpScaling = mat4(1.0f);
	mat4 setUpRotation = mat4(1.0f);
	mat4 setUpTranslation = mat4(1.0f);

	// Controls =
	// 
	float spacing = 3.0f;
	int index = -10;
	vector<Model*> entireModelChildren = vector<Model*>();

	//C
	setUpTranslation = translate(mat4(1.0f), vec3(index++ * spacing, 0.0f, 0.0f));
	bool cValues[17] = { true, true, false, false, true, true, false, false, false, false, false, false, false, false, false, false, false };
	entireModelChildren.push_back(makeCharacter(cValues, setUpTranslation));

	//O
	setUpTranslation = translate(mat4(1.0f), vec3(index++ * spacing, 0.0f, 0.0f));
	bool oValues[17] = { true, true, false, true, true, true, true, false, false, false, false, false, false, false, false, false, false };
	entireModelChildren.push_back(makeCharacter(oValues, setUpTranslation));

	//N
	setUpTranslation = translate(mat4(1.0f), vec3(index++ * spacing, 0.0f, 0.0f));
	bool nValues[17] = { false, true, false, true, true, false, true, true, true, false, false, false, false, false, false, false, false };
	entireModelChildren.push_back(makeCharacter(nValues, setUpTranslation));

	//T
	setUpTranslation = translate(mat4(1.0f), vec3(index++ * spacing, 0.0f, 0.0f));
	bool tValues[17] = { false, false, false, false, false, false, false, false, false, true, true, true, true, false, false, false, false };
	entireModelChildren.push_back(makeCharacter(tValues, setUpTranslation));

	//R
	setUpTranslation = translate(mat4(1.0f), vec3(index++ * spacing, 0.0f, 0.0f));
	bool rValues[17] = { true, true, true, true, true, false, false, false, false, false, false, false, false, true, false,  false,  false};
	entireModelChildren.push_back(makeCharacter(rValues, setUpTranslation));

	//O2
	setUpTranslation = translate(mat4(1.0f), vec3(index++ * spacing, 0.0f, 0.0f));
	entireModelChildren.push_back(makeCharacter(oValues, setUpTranslation));

	//L
	setUpTranslation = translate(mat4(1.0f), vec3(index++ * spacing, 0.0f, 0.0f));
	bool lValues[17] = { false, true, false, false, true, true, false, false, false, false, false, false, false, false, false, false, false };
	entireModelChildren.push_back(makeCharacter(lValues, setUpTranslation));

	//S
	setUpTranslation = translate(mat4(1.0f), vec3(index++ * spacing, 0.0f, 0.0f));
	bool sValues[17] = { true, true, true, false, false, true, true, false, false, false, false, false, false, false, false, false, false };
	entireModelChildren.push_back(makeCharacter(sValues, setUpTranslation));
	   
	//space
	//setUpTranslation = translate(mat4(1.0f), vec3(index++ * spacing, 0.0f, 0.0f));

	//equals =
	setUpTranslation = translate(mat4(1.0f), vec3(index++ * spacing, 0.0f, 0.0f));
	bool dashValues[17] = { false, false, false, false, false, false, false, false, false, false, true, false, false, false, false, false, true };
	entireModelChildren.push_back(makeCharacter(dashValues, setUpTranslation));

	
	mat4 translation = translate(mat4(1.0f), vec3(0.5f, 0.5f, 0.0f));
	Model* entireModel = new Model(0, vector<vec3>(), uboWorldMatrix, entireModelChildren, translation, mat4(1.0f), mat4(1.0f));

	return entireModel;
}

