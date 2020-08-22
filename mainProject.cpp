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
#include "Models/Model.h"
#include "Models/Skybox.h"
#include "LightingSystem/LightSource.h"
#include "LightingSystem/LightSourceManager.h"
#include "CollisionSystem/RaycastCollisions.h"
#include "UISystem/TimexLetterModeler.h"
#include "Textures/texture.h"
#include "Terrain/Terrain.h"
#include "Terrain/City.h"
#include <time.h>
#include <algorithm>
#include <list>
#include <vector>
#include <random>
#include "Objects/OBJloader.h"  //For loading .obj files using a polygon list format


//define namespaces for glm and c++ std
using namespace glm;
using namespace std;

int windowWidth = 1024, windowHeight = 764;

GLuint woodTexture, skyTexture, windowTexture, brownTexture, beigeTexture, blackTexture, redTexture, blueTexture, 
purpleTexture, yellowTexture, grayTexture, whiteTexture, cementTexture, marbleTexture;

float dt;

std::vector<std::pair<Terrain*, City*>> worldMap;
std::vector<Model*> floorModels;
Material wood, cement, marble, sky, windowFrame, brown, beige, black, red, blue, purple, yellow, gray, white;

int sphereVAO;
int carVAO;
int lampVAO;
int garbageVAO;
int hydrantVAO;

void setRandomizedPositionScale(mat4& modelMatrix, Terrain terrain);
Model* makeBuilding1Model(int vao, Terrain terrain, glm::vec3 scaleXYZ, float numOfFloors);
Model* makeBuilding2Model(int vao, Terrain terrain, glm::vec3 scaleXYZ, float numOfFloors);
Model* makeBuilding3Model(int vao, Terrain terrain, glm::vec3 scaleXYZ, float numOfFloors);
Model* makeBuilding4Model(int vao, Terrain terrain, glm::vec3 scaleXYZ, float numOfFloors);
Model* makeBuilding5Model(int vao, Terrain terrain, glm::vec3 scaleXYZ, float numOfFloors);


float getRandomNumber(int lowerBound, int upperBound) {
	int diff = upperBound + 1 - lowerBound;

    int result = rand() % diff + lowerBound;

	return result;
}

Model* makeCarModel(int vao);

Model* makeLampModel(int vao);

Model* makeHydrantModel(int vao);

Model* makeGarbageModel(int vao);

std::pair<int, Material> getRandomMaterial()
{
	int num = getRandomNumber(1, 7);

	switch (num) {
	case 1:
		return std::make_pair(1, brown);
	case 2:
		return std::make_pair(2, beige);
	case 3:
		return std::make_pair(3, black);
	case 4:
		return std::make_pair(4, red);
	case 5:
		return std::make_pair(5, blue);
	case 6:
		return std::make_pair(6, purple);
	case 7:
		return std::make_pair(7, yellow);
	}
}

#pragma region unitModels
vector<vec3> cubeVertexPositions;
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

	//Copy the vertex positions, for ray casting later on.
	for (int i = 0; i < sizeof(vertexArray) / sizeof(vertexArray[0]); )
	{
		cubeVertexPositions.push_back(vec3(vertexArray[i], vertexArray[i + 1], vertexArray[i + 2]));
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
		11 * sizeof(float), // stride - each vertex contain 3 vec3 (position, color, normal)
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

vector<vec3> sphereVertices;
GLuint createSphereObjectVAO(string path) {
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

	sphereVertices = vertices;

	return VAO;
}

vector<vec3> carVertices, lampVertices, garbageVertices, hydrantVertices;
GLuint createObjectVAO(string path, vector<vec3>& numOfVertices) {
	vector<vec3> vertices;
	vector<vec3> colors;
	vector<vec3> normals;
	vector<vec2> UVs;

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

	numOfVertices = vertices;

	return VAO;
}
#pragma endregion

#pragma region cameraInput
// Camera parameters
float cameraHorizontalAngle = 90.0f;
float cameraVerticalAngle = -25.0f;
const float cameraAngularSpeed = 60.0f;

float viewDistance = 2048.0f;

// Camera parameters for view transform
vec3 cameraLookAt(0.0f, 0.0f, 0.0f);
vec3 cameraUp(0.0f, 1.0f, 0.0f);
vec3 cameraPosition(0.0f, 40.0f, 30.0f);
float currentCamStrafingMovement = 0, currentCamFacingMovement = 0.0f;

// Set projection matrix for shader, this won't change
mat4 projectionMatrix = perspective(70.0f, // field of view in degrees
(float)windowWidth / windowHeight,  // aspect ratio
0.01f, viewDistance);   // near and far (near > 0)

// Set initial view matrix
mat4 viewMatrix = lookAt(cameraPosition,  // eye
	cameraPosition + cameraLookAt,  // center
	cameraUp); // up

void handleCameraPositionInputs(GLFWwindow* window) {
	float walkSpeed = 150.0f;
	if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
	{
		currentCamStrafingMovement -= walkSpeed * dt;
	}
	if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
	{
		currentCamStrafingMovement += walkSpeed * dt;
	}
	if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
	{
		currentCamFacingMovement += walkSpeed * dt;
	}
	if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
	{
		currentCamFacingMovement -= walkSpeed * dt;
	}
    if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS)
    {
        cameraPosition.y += walkSpeed * dt;
    }
    if (glfwGetKey(window, GLFW_KEY_LEFT_CONTROL) == GLFW_PRESS)
    {
        cameraPosition.y -= walkSpeed * dt;
    }
}
#pragma endregion

#pragma region renderingInput
//storing the redering mode in a variable 
int renderingMode = GL_TRIANGLES;
bool isFlashLightOn = true;
static bool FPressed = false;
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

	if (glfwGetKey(window, GLFW_KEY_F) == GLFW_PRESS && FPressed == false)  //toggle flashlight
	{
		isFlashLightOn = !isFlashLightOn;
		FPressed = true;
	}
	if (glfwGetKey(window, GLFW_KEY_F) == GLFW_RELEASE && FPressed == true)
	{
		FPressed = false;
	}
}

void handleExitInput(GLFWwindow* window) {
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
		glfwSetWindowShouldClose(window, true);
}

// reference https://www.glfw.org/docs/latest/group__window.html#gae49ee6ebc03fa2da024b89943a331355
void windowResizeCallback(GLFWwindow* window, int width, int height)
{
	windowHeight = height;
	windowWidth = width;
}
#pragma endregion

#pragma region makeModels
unsigned int uboWorldMatrixBlock;
unsigned int uboDepthVPBlock;
unsigned int uboLightInfoBlock;
int texturedCubeVAO;

Model* makeL9BottomModel() {

	// Draw L9 using hierarchical modeling, start at the lowest model(s) in the hierarchy
	glm::mat4 setUpScaling = mat4(1.0f);
	glm::mat4 setUpRotation = mat4(1.0f);
	glm::mat4 setUpTranslation = mat4(1.0f);

	// Creating bottom-part of the letter L
	setUpTranslation = translate(glm::mat4(1.0f), glm::vec3(-1.0f, -1.0f, 0.0f));
	setUpScaling = scale(glm::mat4(1.0f), glm::vec3(1.0f, 2.5f, 1.0f));
	Model* modelLbottomBar = new Model(texturedCubeVAO, cubeVertexPositions, uboWorldMatrixBlock, vector<Model*>(), setUpTranslation, mat4(1.0f), setUpScaling, cement);

	// Creating right-part of the letter L
	setUpScaling = scale(glm::mat4(1.0f), glm::vec3(2.0f, 1.0f, 1.0f));
	setUpTranslation = translate(glm::mat4(1.0f), glm::vec3(0.5f, -1.75f, 0.0f));
	Model* modelLverticalBar = new Model(texturedCubeVAO, cubeVertexPositions, uboWorldMatrixBlock, vector<Model*>(), setUpTranslation, mat4(1.0f), setUpScaling, cement);

	// Setting up the letter L
	vector<Model*> LChildren = vector<Model*>();
	LChildren.push_back(modelLbottomBar);
	LChildren.push_back(modelLverticalBar);
	//The pieces of the L are placed such that the entire L is centered at origin on all axes
	//We can then very simply manipulate this modelL to transform the entire L
	//for example, to scoot the L left to make room for the number, making the entire L9 centered.
	setUpTranslation = translate(glm::mat4(1.0f), glm::vec3(-2.0f, 0.0f, 0.0f));
	Model* modelL = new Model(texturedCubeVAO, vector<vec3>(), uboWorldMatrixBlock, LChildren, setUpTranslation, mat4(1.0f), mat4(1.0f));

	// Creating right-part of the number 9
	setUpTranslation = translate(glm::mat4(1.0f), glm::vec3(1.5f, -1.0f, 0.0f));
	setUpScaling = scale(glm::mat4(1.0f), glm::vec3(1.0f, 2.5f, 1.0f));
	Model* model9bottom = new Model(texturedCubeVAO, cubeVertexPositions, uboWorldMatrixBlock, vector<Model*>(), setUpTranslation, mat4(1.0f), setUpScaling, cement);

	// Setting up the number 9
	vector<Model*> nineChildren = vector<Model*>();
	nineChildren.push_back(model9bottom);
	setUpTranslation = translate(glm::mat4(1.0f), glm::vec3(1.5f, 0.0f, 0.0f));
	Model* model9 = new Model(texturedCubeVAO, vector<vec3>(), uboWorldMatrixBlock, nineChildren, setUpTranslation, mat4(1.0f), mat4(1.0f));


	// Setting up the entire L9
	// This will be the root, and will be provided with the current world and sharedModel matrices in draw() from main()
	vector<Model*> L9Children = vector<Model*>();
	L9Children.push_back(modelL);
	L9Children.push_back(model9);
	Model* modelL9 = new Model(texturedCubeVAO, vector<vec3>(), uboWorldMatrixBlock, L9Children, mat4(1.0f), mat4(1.0f), mat4(1.0f));

	// Setting up the entire model
	// This will be the root, and will be provided with the current world and sharedModel matrices in draw() from main()
	vector<Model*> entireModelChildren = vector<Model*>();
	entireModelChildren.push_back(modelL9);
	Model* entireModel = new Model(texturedCubeVAO, vector<vec3>(), uboWorldMatrixBlock, entireModelChildren, mat4(1.0f), mat4(1.0f), mat4(1.0f));

	return entireModel;

}

Model* makeL9Model() {
	mat4 setUpScaling = mat4(1.0f);
	mat4 setUpRotation = mat4(1.0f);
	mat4 setUpTranslation = mat4(1.0f);

	// Creating left-part of the letter L
	setUpTranslation = translate(glm::mat4(1.0f), glm::vec3(-1.0f, 1.5f, 0.0f));
	setUpScaling = scale(glm::mat4(1.0f), glm::vec3(1.0f, 2.5f, 1.0f));
	Model* modelLbottomBar = new Model(texturedCubeVAO, cubeVertexPositions, uboWorldMatrixBlock, vector<Model*>(), setUpTranslation, mat4(1.0f), setUpScaling, cement);

	// Setting up the letter L
	vector<Model*> LChildren = vector<Model*>();
	LChildren.push_back(modelLbottomBar);
	//The pieces of the L are placed such that the entire L is centered at origin on all axes
	//We can then very simply manipulate this modelL to transform the entire L
	//for example, to scoot the L left to make room for the number, making the entire L9 centered.
	setUpTranslation = translate(mat4(1.0f), vec3(-2.0f, 0.0f, 0.0f));
	Model* modelL = new Model(texturedCubeVAO, vector<vec3>(), uboWorldMatrixBlock, LChildren, setUpTranslation, mat4(1.0f), mat4(1.0f), cement);


	// Creating top-part of the number 9
	setUpTranslation = translate(mat4(1.0f), vec3(0.0f, 2.0f, 0.0f));
	setUpScaling = scale(mat4(1.0f), vec3(2.0f, 1.0f, 1.0f));
	Model* model9top = new Model(texturedCubeVAO, cubeVertexPositions, uboWorldMatrixBlock, vector<Model*>(), setUpTranslation, mat4(1.0f), setUpScaling, cement);

	// Creating right-part of the number 9
	setUpTranslation = translate(glm::mat4(1.0f), glm::vec3(1.5f, 1.25f, 0.0f));
	setUpScaling = scale(glm::mat4(1.0f), glm::vec3(1.0f, 2.5f, 1.0f));
	Model* model9right = new Model(texturedCubeVAO, cubeVertexPositions, uboWorldMatrixBlock, vector<Model*>(), setUpTranslation, mat4(1.0f), setUpScaling, cement);

	// Creating left-part of the number 9
	setUpTranslation = translate(mat4(1.0f), vec3(-0.5f, 0.5f, 0.0f));
	setUpScaling = scale(mat4(1.0f), vec3(1.0f, 2.0f, 1.0f));
	Model* model9left = new Model(texturedCubeVAO, cubeVertexPositions, uboWorldMatrixBlock, vector<Model*>(), setUpTranslation, mat4(1.0f), setUpScaling, cement);

	// Creating bottom-part of the number 9
	setUpTranslation = translate(mat4(1.0f), vec3(0.5f, 0.0f, 0.0f));
	Model* model9bottom = new Model(texturedCubeVAO, cubeVertexPositions, uboWorldMatrixBlock, vector<Model*>(), setUpTranslation, mat4(1.0f), mat4(1.0f), cement);

	// Setting up the number 9
	vector<Model*> nineChildren = vector<Model*>();
	nineChildren.push_back(model9top);
	nineChildren.push_back(model9right);
	nineChildren.push_back(model9left);
	nineChildren.push_back(model9bottom);
	setUpTranslation = translate(mat4(1.0f), vec3(1.5f, 0.0f, 0.0f));
	Model* model9 = new Model(texturedCubeVAO, vector<vec3>(), uboWorldMatrixBlock, nineChildren, setUpTranslation, mat4(1.0f), mat4(1.0f), cement);


	// Setting up the entire L9
	// This will be the root, and will be provided with the current world and sharedModel matrices in draw() from main()
	vector<Model*> L9Children = vector<Model*>();
	L9Children.push_back(modelL);
	L9Children.push_back(model9);
	L9Children.push_back(makeL9BottomModel());
	Model* modelL9 = new Model(texturedCubeVAO, vector<vec3>(), uboWorldMatrixBlock, L9Children, mat4(1.0f), mat4(1.0f), mat4(1.0f));

	// Setting up the entire model
	// This will be the root, and will be provided with the current world and sharedModel matrices in draw() from main()
	vector<Model*> entireModelChildren = vector<Model*>();
	entireModelChildren.push_back(modelL9);
	Model* entireModel = new Model(texturedCubeVAO, vector<vec3>(), uboWorldMatrixBlock, entireModelChildren, mat4(1.0f), mat4(1.0f), mat4(1.0f));

	return entireModel;
}

Model* makeI9BottomModel() {
	// Draw I9 using hierarchical modeling, start at the lowest model(s) in the hierarchy
	glm::mat4 setUpScaling = mat4(1.0f);
	glm::mat4 setUpRotation = mat4(1.0f);
	glm::mat4 setUpTranslation = mat4(1.0f);

	// Creating middlet-part of the letter I
	setUpTranslation = translate(glm::mat4(1.0f), glm::vec3(-2.0f, 1.0f, 0.0f));
	setUpScaling = scale(glm::mat4(1.0f), glm::vec3(1.0f, 2.5f, 1.0f));
	Model* modelImiddleBar = new Model(texturedCubeVAO, cubeVertexPositions, uboWorldMatrixBlock, vector<Model*>(), setUpTranslation, mat4(1.0f), setUpScaling, marble);

	// Creating bottom-part of the letter I
	setUpTranslation = translate(glm::mat4(1.0f), glm::vec3(-2.0f, 0.0f, 0.0f));
	setUpScaling = scale(glm::mat4(1.0f), glm::vec3(4.0f, 1.0f, 1.0f));
	Model* modelIbottomBar = new Model(texturedCubeVAO, cubeVertexPositions, uboWorldMatrixBlock, vector<Model*>(), setUpTranslation, mat4(1.0f), setUpScaling, marble);

	// Setting up the letter I
	vector<Model*> IChildren = vector<Model*>();
	IChildren.push_back(modelImiddleBar);
	IChildren.push_back(modelIbottomBar);
	//The pieces of the I are placed such that the entire I is centered at origin on all axes
	//We can then very simply manipulate this modelI to transform the entire I
	//for example, to scoot the I left to make room for the number, making the entire I9 centered.
	setUpTranslation = translate(glm::mat4(1.0f), glm::vec3(-1.0f, -2.0f, 0.0f));
	Model* modelI = new Model(texturedCubeVAO, vector<vec3>(), uboWorldMatrixBlock, IChildren, setUpTranslation, mat4(1.0f), mat4(1.0f));


	// Creating right-part of the number 9
	setUpTranslation = translate(glm::mat4(1.0f), glm::vec3(5.0f, 0.5f, 0.0f));
	setUpScaling = scale(glm::mat4(1.0f), glm::vec3(1.0f, 2.5f, 1.0f));
	Model* model9right = new Model(texturedCubeVAO, cubeVertexPositions, uboWorldMatrixBlock, vector<Model*>(), setUpTranslation, mat4(1.0f), setUpScaling, marble);


	// Setting up the number 9
	vector<Model*> nineChildren = vector<Model*>();
	nineChildren.push_back(model9right);
	setUpTranslation = translate(glm::mat4(1.0f), glm::vec3(-1.0f, -2.0f, 0.0f));
	Model* model9 = new Model(texturedCubeVAO, vector<vec3>(), uboWorldMatrixBlock, nineChildren, setUpTranslation, mat4(1.0f), mat4(1.0f));


	// Setting up the entire I9
	// This will be the root, and will be provided with the current world and sharedModel matrices in draw() from main()
	vector<Model*> I9Children = vector<Model*>();
	I9Children.push_back(modelI);
	I9Children.push_back(model9);
	Model* modelI9 = new Model(texturedCubeVAO, vector<vec3>(), uboWorldMatrixBlock, I9Children, mat4(1.0f), mat4(1.0f), mat4(1.0f));

	// Setting up the entire model
	// This will be the root, and will be provided with the current world and sharedModel matrices in draw() from main()
	vector<Model*> entireModelChildren = vector<Model*>();
	entireModelChildren.push_back(modelI9);
	Model* entireModel = new Model(texturedCubeVAO, vector<vec3>(), uboWorldMatrixBlock, entireModelChildren, mat4(1.0f), mat4(1.0f), mat4(1.0f));

	return entireModel;
}

Model* makeI9Model() {
	// Draw I9 using hierarchical modeling, start at the lowest model(s) in the hierarchy
	mat4 setUpScaling = mat4(1.0f);
	mat4 setUpRotation = mat4(1.0f);
	mat4 setUpTranslation = mat4(1.0f);

	// Creating top-part of the letter I
	setUpTranslation = translate(mat4(1.0f), vec3(-2.0f, 4.5f, 0.0f));
	setUpScaling = scale(mat4(1.0f), vec3(4.0f, 1.0f, 1.0f));
	Model* modelItopBar = new Model(texturedCubeVAO, cubeVertexPositions, uboWorldMatrixBlock, vector<Model*>(), setUpTranslation, mat4(1.0f), setUpScaling, marble);


	// Creating middlet-part of the letter I
	setUpTranslation = translate(glm::mat4(1.0f), glm::vec3(-2.0f, 2.75f, 0.0f));
	setUpScaling = scale(glm::mat4(1.0f), glm::vec3(1.0f, 2.5f, 1.0f));
	Model* modelImiddleBar = new Model(texturedCubeVAO, cubeVertexPositions, uboWorldMatrixBlock, vector<Model*>(), setUpTranslation, mat4(1.0f), setUpScaling, marble);

	// Setting up the letter I
	vector<Model*> IChildren = vector<Model*>();
	IChildren.push_back(modelItopBar);
	IChildren.push_back(modelImiddleBar);
	//The pieces of the I are placed such that the entire I is centered at origin on all axes
	//We can then very simply manipulate this modelI to transform the entire I
	//for example, to scoot the I left to make room for the number, making the entire I9 centered.
	setUpTranslation = translate(mat4(1.0f), vec3(-1.0f, -2.0f, 0.0f));
	Model* modelI = new Model(texturedCubeVAO, vector<vec3>(), uboWorldMatrixBlock, IChildren, setUpTranslation, mat4(1.0f), mat4(1.0f));


	// Creating top-part of the number 9
	setUpTranslation = translate(mat4(1.0f), vec3(3.75f, 4.5f, 0.0f));
	setUpScaling = scale(mat4(1.0f), vec3(1.5f, 1.0f, 1.0f));
	Model* model9top = new Model(texturedCubeVAO, cubeVertexPositions, uboWorldMatrixBlock, vector<Model*>(), setUpTranslation, mat4(1.0f), setUpScaling, marble);

	// Creating left-part of the number 9
	setUpTranslation = translate(mat4(1.0f), vec3(2.5f, 3.25f, 0.0f));
	setUpScaling = scale(mat4(1.0f), vec3(1.0f, 3.5f, 1.0f));
	Model* model9left = new Model(texturedCubeVAO, cubeVertexPositions, uboWorldMatrixBlock, vector<Model*>(), setUpTranslation, mat4(1.0f), setUpScaling, marble);

	// Creating bottom-part of the number 9
	setUpTranslation = translate(mat4(1.0f), vec3(3.75f, 2.0f, 0.0f));
	setUpScaling = scale(mat4(1.0f), vec3(2.0f, 1.0f, 1.0f));
	Model* model9bottom = new Model(texturedCubeVAO, cubeVertexPositions, uboWorldMatrixBlock, vector<Model*>(), setUpTranslation, mat4(1.0f), setUpScaling, marble);

	// Creating right-part of the number 9
	setUpTranslation = translate(glm::mat4(1.0f), glm::vec3(5.0f, 3.0f, 0.0f));
	setUpScaling = scale(glm::mat4(1.0f), glm::vec3(1.0f, 4.0f, 1.0f));
	Model* model9right = new Model(texturedCubeVAO, cubeVertexPositions, uboWorldMatrixBlock, vector<Model*>(), setUpTranslation, mat4(1.0f), setUpScaling, marble);


	// Setting up the number 9
	vector<Model*> nineChildren = vector<Model*>();
	nineChildren.push_back(model9top);
	nineChildren.push_back(model9right);
	nineChildren.push_back(model9left);
	nineChildren.push_back(model9bottom);
	setUpTranslation = translate(mat4(1.0f), vec3(-1.0f, -2.0f, 0.0f));
	Model* model9 = new Model(texturedCubeVAO, vector<vec3>(), uboWorldMatrixBlock, nineChildren, setUpTranslation, mat4(1.0f), mat4(1.0f));


	// Setting up the entire I9
	// This will be the root, and will be provided with the current world and sharedModel matrices in draw() from main()
	vector<Model*> I9Children = vector<Model*>();
	I9Children.push_back(modelI);
	I9Children.push_back(model9);
	I9Children.push_back(makeI9BottomModel());
	Model* modelI9 = new Model(texturedCubeVAO, vector<vec3>(), uboWorldMatrixBlock, I9Children, mat4(1.0f), mat4(1.0f), mat4(1.0f));

	// Setting up the entire model
	// This will be the root, and will be provided with the current world and sharedModel matrices in draw() from main()
	vector<Model*> entireModelChildren = vector<Model*>();
	entireModelChildren.push_back(modelI9);
	Model* entireModel = new Model(texturedCubeVAO, vector<vec3>(), uboWorldMatrixBlock, entireModelChildren, mat4(1.0f), mat4(1.0f), mat4(1.0f));

	return entireModel;
}

Model* makeU3BottomModel() {
	// Draw U3 using hierarchical modeling, start at the lowest model(s) in the hierarchy
	mat4 setUpScaling = mat4(1.0f);
	mat4 setUpRotation = mat4(1.0f);
	mat4 setUpTranslation = mat4(1.0f);

	// Creating left-part of the letter U
	setUpTranslation = translate(glm::mat4(1.0f), glm::vec3(-3.0f, 0.0f, 0.0f));
	setUpScaling = scale(glm::mat4(1.0f), glm::vec3(1.0f, 2.5f, 1.0f));
	Model* modelUleftBar = new Model(texturedCubeVAO, cubeVertexPositions, uboWorldMatrixBlock, vector<Model*>(), setUpTranslation, mat4(1.0f), setUpScaling, cement);


	// Creating bottom-part of the letter U
	setUpTranslation = translate(glm::mat4(1.0f), glm::vec3(-1.5f, -1.25f, 0.0f));
	setUpScaling = scale(glm::mat4(1.0f), glm::vec3(4.0f, 1.0f, 1.0f));
	Model* modelUbottomBar = new Model(texturedCubeVAO, cubeVertexPositions, uboWorldMatrixBlock, vector<Model*>(), setUpTranslation, mat4(1.0f), setUpScaling, cement);

	// Creating right-part of the letter U
	setUpTranslation = translate(glm::mat4(1.0f), glm::vec3(0.0f, 0.0f, 0.0f));
	setUpScaling = scale(glm::mat4(1.0f), glm::vec3(1.0f, 2.5f, 1.0f));
	Model* modelUrightBar = new Model(texturedCubeVAO, cubeVertexPositions, uboWorldMatrixBlock, vector<Model*>(), setUpTranslation, mat4(1.0f), setUpScaling, cement);

	// Setting up the letter U
	vector<Model*> UChildren = vector<Model*>();
	UChildren.push_back(modelUleftBar);
	UChildren.push_back(modelUbottomBar);
	UChildren.push_back(modelUrightBar);
	//The pieces of the U are placed such that the entire U is centered at origin on all axes
	//We can then very simply manipulate this modelU to transform the entire U
	//for example, to scoot the U left to make room for the number, making the entire U3 centered.
	setUpTranslation = translate(mat4(1.0f), vec3(-2.0f, 0.0f, 0.0f));
	Model* modelU = new Model(texturedCubeVAO, vector<vec3>(), uboWorldMatrixBlock, UChildren, setUpTranslation, mat4(1.0f), mat4(1.0f));


	// Creating base-part of the number 3
	setUpTranslation = translate(glm::mat4(1.0f), glm::vec3(4.0f, -0.5f, 0.0f));
	setUpScaling = scale(glm::mat4(1.0f), glm::vec3(1.0f, 2.5f, 1.0f));
	Model* model3base = new Model(texturedCubeVAO, cubeVertexPositions, uboWorldMatrixBlock, vector<Model*>(), setUpTranslation, mat4(1.0f), setUpScaling, cement);


	// Creating bottomArm-part of the number 3
	setUpTranslation = translate(glm::mat4(1.0f), glm::vec3(2.5f, -1.25f, 0.0f));
	setUpScaling = scale(glm::mat4(1.0f), glm::vec3(2.5f, 1.0f, 1.0f));
	Model* model3bottomArm = new Model(texturedCubeVAO, cubeVertexPositions, uboWorldMatrixBlock, vector<Model*>(), setUpTranslation, mat4(1.0f), setUpScaling, cement);

	// Setting up the number 3
	vector<Model*> threeChildren = vector<Model*>();
	threeChildren.push_back(model3base);
	threeChildren.push_back(model3bottomArm);
	setUpTranslation = translate(mat4(1.0f), vec3(0.0f, 0.0f, 0.0f));
	Model* model3 = new Model(texturedCubeVAO, vector<vec3>(), uboWorldMatrixBlock, threeChildren, setUpTranslation, mat4(1.0f), mat4(1.0f));


	// Setting up the entire U3
	// This will be the root, and will be provided with the current world and sharedModel matrices in draw() from main()
	vector<Model*> U3Children = vector<Model*>();
	U3Children.push_back(modelU);
	U3Children.push_back(model3);
	Model* modelU3 = new Model(texturedCubeVAO, vector<vec3>(), uboWorldMatrixBlock, U3Children, mat4(1.0f), mat4(1.0f), mat4(1.0f));

	// Setting up the entire model
	// This will be the root, and will be provided with the current world and sharedModel matrices in draw() from main()
	vector<Model*> entireModelChildren = vector<Model*>();
	entireModelChildren.push_back(modelU3);
	Model* entireModel = new Model(texturedCubeVAO, vector<vec3>(), uboWorldMatrixBlock, entireModelChildren, mat4(1.0f), mat4(1.0f), mat4(1.0f));

	return entireModel;
}

Model* makeU3Model() {
	// Draw U3 using hierarchical modeling, start at the lowest model(s) in the hierarchy
	glm::mat4 setUpScaling = mat4(1.0f);
	glm::mat4 setUpRotation = mat4(1.0f);
	glm::mat4 setUpTranslation = mat4(1.0f);

	// Creating left-part of the letter U
	setUpTranslation = translate(glm::mat4(1.0f), glm::vec3(-3.0f, 2.5f, 0.0f));
	setUpScaling = scale(glm::mat4(1.0f), glm::vec3(1.0f, 2.5f, 1.0f));
	Model* modelUleftBar = new Model(texturedCubeVAO, cubeVertexPositions, uboWorldMatrixBlock, vector<Model*>(), setUpTranslation, mat4(1.0f), setUpScaling, cement);

	// Creating right-part of the letter U
	setUpTranslation = translate(glm::mat4(1.0f), glm::vec3(0.0f, 2.5f, 0.0f));
	setUpScaling = scale(glm::mat4(1.0f), glm::vec3(1.0f, 2.5f, 1.0f));
	Model* modelUrightBar = new Model(texturedCubeVAO, cubeVertexPositions, uboWorldMatrixBlock, vector<Model*>(), setUpTranslation, mat4(1.0f), setUpScaling, cement);

	// Setting up the letter U
	vector<Model*> UChildren = vector<Model*>();
	UChildren.push_back(modelUleftBar);
	UChildren.push_back(modelUrightBar);
	//The pieces of the U are placed such that the entire U is centered at origin on all axes
	//We can then very simply manipulate this modelU to transform the entire U
	//for example, to scoot the U left to make room for the number, making the entire U3 centered.
	setUpTranslation = translate(glm::mat4(1.0f), glm::vec3(-2.0f, 0.0f, 0.0f));
	Model* modelU = new Model(texturedCubeVAO, vector<vec3>(), uboWorldMatrixBlock, UChildren, setUpTranslation, mat4(1.0f), mat4(1.0f));

	// Creating base-part of the number 3
	setUpTranslation = translate(glm::mat4(1.0f), glm::vec3(4.0f, 2.0f, 0.0f));
	setUpScaling = scale(glm::mat4(1.0f), glm::vec3(1.0f, 3.0f, 1.0f));
	Model* model3base = new Model(texturedCubeVAO, cubeVertexPositions, uboWorldMatrixBlock, vector<Model*>(), setUpTranslation, mat4(1.0f), setUpScaling, cement);

	// Creating topArm-part of the number 3
	setUpTranslation = translate(glm::mat4(1.0f), glm::vec3(2.5f, 3.0f, 0.0f));
	setUpScaling = scale(glm::mat4(1.0f), glm::vec3(2.5f, 1.0f, 1.0f));
	Model* model3topArm = new Model(texturedCubeVAO, cubeVertexPositions, uboWorldMatrixBlock, vector<Model*>(), setUpTranslation, mat4(1.0f), setUpScaling, cement);

	// Creating middleArm-part of the number 3
	setUpTranslation = translate(glm::mat4(1.0f), glm::vec3(3.0f, 0.75f, 0.0f));
	setUpScaling = scale(glm::mat4(1.0f), glm::vec3(1.5f, 1.0f, 1.0f));
	Model* model3middleArm = new Model(texturedCubeVAO, cubeVertexPositions, uboWorldMatrixBlock, vector<Model*>(), setUpTranslation, mat4(1.0f), setUpScaling, cement);

	// Setting up the number 3
	vector<Model*> threeChildren = vector<Model*>();
	threeChildren.push_back(model3base);
	threeChildren.push_back(model3topArm);
	threeChildren.push_back(model3middleArm);
	//threeChildren.push_back(model3bottomArm);
	setUpTranslation = translate(glm::mat4(1.0f), glm::vec3(0.0f, 0.0f, 0.0f));
	Model* model3 = new Model(texturedCubeVAO, vector<vec3>(), uboWorldMatrixBlock, threeChildren, setUpTranslation, mat4(1.0f), mat4(1.0f));

	// Setting up the entire U3
	// This will be the root, and will be provided with the current world and sharedModel matrices in draw() from main()
	vector<Model*> U3Children = vector<Model*>();
	U3Children.push_back(makeU3BottomModel());
	U3Children.push_back(modelU);
	U3Children.push_back(model3);
	Model* modelU3 = new Model(texturedCubeVAO, vector<vec3>(), uboWorldMatrixBlock, U3Children, mat4(1.0f), mat4(1.0f), mat4(1.0f));

	// Setting up the entire model
	// This will be the root, and will be provided with the current world and sharedModel matrices in draw() from main()
	vector<Model*> entireModelChildren = vector<Model*>();
	entireModelChildren.push_back(modelU3);
	Model* entireModel = new Model(texturedCubeVAO, vector<vec3>(), uboWorldMatrixBlock, entireModelChildren, mat4(1.0f), mat4(1.0f), mat4(1.0f));

	return entireModel;
}

Model* makeT9BottomModel() {
	// Draw T9 using hierarchical modeling, start at the lowest model(s) in the hierarchy
	glm::mat4 setUpScaling = mat4(1.0f);
	glm::mat4 setUpRotation = mat4(1.0f);
	glm::mat4 setUpTranslation = mat4(1.0f);

	// Creating middlet-part of the letter T
	setUpTranslation = translate(glm::mat4(1.0f), glm::vec3(-1.5f, 0.5f, 0.0f));
	setUpScaling = scale(glm::mat4(1.0f), glm::vec3(1.0f, 2.0f, 1.0f));
	Model* modelTmiddleBar = new Model(texturedCubeVAO, cubeVertexPositions, uboWorldMatrixBlock, vector<Model*>(), setUpTranslation, mat4(1.0f), setUpScaling, marble);

	// Setting up the letter T
	vector<Model*> TChildren = vector<Model*>();
	TChildren.push_back(modelTmiddleBar);
	//The pieces of the I are placed such that the entire I is centered at origin on all axes
	//We can then very simply manipulate this modelI to transform the entire I
	//for example, to scoot the I left to make room for the number, making the entire I9 centered.
	setUpTranslation = translate(glm::mat4(1.0f), glm::vec3(-1.0f, -2.0f, 0.0f));
	Model* modelI = new Model(texturedCubeVAO, vector<vec3>(), uboWorldMatrixBlock, TChildren, setUpTranslation, mat4(1.0f), mat4(1.0f));


	// Creating right-part of the number 9
	setUpTranslation = translate(glm::mat4(1.0f), glm::vec3(5.0f, 0.5f, 0.0f));
	setUpScaling = scale(glm::mat4(1.0f), glm::vec3(1.0f, 2.5f, 1.0f));
	Model* model9right = new Model(texturedCubeVAO, cubeVertexPositions, uboWorldMatrixBlock, vector<Model*>(), setUpTranslation, mat4(1.0f), setUpScaling, marble);


	// Setting up the number 9
	vector<Model*> nineChildren = vector<Model*>();
	nineChildren.push_back(model9right);
	setUpTranslation = translate(glm::mat4(1.0f), glm::vec3(-1.0f, -2.0f, 0.0f));
	Model* model9 = new Model(texturedCubeVAO, vector<vec3>(), uboWorldMatrixBlock, nineChildren, setUpTranslation, mat4(1.0f), mat4(1.0f));


	// Setting up the entire T9
	// This will be the root, and will be provided with the current world and sharedModel matrices in draw() from main()
	vector<Model*> T9Children = vector<Model*>();
	T9Children.push_back(modelI);
	T9Children.push_back(model9);
	Model* modelT9 = new Model(texturedCubeVAO, vector<vec3>(), uboWorldMatrixBlock, T9Children, mat4(1.0f), mat4(1.0f), mat4(1.0f));

	// Setting up the entire model
	// This will be the root, and will be provided with the current world and sharedModel matrices in draw() from main()
	vector<Model*> entireModelChildren = vector<Model*>();
	entireModelChildren.push_back(modelT9);
	Model* entireModel = new Model(texturedCubeVAO, vector<vec3>(), uboWorldMatrixBlock, entireModelChildren, mat4(1.0f), mat4(1.0f), mat4(1.0f));

	return entireModel;
}

Model* makeT9Model() {
	// Draw T9 using hierarchical modeling, start at the lowest model(s) in the hierarchy
	glm::mat4 setUpScaling = mat4(1.0f);
	glm::mat4 setUpRotation = mat4(1.0f);
	glm::mat4 setUpTranslation = mat4(1.0f);

	// Creating top-part of the letter T
	setUpTranslation = translate(glm::mat4(1.0f), glm::vec3(-1.5f, 4.25f, 0.0f));
	setUpScaling = scale(glm::mat4(1.0f), glm::vec3(4.0f, 1.0f, 1.0f));
	Model* modelTtopBar = new Model(texturedCubeVAO, cubeVertexPositions, uboWorldMatrixBlock, vector<Model*>(), setUpTranslation, mat4(1.0f), setUpScaling, marble);

	// Creating middle-part of the letter T
	setUpTranslation = translate(glm::mat4(1.0f), glm::vec3(-1.5f, 2.5f, 0.0f));
	setUpScaling = scale(glm::mat4(1.0f), glm::vec3(1.0f, 2.5f, 1.0f));
	Model* modelTmiddleBar = new Model(texturedCubeVAO, cubeVertexPositions, uboWorldMatrixBlock, vector<Model*>(), setUpTranslation, mat4(1.0f), setUpScaling, marble);

	// Setting up the letter T
	vector<Model*> TChildren = vector<Model*>();
	TChildren.push_back(modelTtopBar);
	TChildren.push_back(modelTmiddleBar);
	//The pieces of the I are placed such that the entire T is centered at origin on all axes
	//We can then very simply manipulate this modelI to transform the entire T
	//for example, to scoot the T left to make room for the number, making the entire I9 centered.
	setUpTranslation = translate(glm::mat4(1.0f), glm::vec3(-1.0f, -2.0f, 0.0f));
	Model* modelT = new Model(texturedCubeVAO, vector<vec3>(), uboWorldMatrixBlock, TChildren, setUpTranslation, mat4(1.0f), mat4(1.0f));

	// Creating top-part of the number 9
	setUpTranslation = translate(mat4(1.0f), vec3(3.75f, 4.5f, 0.0f));
	setUpScaling = scale(mat4(1.0f), vec3(1.5f, 1.0f, 1.0f));
	Model* model9top = new Model(texturedCubeVAO, cubeVertexPositions, uboWorldMatrixBlock, vector<Model*>(), setUpTranslation, mat4(1.0f), setUpScaling, marble);

	// Creating left-part of the number 9
	setUpTranslation = translate(mat4(1.0f), vec3(2.5f, 3.25f, 0.0f));
	setUpScaling = scale(mat4(1.0f), vec3(1.0f, 3.5f, 1.0f));
	Model* model9left = new Model(texturedCubeVAO, cubeVertexPositions, uboWorldMatrixBlock, vector<Model*>(), setUpTranslation, mat4(1.0f), setUpScaling, marble);

	// Creating bottom-part of the number 9
	setUpTranslation = translate(mat4(1.0f), vec3(3.75f, 2.0f, 0.0f));
	setUpScaling = scale(mat4(1.0f), vec3(2.0f, 1.0f, 1.0f));
	Model* model9bottom = new Model(texturedCubeVAO, cubeVertexPositions, uboWorldMatrixBlock, vector<Model*>(), setUpTranslation, mat4(1.0f), setUpScaling, marble);

	// Creating right-part of the number 9
	setUpTranslation = translate(glm::mat4(1.0f), glm::vec3(5.0f, 3.0f, 0.0f));
	setUpScaling = scale(glm::mat4(1.0f), glm::vec3(1.0f, 4.0f, 1.0f));
	Model* model9right = new Model(texturedCubeVAO, cubeVertexPositions, uboWorldMatrixBlock, vector<Model*>(), setUpTranslation, mat4(1.0f), setUpScaling, marble);


	// Setting up the number 9
	vector<Model*> nineChildren = vector<Model*>();
	nineChildren.push_back(model9top);
	nineChildren.push_back(model9right);
	nineChildren.push_back(model9left);
	nineChildren.push_back(model9bottom);
	setUpTranslation = translate(mat4(1.0f), vec3(-1.0f, -2.0f, 0.0f));
	Model* model9 = new Model(texturedCubeVAO, vector<vec3>(), uboWorldMatrixBlock, nineChildren, setUpTranslation, mat4(1.0f), mat4(1.0f));


	// Setting up the entire T9
	// This will be the root, and will be provided with the current world and sharedModel matrices in draw() from main()
	vector<Model*> T9Children = vector<Model*>();
	T9Children.push_back(makeT9BottomModel());
	T9Children.push_back(modelT);
	T9Children.push_back(model9);
	Model* modelT9 = new Model(texturedCubeVAO, vector<vec3>(), uboWorldMatrixBlock, T9Children, mat4(1.0f), mat4(1.0f), mat4(1.0f));

	// Setting up the entire model
	// This will be the root, and will be provided with the current world and sharedModel matrices in draw() from main()
	vector<Model*> entireModelChildren = vector<Model*>();
	entireModelChildren.push_back(modelT9);
	Model* entireModel = new Model(texturedCubeVAO, vector<vec3>(), uboWorldMatrixBlock, entireModelChildren, mat4(1.0f), mat4(1.0f), mat4(1.0f));

	return entireModel;
}

Model* makeC4BottomModel() {
	// Draw C4 using hierarchical modeling, start at the lowest model(s) in the hierarchy
	glm::mat4 setUpScaling = mat4(1.0f);
	glm::mat4 setUpRotation = mat4(1.0f);
	glm::mat4 setUpTranslation = mat4(1.0f);

	// Creating left-part of the letter C
	setUpTranslation = translate(glm::mat4(1.0f), glm::vec3(2.0f, 0.0f, 0.0f));
	setUpScaling = scale(glm::mat4(1.0f), glm::vec3(1.0f, 2.5f, 1.0f));
	Model* modelCleftBar = new Model(texturedCubeVAO, cubeVertexPositions, uboWorldMatrixBlock, vector<Model*>(), setUpTranslation, mat4(1.0f), setUpScaling, marble);

	// Creating bottom-part of the letter C
	setUpScaling = scale(glm::mat4(1.0f), glm::vec3(3.0f, 1.0f, 1.0f));
	setUpTranslation = translate(glm::mat4(1.0f), glm::vec3(3.0f, -0.75f, 0.0f));
	Model* modelCbottomBar = new Model(texturedCubeVAO, cubeVertexPositions, uboWorldMatrixBlock, vector<Model*>(), setUpTranslation, mat4(1.0f), setUpScaling, marble);

	// Setting up the letter C
	vector<Model*> CChildren = vector<Model*>();
	CChildren.push_back(modelCleftBar);
	CChildren.push_back(modelCbottomBar);
	//The pieces of the C are placed such that the entire C is centered at origin on all axes
	//We can then very simply manipulate this modelC to transform the entire C
	//for example, to scoot the C left to make room for the number, making the entire C4 centered.
	setUpTranslation = translate(glm::mat4(1.0f), glm::vec3(-7.0f, -2.0f, 0.0f));
	Model* modelC = new Model(texturedCubeVAO, vector<vec3>(), uboWorldMatrixBlock, CChildren, setUpTranslation, mat4(1.0f), mat4(1.0f));


	// Creating right-part of the number 4
	setUpTranslation = translate(glm::mat4(1.0f), glm::vec3(2.0f, 0.0f, 0.0f));
	setUpScaling = scale(glm::mat4(1.0f), glm::vec3(1.0f, 2.5f, 1.0f));
	Model* model4right = new Model(texturedCubeVAO, cubeVertexPositions, uboWorldMatrixBlock, vector<Model*>(), setUpTranslation, mat4(1.0f), setUpScaling, marble);


	// Setting up the number 4
	vector<Model*> fourChildren = vector<Model*>();
	fourChildren.push_back(model4right);
	setUpTranslation = translate(glm::mat4(1.0f), glm::vec3(2.0f, -2.0f, 0.0f));
	Model* model4 = new Model(texturedCubeVAO, vector<vec3>(), uboWorldMatrixBlock, fourChildren, setUpTranslation, mat4(1.0f), mat4(1.0f));

	// Setting up the entire C4
	// This will be the root, and will be provided with the current world and sharedModel matrices in draw() from main()

	vector<Model*> C4Children = vector<Model*>();
	C4Children.push_back(modelC);
	C4Children.push_back(model4);
	Model* modelC4 = new Model(texturedCubeVAO, vector<vec3>(), uboWorldMatrixBlock, C4Children, mat4(1.0f), mat4(1.0f), mat4(1.0f));

	// Setting up the entire model
	// This will be the root, and will be provided with the current world and sharedModel matrices in draw() from main()
	vector<Model*> entireModelChildren = vector<Model*>();
	entireModelChildren.push_back(modelC4);
	Model* entireModel = new Model(texturedCubeVAO, vector<vec3>(), uboWorldMatrixBlock, entireModelChildren, mat4(1.0f), mat4(1.0f), mat4(1.0f), marble);

	return entireModel;
}

Model* makeC4Model() {
	// Draw C4 using hierarchical modeling, start at the lowest model(s) in the hierarchy
	mat4 setUpScaling = mat4(1.0f);
	mat4 setUpRotation = mat4(1.0f);
	mat4 setUpTranslation = mat4(1.0f);

	// Creating left-part of the letter C
	setUpTranslation = translate(glm::mat4(1.0f), glm::vec3(0.0f, 2.5f, 0.0f));
	setUpScaling = scale(glm::mat4(1.0f), glm::vec3(1.0f, 2.5f, 1.0f));
	Model* modelCleftBar = new Model(texturedCubeVAO, cubeVertexPositions, uboWorldMatrixBlock, vector<Model*>(), setUpTranslation, mat4(1.0f), setUpScaling, marble);

	// Creating top-part of the letter C
	setUpScaling = scale(glm::mat4(1.0f), glm::vec3(3.0f, 1.0f, 1.0f));
	setUpTranslation = translate(glm::mat4(1.0f), glm::vec3(1.0f, 4.0f, 0.0f));
	Model* modelCtopBar = new Model(texturedCubeVAO, cubeVertexPositions, uboWorldMatrixBlock, vector<Model*>(), setUpTranslation, mat4(1.0f), setUpScaling, marble);

	// Setting up the letter C
	vector<Model*> CChildren = vector<Model*>();
	CChildren.push_back(modelCleftBar);
	CChildren.push_back(modelCtopBar);
	//The pieces of the C are placed such that the entire C is centered at origin on all axes
	//We can then very simply manipulate this modelC to transform the entire C
	//for example, to scoot the C left to make room for the number, making the entire C4 centered.
	setUpTranslation = translate(glm::mat4(1.0f), glm::vec3(-5.0f, -2.0f, 0.0f));
	Model* modelC = new Model(texturedCubeVAO, vector<vec3>(), uboWorldMatrixBlock, CChildren, setUpTranslation, mat4(1.0f), mat4(1.0f));

	// Creating right-part of the number 4
	setUpTranslation = translate(glm::mat4(1.0f), glm::vec3(2.0f, 2.5f, 0.0f));
	setUpScaling = scale(glm::mat4(1.0f), glm::vec3(1.0f, 2.75f, 1.0f));
	Model* model4right = new Model(texturedCubeVAO, cubeVertexPositions, uboWorldMatrixBlock, vector<Model*>(), setUpTranslation, mat4(1.0f), setUpScaling, marble);

	// Creating middle-part of the number 4
	setUpTranslation = translate(glm::mat4(1.0f), glm::vec3(0.0f, 1.5f, 0.0f));
	setUpScaling = scale(glm::mat4(1.0f), glm::vec3(3.0f, 1.0f, 1.0f));
	Model* model4middle = new Model(texturedCubeVAO, cubeVertexPositions, uboWorldMatrixBlock, vector<Model*>(), setUpTranslation, mat4(1.0f), setUpScaling, marble);

	// Creating left-part of the number 4
	setUpTranslation = translate(glm::mat4(1.0f), glm::vec3(-1.0f, 3.0f, 0.0f));
	setUpScaling = scale(glm::mat4(1.0f), glm::vec3(1.0f, 2.0f, 1.0f));
	Model* model4left = new Model(texturedCubeVAO, cubeVertexPositions, uboWorldMatrixBlock, vector<Model*>(), setUpTranslation, mat4(1.0f), setUpScaling, marble);

	// Setting up the number 4
	vector<Model*> fourChildren = vector<Model*>();
	fourChildren.push_back(model4left);
	fourChildren.push_back(model4middle);
	fourChildren.push_back(model4right);
	setUpTranslation = translate(glm::mat4(1.0f), glm::vec3(2.0f, -2.0f, 0.0f));
	Model* model4 = new Model(texturedCubeVAO, vector<vec3>(), uboWorldMatrixBlock, fourChildren, setUpTranslation, mat4(1.0f), mat4(1.0f));

	// Setting up the entire C4
	// This will be the root, and will be provided with the current world and sharedModel matrices in draw() from main()
	vector<Model*> C4Children = vector<Model*>();
	C4Children.push_back(makeC4BottomModel());
	C4Children.push_back(modelC);
	C4Children.push_back(model4);
	Model* modelC4 = new Model(texturedCubeVAO, vector<vec3>(), uboWorldMatrixBlock, C4Children, mat4(1.0f), mat4(1.0f), mat4(1.0f));

	// Setting up the entire model
	// This will be the root, and will be provided with the current world and sharedModel matrices in draw() from main()
	vector<Model*> entireModelChildren = vector<Model*>();
	entireModelChildren.push_back(modelC4);
	Model* entireModel = new Model(texturedCubeVAO, vector<vec3>(), uboWorldMatrixBlock, entireModelChildren, mat4(1.0f), mat4(1.0f), mat4(1.0f));

	return entireModel;
}

Model* makeFloorModel(Terrain terrain, City city, glm::vec3 worldPos) {
	// Draw floor using hierarchical modeling, start at the lowest model(s) in the hierarchy
	mat4 setUpScaling = scale(mat4(1.0f), vec3(1.0f));
	mat4 setUpRotation = rotate(mat4(1.0f), 0.0f, vec3(1.0f));
	mat4 setUpTranslation = translate(mat4(1.0f), glm::vec3(worldPos.x * city.gridWidth, 0, worldPos.z * city.gridHeight));

	// This will be the root, and will be provided with the current world and sharedModel matrices in draw() from main()
	vector<Model*> children = vector<Model*>();

    for(int b = 0; b < city.buildings.size(); b++) {
        Building* building = city.buildings[b];
        vector<Model*> buildingChildren = vector<Model*>();

        srand((building->pos.x + city.offset.x) * (building->pos.y * city.offset.z) - building->type->type);

        int r = rand() % 5;

        switch(r) {
            case 0:
                buildingChildren.push_back(
                        makeBuilding1Model(
                                texturedCubeVAO,
                                terrain,
                                glm::vec3(0.2),
                                1
                        )
                );
                break;
            case 1:
                buildingChildren.push_back(
                        makeBuilding2Model(
                                texturedCubeVAO,
                                terrain,
                                glm::vec3(0.2),
                                1
                        )
                );
                break;
            case 2:
                buildingChildren.push_back(
                        makeBuilding3Model(
                                texturedCubeVAO,
                                terrain,
                                glm::vec3(0.2),
                                1
                        )
                );
                break;
            case 3:
                buildingChildren.push_back(
                        makeBuilding4Model(
                                texturedCubeVAO,
                                terrain,
                                glm::vec3(0.2),
                                1
                        )
                );
                break;
            case 4:
                buildingChildren.push_back(
                        makeBuilding5Model(
                                texturedCubeVAO,
                                terrain,
                                glm::vec3(0.2),
                                1
                        )
                );
                break;
        }

        children.push_back(
                new Model(
                        0,
                        vector<glm::vec3>(),
                        uboWorldMatrixBlock,
                        buildingChildren,
                        translate(
                                mat4(1.0f),
                                glm::vec3(
                                        building->pos.x,
                                        terrain.getHeightAt(
                                                building->pos.x - city.gridWidth / 2,
                                                building->pos.y - city.gridHeight / 2
                                        ),
                                        building->pos.y
                                ) * glm::vec3(1, 1, 1) - glm::vec3(city.gridWidth / 2, 0, city.gridHeight / 2)
                        ),
                        mat4(1.0f),
                        scale(
                                mat4(1.0f),
                                building->size * glm::vec3(1, 1, 1)
                        ),
                        beige
                )
        );
    }

    glm::vec2* secret = city.secrets[0];
    std::vector<Model*> placeholder = std::vector<Model*>();
    placeholder.push_back(makeL9Model());
    children.push_back(
            new Model(
                    0,
                    vector<glm::vec3>(),
                    uboWorldMatrixBlock,
                    placeholder,
                    translate(
                            mat4(1.0f),
                            glm::vec3(
                                    secret->x,
                                    terrain.getHeightAt(
                                            secret->x - city.gridWidth / 2,
                                            secret->y - city.gridHeight / 2
                                    ) + 1,
                                    secret->y
                            ) * glm::vec3(1, 1, 1) - glm::vec3(city.gridWidth / 2, 0, city.gridHeight / 2)
                    ),
                    mat4(1.0f),
                    scale(mat4(1),vec3(1.0f/200.0f, 1.0f/100.0f, 1/200.0f)),
                    beige
            )
    );

    secret = city.secrets[1];
    placeholder = std::vector<Model*>();
    placeholder.push_back(makeI9Model());
    children.push_back(
            new Model(
                    0,
                    vector<glm::vec3>(),
                    uboWorldMatrixBlock,
                    placeholder,
                    translate(
                            mat4(1.0f),
                            glm::vec3(
                                    secret->x,
                                    terrain.getHeightAt(
                                            secret->x - city.gridWidth / 2,
                                            secret->y - city.gridHeight / 2
                                    ) + 1,
                                    secret->y
                            ) * glm::vec3(1, 1, 1) - glm::vec3(city.gridWidth / 2, 0, city.gridHeight / 2)
                    ),
                    mat4(1.0f),
                    scale(mat4(1),vec3(1.0f/200.0f, 1.0f/100.0f, 1/200.0f)),
                    beige
            )
    );

    secret = city.secrets[2];
    placeholder = std::vector<Model*>();
    placeholder.push_back(makeU3Model());
    children.push_back(
            new Model(
                    0,
                    vector<glm::vec3>(),
                    uboWorldMatrixBlock,
                    placeholder,
                    translate(
                            mat4(1.0f),
                            glm::vec3(
                                    secret->x,
                                    terrain.getHeightAt(
                                            secret->x - city.gridWidth / 2,
                                            secret->y - city.gridHeight / 2
                                    ) + 1,
                                    secret->y
                            ) * glm::vec3(1, 1, 1) - glm::vec3(city.gridWidth / 2, 0, city.gridHeight / 2)
                    ),
                    mat4(1.0f),
                    scale(mat4(1),vec3(1.0f/200.0f, 1.0f/100.0f, 1/200.0f)),
                    beige
            )
    );

    secret = city.secrets[3];
    placeholder = std::vector<Model*>();
    placeholder.push_back(makeT9Model());
    children.push_back(
            new Model(
                    0,
                    vector<glm::vec3>(),
                    uboWorldMatrixBlock,
                    placeholder,
                    translate(
                            mat4(1.0f),
                            glm::vec3(
                                    secret->x,
                                    terrain.getHeightAt(
                                            secret->x - city.gridWidth / 2,
                                            secret->y - city.gridHeight / 2
                                    ) + 1,
                                    secret->y
                            ) * glm::vec3(1, 1, 1) - glm::vec3(city.gridWidth / 2, 0, city.gridHeight / 2)
                    ),
                    mat4(1.0f),
                    scale(mat4(1),vec3(1.0f/200.0f, 1.0f/100.0f, 1/200.0f)),
                    beige
            )
    );

    secret = city.secrets[4];
    placeholder = std::vector<Model*>();
    placeholder.push_back(makeC4Model());
    children.push_back(
            new Model(
                    0,
                    vector<glm::vec3>(),
                    uboWorldMatrixBlock,
                    placeholder,
                    translate(
                            mat4(1.0f),
                            glm::vec3(
                                    secret->x,
                                    terrain.getHeightAt(
                                            secret->x - city.gridWidth / 2,
                                            secret->y - city.gridHeight / 2
                                    ) + 1,
                                    secret->y
                            ) * glm::vec3(1, 1, 1) - glm::vec3(city.gridWidth / 2, 0, city.gridHeight / 2)
                    ),
                    mat4(1.0f),
                    scale(mat4(1),vec3(1.0f/200.0f, 1.0f/100.0f, 1/200.0f)),
                    beige
            )
    );

    placeholder = std::vector<Model*>();
    placeholder.push_back(makeLampModel(lampVAO));

    std::vector<MainStreet*> ms = city.mainStreets;

    for(int s = 0; s < ms.size(); s++) {
        MainStreet m = *ms[s];

        int start_x = m.start.x;

        if (start_x >= city.gridWidth)
            continue;

        for(int j = m.start.y; j <= m.end.y - 5; j+=5) {
            int start_y = j;
            placeholder = std::vector<Model*>();
            placeholder.push_back(makeLampModel(lampVAO));
            children.push_back(
                    new Model(
                            0,
                            vector<glm::vec3>(),
                            uboWorldMatrixBlock,
                            placeholder,
                            translate(
                                    mat4(1.0f),
                                    glm::vec3(
                                            start_x + 0.3,
                                            terrain.getHeightAt(
                                                    (start_x + 0.3) - city.gridWidth / 2,
                                                    start_y - city.gridHeight / 2
                                            ),
                                            start_y
                                    ) * glm::vec3(1, 1, 1) - glm::vec3(city.gridWidth / 2, 0, city.gridHeight / 2)
                            ),
                            mat4(1.0f),
                            scale(mat4(1),vec3(2.0f/200.0f, 5.0f/100.0f, 2/200.0f)),
                            beige
                    )
            );

            children.push_back(
                    new Model(
                            0,
                            vector<glm::vec3>(),
                            uboWorldMatrixBlock,
                            placeholder,
                            translate(
                                    mat4(1.0f),
                                    glm::vec3(
                                            start_x - 0.3,
                                            terrain.getHeightAt(
                                                    (start_x - 0.3) - city.gridWidth / 2,
                                                    start_y - city.gridHeight / 2
                                            ),
                                            start_y
                                    ) * glm::vec3(1, 1, 1) - glm::vec3(city.gridWidth / 2, 0, city.gridHeight / 2)
                            ),
                            mat4(1.0f),
                            scale(mat4(1),vec3(2.0f/200.0f, 5.0f/100.0f, 2/200.0f)),
                            beige
                    )
            );

            placeholder = std::vector<Model*>();
            placeholder.push_back(makeCarModel(carVAO));

            children.push_back(
                    new Model(
                            0,
                            vector<glm::vec3>(),
                            uboWorldMatrixBlock,
                            placeholder,
                            translate(
                                    mat4(1.0f),
                                    glm::vec3(
                                            start_x,
                                            terrain.getHeightAt(
                                                    (start_x) - city.gridWidth / 2,
                                                    (start_y + 3) - city.gridHeight / 2
                                            ),
                                            start_y + 3
                                    ) * glm::vec3(1, 1, 1) - glm::vec3(city.gridWidth / 2, 0, city.gridHeight / 2)
                            ),
                            mat4(1.0f),
                            scale(mat4(1),vec3(0.5f/200.0f, 0.5f/100.0f, 0.5f/200.0f)),
                            beige
                    )
            );
        }
    }

//    carModel = makeCarModel(carVAO);
//    garbageModel = makeGarbageModel(garbageVAO);
//    hydrantModel = makeHydrantModel(hydrantVAO);
//    lampModel = makeLampModel(lampVAO);


    setUpScaling = scale(glm::mat4(1), glm::vec3(200, 100, 200));

    Model* floorModel = new Model(terrain.getVAO(), terrain.getVertices(), uboWorldMatrixBlock, children, setUpTranslation, setUpRotation, setUpScaling, blue);

	return floorModel;
}

Skybox* makeSkyBoxModel(int vao) {
	return new Skybox(vao, sphereVertices, uboWorldMatrixBlock, vector<Model*>(), mat4(1.0f), mat4(1.0f), mat4(1.0f), sky);
}

Model* makeBuilding1Model(int vao, Terrain terrain, glm::vec3 scaleXYZ, float numOfFloors) {
	// Draw building using hierarchical modeling, start at the lowest model(s) in the hierarchy
	mat4 setUpScaling = scale(mat4(1.0f), vec3(1.0f));
	mat4 setUpRotation = rotate(mat4(1.0f), 0.0f, vec3(1.0f));
	mat4 setUpTranslation = translate(mat4(1.0f), vec3(0.0f));

//	srand(scaleXYZ.y * numOfFloors);

	pair<int, Material> materialBuilding = getRandomMaterial();
	pair<int, Material> materialDoor;

	do {
		materialDoor = getRandomMaterial();
	} while (materialDoor.first == materialBuilding.first);


	setUpTranslation = translate(mat4(1.0f), vec3(0.0f, 1.f, 0.0f));
	setUpScaling = scale(mat4(1.0f), vec3(5.0f, 2.0f, 5.0f));
	Model* bottom = new Model(vao, cubeVertexPositions, uboWorldMatrixBlock, vector<Model*>(), setUpTranslation, mat4(1.0f), setUpScaling, materialBuilding.second);

	setUpTranslation = translate(mat4(1.0f), vec3(0.0f, 3.5f, 0.0f));
	setUpScaling = scale(mat4(1.0f), vec3(5.0f, 7.0f, 5.0f));
	Model* windowWall = new Model(vao, cubeVertexPositions, uboWorldMatrixBlock, vector<Model*>(), setUpTranslation, mat4(1.0f), setUpScaling, materialBuilding.second);

	setUpTranslation = translate(mat4(1.0f), vec3(0.0f, 4.0f, 2.6f));
	setUpScaling = scale(mat4(1.0f), vec3(3.0f, 5.0f, 0.1f));
	Model* window1 = new Model(vao, cubeVertexPositions, uboWorldMatrixBlock, vector<Model*>(), setUpTranslation, mat4(1.0f), setUpScaling, windowFrame);

	setUpTranslation = translate(mat4(1.0f), vec3(0.0f, 4.0f, -2.6f));
	setUpScaling = scale(mat4(1.0f), vec3(3.0f, 5.0f, 0.1f));
	Model* window2 = new Model(vao, cubeVertexPositions, uboWorldMatrixBlock, vector<Model*>(), setUpTranslation, mat4(1.0f), setUpScaling, windowFrame);

	setUpTranslation = translate(mat4(1.0f), vec3(2.5f, 4.0f, 0.0f));
	setUpRotation = rotate(mat4(1.0f), radians(90.0f), vec3(0.0f, 1.0f, 0.0f));
	setUpScaling = scale(mat4(1.0f), vec3(3.0f, 5.0f, 0.1f));
	Model* window3 = new Model(vao, cubeVertexPositions, uboWorldMatrixBlock, vector<Model*>(), setUpTranslation, setUpRotation, setUpScaling, windowFrame);

	setUpTranslation = translate(mat4(1.0f), vec3(-2.5f, 4.0f, 0.0f));
	setUpRotation = rotate(mat4(1.0f), radians(90.0f), vec3(0.0f, 1.0f, 0.0f));
	setUpScaling = scale(mat4(1.0f), vec3(3.0f, 5.0f, 0.1f));
	Model* window4 = new Model(vao, cubeVertexPositions, uboWorldMatrixBlock, vector<Model*>(), setUpTranslation, setUpRotation, setUpScaling, windowFrame);

	setUpTranslation = translate(mat4(1.0f), vec3(0.0f, 0.75f, 2.6f));
	setUpScaling = scale(mat4(1.0f), vec3(1.0f, 1.5f, .1f));
	Model* door1 = new Model(vao, cubeVertexPositions, uboWorldMatrixBlock, vector<Model*>(), setUpTranslation, mat4(1.0f), setUpScaling, materialDoor.second);

	setUpTranslation = translate(mat4(1.0f), vec3(0.0f, 0.75f, -2.6f));
	setUpScaling = scale(mat4(1.0f), vec3(1.0f, 1.5f, .1f));
	Model* door2 = new Model(vao, cubeVertexPositions, uboWorldMatrixBlock, vector<Model*>(), setUpTranslation, mat4(1.0f), setUpScaling, materialDoor.second);


	vector<Model*> buildingChildren = vector<Model*>();
	buildingChildren.push_back(door1);
	buildingChildren.push_back(door2);
	buildingChildren.push_back(bottom);

	vector<Model*> buildingMiddleChildren = vector<Model*>();
	buildingMiddleChildren.push_back(windowWall);
	buildingMiddleChildren.push_back(window1);
	buildingMiddleChildren.push_back(window2);
	buildingMiddleChildren.push_back(window3);
	buildingMiddleChildren.push_back(window4);

	Model* buildingMiddleModel = new Model(vao, vector<vec3>(), uboWorldMatrixBlock, buildingMiddleChildren, setUpTranslation, mat4(1.0f), setUpScaling);

	float repeatTranslate = 0.5f;

	for (int i = 0; i < numOfFloors; i++)
	{
		setUpTranslation = translate(mat4(1.0f), vec3(0, repeatTranslate, 0));
		Model* buildingRepeatModel = new Model(vao, vector<vec3>(), uboWorldMatrixBlock, buildingMiddleChildren, setUpTranslation, mat4(1.0f), mat4(1.0f));

		repeatTranslate += 6.0f;

		buildingChildren.push_back(buildingRepeatModel);
	}

	setUpTranslation = translate(mat4(1.0f), vec3(0.0f, repeatTranslate + 1.5, 0.0f));
	setUpScaling = scale(mat4(1.0f), vec3(4.0f, 1.0f, 4.0f));
	Model* middle = new Model(vao, cubeVertexPositions, uboWorldMatrixBlock, vector<Model*>(), setUpTranslation, mat4(1.0f), setUpScaling, materialBuilding.second);

	setUpTranslation = translate(mat4(1.0f), vec3(0.0f, repeatTranslate + 2.5f, 0.0f));
	setUpScaling = scale(mat4(1.0f), vec3(3.0f, 1.0f, 3.0f));
	Model* top = new Model(vao, cubeVertexPositions, uboWorldMatrixBlock, vector<Model*>(), setUpTranslation, mat4(1.0f), setUpScaling, materialBuilding.second);


	buildingChildren.push_back(middle);
	buildingChildren.push_back(top);

	setUpScaling = scale(mat4(1.0f), scaleXYZ);
	setUpTranslation = translate(mat4(1.0f), vec3(0, 0, 0));

	Model* buildingModel = new Model(vao, vector<vec3>(), uboWorldMatrixBlock, buildingChildren, setUpTranslation, mat4(1.0f), setUpScaling);

	return buildingModel;
}

Model* makeBuilding2Model(int vao, Terrain terrain, glm::vec3 scaleXYZ, float numOfFloors) {
	// Draw building using hierarchical modeling, start at the lowest model(s) in the hierarchy
	mat4 setUpScaling = scale(mat4(1.0f), vec3(1.0f));
	mat4 setUpRotation = rotate(mat4(1.0f), 0.0f, vec3(1.0f));
	mat4 setUpTranslation = translate(mat4(1.0f), vec3(0.0f));

//    srand(scaleXYZ.y * numOfFloors);

	pair<int, Material> materialBuilding = getRandomMaterial();
	pair<int, Material> materialDoor;

	do {
		materialDoor = getRandomMaterial();
	} while (materialDoor.first == materialBuilding.first);

	setUpTranslation = translate(mat4(1.0f), vec3(0.0f, 1.0f, 0.0f));
	setUpScaling = scale(mat4(1.0f), vec3(2.0f, 2.0f, 2.0f));
	Model* base = new Model(vao, cubeVertexPositions, uboWorldMatrixBlock, vector<Model*>(), setUpTranslation, mat4(1.0f), setUpScaling, materialBuilding.second);

	setUpTranslation = translate(mat4(1.0f), vec3(0.0f, 3.5f, 0.0f));
	setUpScaling = scale(mat4(1.0f), vec3(2.0f, 5.0f, 2.0f));
	Model* windowWall = new Model(vao, cubeVertexPositions, uboWorldMatrixBlock, vector<Model*>(), setUpTranslation, mat4(1.0f), setUpScaling, materialBuilding.second);

	setUpTranslation = translate(mat4(1.0f), vec3(-0.5f, 4.0f, 1.005f));
	setUpScaling = scale(mat4(1.0f), vec3(0.7f, 2.5f, 0.1f));
	Model* window1 = new Model(vao, cubeVertexPositions, uboWorldMatrixBlock, vector<Model*>(), setUpTranslation, mat4(1.0f), setUpScaling, windowFrame);

	setUpTranslation = translate(mat4(1.0f), vec3(0.5f, 4.0f, 1.005f));
	setUpScaling = scale(mat4(1.0f), vec3(0.7f, 2.5f, 0.1f));
	Model* window2 = new Model(vao, cubeVertexPositions, uboWorldMatrixBlock, vector<Model*>(), setUpTranslation, mat4(1.0f), setUpScaling, windowFrame);

	setUpTranslation = translate(mat4(1.0f), vec3(-0.5f, 4.0f, -1.005f));
	setUpScaling = scale(mat4(1.0f), vec3(0.7f, 2.5f, 0.1f));
	Model* window3 = new Model(vao, cubeVertexPositions, uboWorldMatrixBlock, vector<Model*>(), setUpTranslation, mat4(1.0f), setUpScaling, windowFrame);

	setUpTranslation = translate(mat4(1.0f), vec3(0.5f, 4.0f, -1.005f));
	setUpScaling = scale(mat4(1.0f), vec3(0.7f, 2.5f, 0.1f));
	Model* window4 = new Model(vao, cubeVertexPositions, uboWorldMatrixBlock, vector<Model*>(), setUpTranslation, mat4(1.0f), setUpScaling, windowFrame);

	setUpTranslation = translate(mat4(1.0f), vec3(1.0f, 4.0f, -0.5f));
	setUpRotation = rotate(mat4(1.0f), radians(90.0f), vec3(0.0f, 1.0f, 0.0f));
	setUpScaling = scale(mat4(1.0f), vec3(0.7f, 2.5f, 0.1f));
	Model* window5 = new Model(vao, cubeVertexPositions, uboWorldMatrixBlock, vector<Model*>(), setUpTranslation, setUpRotation, setUpScaling, windowFrame);

	setUpTranslation = translate(mat4(1.0f), vec3(1.0f, 4.0f, 0.5f));
	setUpRotation = rotate(mat4(1.0f), radians(90.0f), vec3(0.0f, 1.0f, 0.0f));
	setUpScaling = scale(mat4(1.0f), vec3(0.7f, 2.5f, 0.1f));
	Model* window6 = new Model(vao, cubeVertexPositions, uboWorldMatrixBlock, vector<Model*>(), setUpTranslation, setUpRotation, setUpScaling, windowFrame);

	setUpTranslation = translate(mat4(1.0f), vec3(-1.0f, 4.0f, -0.5f));
	setUpRotation = rotate(mat4(1.0f), radians(90.0f), vec3(0.0f, 1.0f, 0.0f));
	setUpScaling = scale(mat4(1.0f), vec3(0.7f, 2.5f, 0.1f));
	Model* window7 = new Model(vao, cubeVertexPositions, uboWorldMatrixBlock, vector<Model*>(), setUpTranslation, setUpRotation, setUpScaling, windowFrame);

	setUpTranslation = translate(mat4(1.0f), vec3(-1.0f, 4.0f, 0.5f));
	setUpRotation = rotate(mat4(1.0f), radians(90.0f), vec3(0.0f, 1.0f, 0.0f));
	setUpScaling = scale(mat4(1.0f), vec3(0.7f, 2.5f, 0.1f));
	Model* window8 = new Model(vao, cubeVertexPositions, uboWorldMatrixBlock, vector<Model*>(), setUpTranslation, setUpRotation, setUpScaling, windowFrame);

	setUpTranslation = translate(mat4(1.0f), vec3(0.0f, 0.75f, 1.05f));
	setUpScaling = scale(mat4(1.0f), vec3(0.5f, 1.5f, 0.1f));
	Model* door1 = new Model(vao, cubeVertexPositions, uboWorldMatrixBlock, vector<Model*>(), setUpTranslation, mat4(1.0f), setUpScaling, materialDoor.second);

	setUpTranslation = translate(mat4(1.0f), vec3(0.0f, 0.75f, -1.05f));
	setUpScaling = scale(mat4(1.0f), vec3(0.5f, 1.5f, 0.1f));
	Model* door2 = new Model(vao, cubeVertexPositions, uboWorldMatrixBlock, vector<Model*>(), setUpTranslation, mat4(1.0f), setUpScaling, materialDoor.second);

	vector<Model*> buildingChildren = vector<Model*>();
	buildingChildren.push_back(door1);
	buildingChildren.push_back(door2);
	buildingChildren.push_back(base);

	vector<Model*> buildingMiddleChildren = vector<Model*>();
	buildingMiddleChildren.push_back(windowWall);
	buildingMiddleChildren.push_back(window1);
	buildingMiddleChildren.push_back(window2);
	buildingMiddleChildren.push_back(window3);
	buildingMiddleChildren.push_back(window4);
	buildingMiddleChildren.push_back(window5);
	buildingMiddleChildren.push_back(window6);
	buildingMiddleChildren.push_back(window7);
	buildingMiddleChildren.push_back(window8);

	Model* buildingMiddleModel = new Model(vao, vector<vec3>(), uboWorldMatrixBlock, buildingMiddleChildren, setUpTranslation, mat4(1.0f), setUpScaling);

	float repeatTranslate = 0.5f;

	for (int i = 0; i < numOfFloors; i++)
	{
		setUpTranslation = translate(mat4(1.0f), vec3(0, repeatTranslate, 0));
		Model* buildingRepeatModel = new Model(vao, vector<vec3>(), uboWorldMatrixBlock, buildingMiddleChildren, setUpTranslation, mat4(1.0f), mat4(1.0f));

		repeatTranslate += 4.0f;

		buildingChildren.push_back(buildingRepeatModel);
	}

    setUpScaling = scale(mat4(1.0f), scaleXYZ);
    setUpTranslation = translate(mat4(1.0f), vec3(0, 0, 0));

    Model* buildingModel = new Model(vao, vector<vec3>(), uboWorldMatrixBlock, buildingChildren, setUpTranslation, mat4(1.0f), setUpScaling);

    return buildingModel;
}

Model* makeBuilding3Model(int vao, Terrain terrain, glm::vec3 scaleXYZ, float numOfFloors) {
	// Draw building using hierarchical modeling, start at the lowest model(s) in the hierarchy
	mat4 setUpScaling = scale(mat4(1.0f), vec3(1.0f));
	mat4 setUpRotation = rotate(mat4(1.0f), 0.0f, vec3(1.0f));
	mat4 setUpTranslation = translate(mat4(1.0f), vec3(0.0f));

//    srand(scaleXYZ.y * numOfFloors);

	pair<int, Material> materialBuilding1 = getRandomMaterial();
	pair<int, Material> materialBuilding2;

	do {
		materialBuilding2 = getRandomMaterial();
	} while (materialBuilding2.first == materialBuilding1.first);

	setUpTranslation = translate(mat4(1.0f), vec3(0.0f, 1.0f, 2.0f));
	setUpScaling = scale(mat4(1.0f), vec3(4.0f, 2.0f, 0.5f));
	Model* front = new Model(vao, cubeVertexPositions, uboWorldMatrixBlock, vector<Model*>(), setUpTranslation, mat4(1.0f), setUpScaling, materialBuilding1.second);

	setUpTranslation = translate(mat4(1.0f), vec3(0.0f, 1.0f, -2.0f));
	setUpScaling = scale(mat4(1.0f), vec3(4.0f, 2.0f, 0.5f));
	Model* back = new Model(vao, cubeVertexPositions, uboWorldMatrixBlock, vector<Model*>(), setUpTranslation, mat4(1.0f), setUpScaling, materialBuilding1.second);

	setUpTranslation = translate(mat4(1.0f), vec3(0.0f, 1.0f, 0.0f));
	setUpScaling = scale(mat4(1.0f), vec3(4.0f, 2.0f, 4.0f));
	Model* base = new Model(vao, cubeVertexPositions, uboWorldMatrixBlock, vector<Model*>(), setUpTranslation, mat4(1.0f), setUpScaling, materialBuilding2.second);

	setUpTranslation = translate(mat4(1.0f), vec3(0.0f, 3.0f, 0.0f));
	setUpScaling = scale(mat4(1.0f), vec3(4.0f, 2.0f, 4.0f));
	Model* top = new Model(vao, cubeVertexPositions, uboWorldMatrixBlock, vector<Model*>(), setUpTranslation, mat4(1.0f), setUpScaling, materialBuilding2.second);

	setUpTranslation = translate(mat4(1.0f), vec3(-2.2f, 1.12f, -0.5f));
	setUpRotation = rotate(mat4(1.0f), radians(90.0f), vec3(0.0f, 1.0f, 0.0f));
	setUpScaling = scale(mat4(1.0f), vec3(3.f, 2.25f, 0.5f));
	Model* leftBottom = new Model(vao, cubeVertexPositions, uboWorldMatrixBlock, vector<Model*>(), setUpTranslation, setUpRotation, setUpScaling, materialBuilding1.second);

	setUpTranslation = translate(mat4(1.0f), vec3(-2.2f, 3.25f, -0.5f));
	setUpRotation = rotate(mat4(1.0f), radians(90.0f), vec3(0.0f, 1.0f, 0.0f));
	setUpScaling = scale(mat4(1.0f), vec3(3.f, 2.25f, 0.5f));
	Model* leftTop = new Model(vao, cubeVertexPositions, uboWorldMatrixBlock, vector<Model*>(), setUpTranslation, setUpRotation, setUpScaling, materialBuilding1.second);

	setUpTranslation = translate(mat4(1.0f), vec3(0.0f, 0.5f, 2.35f));
	setUpScaling = scale(mat4(1.0f), vec3(0.5f, 1.0f, 0.1f));
	Model* door1 = new Model(vao, cubeVertexPositions, uboWorldMatrixBlock, vector<Model*>(), setUpTranslation, mat4(1.0f), setUpScaling, materialBuilding2.second);

	setUpTranslation = translate(mat4(1.0f), vec3(0.0f, 0.5f, -2.35f));
	setUpScaling = scale(mat4(1.0f), vec3(0.5f, 1.0f, 0.1f));
	Model* door2 = new Model(vao, cubeVertexPositions, uboWorldMatrixBlock, vector<Model*>(), setUpTranslation, mat4(1.0f), setUpScaling, materialBuilding2.second);

	setUpTranslation = translate(mat4(1.0f), vec3(-1.0f, 3.0f, 2.05f));
	setUpScaling = scale(mat4(1.0f), vec3(1.0f, 1.0f, 0.1f));
	Model* window1 = new Model(vao, cubeVertexPositions, uboWorldMatrixBlock, vector<Model*>(), setUpTranslation, mat4(1.0f), setUpScaling, windowFrame);

	setUpTranslation = translate(mat4(1.0f), vec3(1.0f, 3.0f, 2.05f));
	setUpScaling = scale(mat4(1.0f), vec3(1.0f, 1.0f, 0.1f));
	Model* window2 = new Model(vao, cubeVertexPositions, uboWorldMatrixBlock, vector<Model*>(), setUpTranslation, mat4(1.0f), setUpScaling, windowFrame);

	setUpTranslation = translate(mat4(1.0f), vec3(-1.0f, 3.0f, -2.05f));
	setUpScaling = scale(mat4(1.0f), vec3(1.0f, 1.0f, 0.1f));
	Model* window3 = new Model(vao, cubeVertexPositions, uboWorldMatrixBlock, vector<Model*>(), setUpTranslation, mat4(1.0f), setUpScaling, windowFrame);

	setUpTranslation = translate(mat4(1.0f), vec3(1.0f, 3.0f, -2.05f));
	setUpScaling = scale(mat4(1.0f), vec3(1.0f, 1.0f, 0.1f));
	Model* window4 = new Model(vao, cubeVertexPositions, uboWorldMatrixBlock, vector<Model*>(), setUpTranslation, mat4(1.0f), setUpScaling, windowFrame);

	setUpTranslation = translate(mat4(1.0f), vec3(2.05f, 3.0f, -1.05f));
	setUpRotation = rotate(mat4(1.0f), radians(90.0f), vec3(0.0f, 1.0f, 0.0f));
	setUpScaling = scale(mat4(1.0f), vec3(1.0f, 1.0f, 0.1f));
	Model* window5 = new Model(vao, cubeVertexPositions, uboWorldMatrixBlock, vector<Model*>(), setUpTranslation, setUpRotation, setUpScaling, windowFrame);

	setUpTranslation = translate(mat4(1.0f), vec3(2.05f, 3.0f, 1.05f));
	setUpRotation = rotate(mat4(1.0f), radians(90.0f), vec3(0.0f, 1.0f, 0.0f));
	setUpScaling = scale(mat4(1.0f), vec3(1.0f, 1.0f, 0.1f));
	Model* window6 = new Model(vao, cubeVertexPositions, uboWorldMatrixBlock, vector<Model*>(), setUpTranslation, setUpRotation, setUpScaling, windowFrame);


	vector<Model*> buildingChildren = vector<Model*>();
	buildingChildren.push_back(front);
	buildingChildren.push_back(back);
	buildingChildren.push_back(base);
	buildingChildren.push_back(door1);
	buildingChildren.push_back(door2);
	buildingChildren.push_back(leftBottom);

	vector<Model*> buildingMiddleChildren = vector<Model*>();
	buildingMiddleChildren.push_back(top);
	buildingMiddleChildren.push_back(leftTop);
	buildingMiddleChildren.push_back(window1);
	buildingMiddleChildren.push_back(window2);
	buildingMiddleChildren.push_back(window3);
	buildingMiddleChildren.push_back(window4);
	buildingMiddleChildren.push_back(window5);
	buildingMiddleChildren.push_back(window6);

	Model* buildingMiddleModel = new Model(vao, vector<vec3>(), uboWorldMatrixBlock, buildingMiddleChildren, setUpTranslation, mat4(1.0f), setUpScaling);

	float repeatTranslate = 0.0f;

	for (int i = 0; i < numOfFloors; i++)
	{
		setUpTranslation = translate(mat4(1.0f), vec3(0, repeatTranslate, 0));
		Model* buildingRepeatModel = new Model(vao, vector<vec3>(), uboWorldMatrixBlock, buildingMiddleChildren, setUpTranslation, mat4(1.0f), mat4(1.0f));

		repeatTranslate += 2.0f;

		buildingChildren.push_back(buildingRepeatModel);
	}

    setUpScaling = scale(mat4(1.0f), scaleXYZ);
    setUpTranslation = translate(mat4(1.0f), vec3(0, 0, 0));

    Model* buildingModel = new Model(vao, vector<vec3>(), uboWorldMatrixBlock, buildingChildren, setUpTranslation, mat4(1.0f), setUpScaling);

    return buildingModel;
}

Model* makeBuilding4Model(int vao, Terrain terrain, glm::vec3 scaleXYZ, float numOfFloors) {
	// Draw building using hierarchical modeling, start at the lowest model(s) in the hierarchy
	mat4 setUpScaling = scale(mat4(1.0f), vec3(1.0f));
	mat4 setUpRotation = rotate(mat4(1.0f), 0.0f, vec3(1.0f));
	mat4 setUpTranslation = translate(mat4(1.0f), vec3(0.0f));

//    srand(scaleXYZ.y * numOfFloors);

	pair<int, Material> materialBuilding1 = getRandomMaterial();
	pair<int, Material> materialBuilding2;

	do {
		materialBuilding2 = getRandomMaterial();
	} while (materialBuilding2.first == materialBuilding1.first);

	setUpTranslation = translate(mat4(1.0f), vec3(0.0f, 1.0f, 0.0f));
	setUpScaling = scale(mat4(1.0f), vec3(4.0f, 2.0f, 4.0f));
	Model* base = new Model(vao, cubeVertexPositions, uboWorldMatrixBlock, vector<Model*>(), setUpTranslation, mat4(1.0f), setUpScaling, materialBuilding1.second);

	setUpTranslation = translate(mat4(1.0f), vec3(0.0f, 3.0f, 0.0f));
	setUpScaling = scale(mat4(1.0f), vec3(4.0f, 2.0f, 4.0f));
	Model* middle = new Model(vao, cubeVertexPositions, uboWorldMatrixBlock, vector<Model*>(), setUpTranslation, mat4(1.0f), setUpScaling, materialBuilding1.second);

	setUpTranslation = translate(mat4(1.0f), vec3(-2.2f, 1.0f, -0.5f));
	setUpRotation = rotate(mat4(1.0f), radians(90.0f), vec3(0.0f, 1.0f, 0.0f));
	setUpScaling = scale(mat4(1.0f), vec3(3.f, 2.0f, 0.5f));
	Model* leftBigBase = new Model(vao, cubeVertexPositions, uboWorldMatrixBlock, vector<Model*>(), setUpTranslation, setUpRotation, setUpScaling, materialBuilding2.second);

	setUpTranslation = translate(mat4(1.0f), vec3(-2.2f, 3.0f, -0.5f));
	setUpRotation = rotate(mat4(1.0f), radians(90.0f), vec3(0.0f, 1.0f, 0.0f));
	setUpScaling = scale(mat4(1.0f), vec3(3.f, 2.0f, 0.5f));
	Model* leftBigTop = new Model(vao, cubeVertexPositions, uboWorldMatrixBlock, vector<Model*>(), setUpTranslation, setUpRotation, setUpScaling, materialBuilding2.second);

	setUpTranslation = translate(mat4(1.0f), vec3(-2.7f, 0.5f, -0.7f));
	setUpRotation = rotate(mat4(1.0f), radians(90.0f), vec3(0.0f, 1.0f, 0.0f));
	setUpScaling = scale(mat4(1.0f), vec3(2.5f, 1.5f, 0.5f));
	Model* leftSmallBase = new Model(vao, cubeVertexPositions, uboWorldMatrixBlock, vector<Model*>(), setUpTranslation, setUpRotation, setUpScaling, materialBuilding1.second);

	setUpTranslation = translate(mat4(1.0f), vec3(-2.7f, 1.75f, -0.7f));
	setUpRotation = rotate(mat4(1.0f), radians(90.0f), vec3(0.0f, 1.0f, 0.0f));
	setUpScaling = scale(mat4(1.0f), vec3(2.5f, 1.5f, 0.5f));
	Model* leftSmallTop = new Model(vao, cubeVertexPositions, uboWorldMatrixBlock, vector<Model*>(), setUpTranslation, setUpRotation, setUpScaling, materialBuilding1.second);

	setUpTranslation = translate(mat4(1.0f), vec3(0.0f, 2.5f, 2.1f));
	setUpScaling = scale(mat4(1.0f), vec3(3.0f, 1.5f, 0.1f));
	Model* window1 = new Model(vao, cubeVertexPositions, uboWorldMatrixBlock, vector<Model*>(), setUpTranslation, mat4(1.0f), setUpScaling, windowFrame);

	setUpTranslation = translate(mat4(1.0f), vec3(0.0f, 2.5f, -2.1f));
	setUpScaling = scale(mat4(1.0f), vec3(3.0f, 1.5f, 0.1f));
	Model* window2 = new Model(vao, cubeVertexPositions, uboWorldMatrixBlock, vector<Model*>(), setUpTranslation, mat4(1.0f), setUpScaling, windowFrame);

	setUpTranslation = translate(mat4(1.0f), vec3(2.0f, 2.5f, 0.0f));
	setUpRotation = rotate(mat4(1.0f), radians(90.0f), vec3(0.0f, 1.0f, 0.0f));
	setUpScaling = scale(mat4(1.0f), vec3(3.0f, 1.5f, 0.1f));
	Model* window3 = new Model(vao, cubeVertexPositions, uboWorldMatrixBlock, vector<Model*>(), setUpTranslation, setUpRotation, setUpScaling, windowFrame);

	setUpTranslation = translate(mat4(1.0f), vec3(0.0f, 0.5f, 2.1f));
	setUpScaling = scale(mat4(1.0f), vec3(1.0f, 1.0f, 0.1f));
	Model* door1 = new Model(vao, cubeVertexPositions, uboWorldMatrixBlock, vector<Model*>(), setUpTranslation, mat4(1.0f), setUpScaling, materialBuilding2.second);

	setUpTranslation = translate(mat4(1.0f), vec3(0.0f, 0.5f, -2.1f));
	setUpScaling = scale(mat4(1.0f), vec3(1.0f, 1.0f, 0.1f));
	Model* door2 = new Model(vao, cubeVertexPositions, uboWorldMatrixBlock, vector<Model*>(), setUpTranslation, mat4(1.0f), setUpScaling, materialBuilding2.second);


	vector<Model*> buildingChildren = vector<Model*>();
	buildingChildren.push_back(base);
	buildingChildren.push_back(leftBigBase);
	buildingChildren.push_back(leftSmallBase);
	buildingChildren.push_back(door1);
	buildingChildren.push_back(door2);

	vector<Model*> buildingMiddleChildren = vector<Model*>();
	buildingMiddleChildren.push_back(middle);
	buildingMiddleChildren.push_back(leftBigTop);
	buildingMiddleChildren.push_back(leftSmallTop);
	buildingMiddleChildren.push_back(window1);
	buildingMiddleChildren.push_back(window2);
	buildingMiddleChildren.push_back(window3);

	Model* buildingMiddleModel = new Model(vao, vector<vec3>(), uboWorldMatrixBlock, buildingMiddleChildren, setUpTranslation, mat4(1.0f), setUpScaling);

	float repeatTranslate = 0.0f;

	for (int i = 0; i < numOfFloors; i++)
	{
		setUpTranslation = translate(mat4(1.0f), vec3(0, repeatTranslate, 0));
		Model* buildingRepeatModel = new Model(vao, vector<vec3>(), uboWorldMatrixBlock, buildingMiddleChildren, setUpTranslation, mat4(1.0f), mat4(1.0f));

		repeatTranslate += 2.0f;

		buildingChildren.push_back(buildingRepeatModel);
	}

	setUpTranslation = translate(mat4(1.0f), vec3(0.3f, 2.2 + repeatTranslate, -0.5f));
	setUpRotation = rotate(mat4(1.0f), radians(90.0f), vec3(1.0f, 0.0f, 0.0f));
	setUpScaling = scale(mat4(1.0f), vec3(3.5f, 3.0f, 0.5f));
	Model* top = new Model(vao, cubeVertexPositions, uboWorldMatrixBlock, vector<Model*>(), setUpTranslation, setUpRotation, setUpScaling, materialBuilding2.second);

	buildingChildren.push_back(top);

    setUpScaling = scale(mat4(1.0f), scaleXYZ);
    setUpTranslation = translate(mat4(1.0f), vec3(0, 0, 0));

    Model* buildingModel = new Model(vao, vector<vec3>(), uboWorldMatrixBlock, buildingChildren, setUpTranslation, mat4(1.0f), setUpScaling);

    return buildingModel;
}

Model* makeBuilding5Model(int vao, Terrain terrain, glm::vec3 scaleXYZ, float numOfFloors) {
	// Draw building using hierarchical modeling, start at the lowest model(s) in the hierarchy
	mat4 setUpScaling = scale(mat4(1.0f), vec3(1.0f));
	mat4 setUpRotation = rotate(mat4(1.0f), 0.0f, vec3(1.0f));
	mat4 setUpTranslation = translate(mat4(1.0f), vec3(0.0f));

//    srand(scaleXYZ.y * numOfFloors);

	pair<int, Material> materialBuilding1 = getRandomMaterial();
	pair<int, Material> materialBuilding2;

	do {
		materialBuilding2 = getRandomMaterial();
	} while (materialBuilding2.first == materialBuilding1.first);

	setUpTranslation = translate(mat4(1.0f), vec3(2.0f, 3.5f, 0.0f));
	setUpScaling = scale(mat4(1.0f), vec3(2.0f, 7.0f, 3.0f));
	Model* right = new Model(vao, cubeVertexPositions, uboWorldMatrixBlock, vector<Model*>(), setUpTranslation, mat4(1.0f), setUpScaling, materialBuilding2.second);

	setUpTranslation = translate(mat4(1.0f), vec3(-2.0f, 2.5f, 0.0f));
	setUpScaling = scale(mat4(1.0f), vec3(2.0f, 5.0f, 2.0f));
	Model* left = new Model(vao, cubeVertexPositions, uboWorldMatrixBlock, vector<Model*>(), setUpTranslation, mat4(1.0f), setUpScaling, materialBuilding2.second);

	setUpTranslation = translate(mat4(1.0f), vec3(0.0f, 1.5f, 0.0f));
	setUpScaling = scale(mat4(1.0f), vec3(2.0f, 3.0f, 1.5f));
	Model* middle = new Model(vao, cubeVertexPositions, uboWorldMatrixBlock, vector<Model*>(), setUpTranslation, mat4(1.0f), setUpScaling, materialBuilding1.second);

	setUpTranslation = translate(mat4(1.0f), vec3(0.0f, 0.75f, 0.8f));
	setUpScaling = scale(mat4(1.0f), vec3(1.0f, 1.5f, 0.1f));
	Model* door1 = new Model(vao, cubeVertexPositions, uboWorldMatrixBlock, vector<Model*>(), setUpTranslation, mat4(1.0f), setUpScaling, materialBuilding2.second);

	setUpTranslation = translate(mat4(1.0f), vec3(0.0f, 0.75f, -0.8f));
	setUpScaling = scale(mat4(1.0f), vec3(1.0f, 1.5f, 0.1f));
	Model* door2 = new Model(vao, cubeVertexPositions, uboWorldMatrixBlock, vector<Model*>(), setUpTranslation, mat4(1.0f), setUpScaling, materialBuilding2.second);

	setUpTranslation = translate(mat4(1.0f), vec3(-2.0f, 3.75f, 1.05f));
	setUpScaling = scale(mat4(1.0f), vec3(1.5f, 2.0f, 0.1f));
	Model* window1 = new Model(vao, cubeVertexPositions, uboWorldMatrixBlock, vector<Model*>(), setUpTranslation, mat4(1.0f), setUpScaling, windowFrame);

	setUpTranslation = translate(mat4(1.0f), vec3(-2.0f, 3.75f, -1.05f));
	setUpScaling = scale(mat4(1.0f), vec3(1.5f, 2.0f, 0.1f));
	Model* window2 = new Model(vao, cubeVertexPositions, uboWorldMatrixBlock, vector<Model*>(), setUpTranslation, mat4(1.0f), setUpScaling, windowFrame);

	setUpTranslation = translate(mat4(1.0f), vec3(-3.0f, 3.75f, 0.0f));
	setUpRotation = rotate(mat4(1.0f), radians(90.0f), vec3(0.0f, 1.0f, 0.0f));
	setUpScaling = scale(mat4(1.0f), vec3(1.5f, 2.0f, 0.1f));
	Model* window3 = new Model(vao, cubeVertexPositions, uboWorldMatrixBlock, vector<Model*>(), setUpTranslation, setUpRotation, setUpScaling, windowFrame);

	setUpTranslation = translate(mat4(1.0f), vec3(2.0f, 2.75f, 1.55f));
	setUpScaling = scale(mat4(1.0f), vec3(1.5f, 2.0f, 0.1f));
	Model* window4 = new Model(vao, cubeVertexPositions, uboWorldMatrixBlock, vector<Model*>(), setUpTranslation, mat4(1.0f), setUpScaling, windowFrame);

	setUpTranslation = translate(mat4(1.0f), vec3(2.0f, 2.75f, -1.55f));
	setUpScaling = scale(mat4(1.0f), vec3(1.5f, 2.0f, 0.1f));
	Model* window5 = new Model(vao, cubeVertexPositions, uboWorldMatrixBlock, vector<Model*>(), setUpTranslation, mat4(1.0f), setUpScaling, windowFrame);

	setUpTranslation = translate(mat4(1.0f), vec3(3.0f, 2.75f, 0.0f));
	setUpRotation = rotate(mat4(1.0f), radians(90.0f), vec3(0.0f, 1.0f, 0.0f));
	setUpScaling = scale(mat4(1.0f), vec3(1.5f, 2.0f, 0.1f));
	Model* window6 = new Model(vao, cubeVertexPositions, uboWorldMatrixBlock, vector<Model*>(), setUpTranslation, setUpRotation, setUpScaling, windowFrame);


	vector<Model*> buildingChildren = vector<Model*>();
	buildingChildren.push_back(middle);
	buildingChildren.push_back(door1);
	buildingChildren.push_back(door2);

	vector<Model*> buildingLeftChildren = vector<Model*>();
	buildingLeftChildren.push_back(left);
	buildingLeftChildren.push_back(window1);
	buildingLeftChildren.push_back(window2);
	buildingLeftChildren.push_back(window3);

	vector<Model*> buildingRightChildren = vector<Model*>();
	buildingRightChildren.push_back(right);
	buildingRightChildren.push_back(window4);
	buildingRightChildren.push_back(window5);
	buildingRightChildren.push_back(window6);

	Model* buildingLeftModel = new Model(vao, vector<vec3>(), uboWorldMatrixBlock, buildingLeftChildren, setUpTranslation, mat4(1.0f), setUpScaling);

	float diff = getRandomNumber(-(numOfFloors - 2), numOfFloors-2);
	float repeatTranslate = 0.0f;

	for (int i = 0; i < numOfFloors-diff; i++)
	{
		setUpTranslation = translate(mat4(1.0f), vec3(0, repeatTranslate, 0));
		Model* buildingRepeatModel = new Model(vao, vector<vec3>(), uboWorldMatrixBlock, buildingLeftChildren, setUpTranslation, mat4(1.0f), mat4(1.0f));

		repeatTranslate += 3.0f;

		buildingChildren.push_back(buildingRepeatModel);
	}

	repeatTranslate = 0.0f;

	for (int i = 0; i < numOfFloors+diff; i++)
	{
		setUpTranslation = translate(mat4(1.0f), vec3(0, repeatTranslate, 0));
		Model* buildingRepeatModel = new Model(vao, vector<vec3>(), uboWorldMatrixBlock, buildingRightChildren, setUpTranslation, mat4(1.0f), mat4(1.0f));

		repeatTranslate += 3.0f;

		buildingChildren.push_back(buildingRepeatModel);
	}

    setUpScaling = scale(mat4(1.0f), scaleXYZ);
    setUpTranslation = translate(mat4(1.0f), vec3(0, 0, 0));

    Model* buildingModel = new Model(vao, vector<vec3>(), uboWorldMatrixBlock, buildingChildren, setUpTranslation, mat4(1.0f), setUpScaling);

    return buildingModel;
}

Model* makeCarModel(int vao) {
	mat4 setUpScaling = scale(mat4(1.0f), vec3(50.0f, 50.0f, 50.0f));
	mat4 setUpRotation = rotate(mat4(1.0f), 0.0f, vec3(1.0f));
	mat4 setUpTranslation = translate(mat4(1.0f), vec3(0.0f));

	vector<Model*> children = vector<Model*>();
	pair<int, Material> carMaterial = getRandomMaterial();
	return new Model(vao, carVertices, uboWorldMatrixBlock, children, setUpTranslation, setUpRotation, setUpScaling, carMaterial.second);
}

Model* makeLampModel(int vao) {
	mat4 setUpScaling = scale(mat4(1.0f), vec3(1.0f));
	mat4 setUpRotation = rotate(mat4(1.0f), 0.0f, vec3(1.0f));
	mat4 setUpTranslation = translate(mat4(1.0f), vec3(0.0f));

	vector<Model*> children = vector<Model*>();
	return new Model(vao, lampVertices, uboWorldMatrixBlock, children, setUpTranslation, setUpRotation, setUpScaling, gray);
}

Model* makeHydrantModel(int vao) {
	mat4 setUpScaling = scale(mat4(1.0f), vec3(1.0f));
	mat4 setUpRotation = rotate(mat4(1.0f), 0.0f, vec3(1.0f));
	mat4 setUpTranslation = translate(mat4(1.0f), vec3(0.0f));

	vector<Model*> children = vector<Model*>();
	return new Model(vao, hydrantVertices, uboWorldMatrixBlock, children, setUpTranslation, setUpRotation, setUpScaling, red);
}

Model* makeGarbageModel(int vao) {
	mat4 setUpScaling = scale(mat4(1.0f), vec3(1.0f));
	mat4 setUpRotation = rotate(mat4(1.0f), 0.0f, vec3(1.0f));
	mat4 setUpTranslation = translate(mat4(1.0f), vec3(0.0f));

	vector<Model*> children = vector<Model*>();
	return new Model(vao, garbageVertices, uboWorldMatrixBlock, children, setUpTranslation, setUpRotation, setUpScaling, gray);
}
#pragma endregion

#pragma usingShaders
GLuint defaultShaderProgram;
GLuint phongLightShaderProgram;
GLuint shadowShaderProgram;
GLuint uiShaderProgram;
void useShader(int shaderProgram, mat4 projectionMatrix, mat4 viewMatrix) {
	glUseProgram(shaderProgram);

	GLuint viewMatrixLocation = glGetUniformLocation(shaderProgram, "viewMatrix");
	glUniformMatrix4fv(viewMatrixLocation, 1, GL_FALSE, &viewMatrix[0][0]);

	GLuint projectionMatrixLocation = glGetUniformLocation(shaderProgram, "projectionMatrix");
	glUniformMatrix4fv(projectionMatrixLocation, 1, GL_FALSE, &projectionMatrix[0][0]);
}

void useStandardShader() {
	projectionMatrix = perspective(70.0f, // field of view in degrees
		(float)windowWidth / windowHeight,  // aspect ratio
		0.01f, viewDistance);
	useShader(defaultShaderProgram, projectionMatrix, viewMatrix);
}

void useLightingShader() {
	projectionMatrix = perspective(70.0f, // field of view in degrees
		(float)windowWidth / windowHeight,  // aspect ratio
		0.01f, viewDistance);
	useShader(phongLightShaderProgram, projectionMatrix, viewMatrix);

	//Set up vertex shader uniforms
	glBindBuffer(GL_UNIFORM_BUFFER, uboLightInfoBlock);
	setDataForDrawingLights();
	glBindBuffer(GL_UNIFORM_BUFFER, 0);

	GLuint texture = glGetUniformLocation(phongLightShaderProgram, "someTexture");
	glUniform1i(texture, 0);

	GLuint lightCoefficients = glGetUniformLocation(phongLightShaderProgram, "lightCoefficients");
	glUniform4f(lightCoefficients, 0.3f, 0.8f, 0.5f, 256);

	GLuint lightColor = glGetUniformLocation(phongLightShaderProgram, "lightColor");
	glUniform3f(lightColor, 1.0f, 1.0f, 1.0f);

	GLuint camPosition = glGetUniformLocation(phongLightShaderProgram, "cameraPosition");
	glUniform3f(camPosition, cameraPosition.x, cameraPosition.y, cameraPosition.z);

	GLuint isLightOnLocation = glGetUniformLocation(phongLightShaderProgram, "isLight1On");
	glUniform1f(isLightOnLocation, isFlashLightOn);
}

void useUIShader() {
	projectionMatrix = perspective(70.0f, // field of view in degrees
		(float)windowWidth / windowHeight,  // aspect ratio
		0.01f, 300.0f);
	useShader(uiShaderProgram, projectionMatrix, viewMatrix);

	GLuint texture = glGetUniformLocation(phongLightShaderProgram, "someTexture");
	glUniform1i(texture, 0);
}
#pragma endregion

Model* l9Model;
Model* i9Model;
Model* u3Model;
Model* t9Model;
Model* c4Model;

Model* carModel;
Model* garbageModel;
Model* hydrantModel;
Model* lampModel;

Model* floorModel;
mat4 L9Matrix, I9Matrix, U3Matrix, T9Matrix, C4Matrix, carMatrix, garbageMatrix, hydrantMatrix, lampMatrix;
void drawScene() {
    GLuint lightCoefLocation = glGetUniformLocation(phongLightShaderProgram, "lightCoefficients");
    GLuint lightColorLocation = glGetUniformLocation(phongLightShaderProgram, "lightColor");
//    l9Model->draw(L9Matrix, renderingMode, lightCoefLocation, lightColorLocation);
//    i9Model->draw(I9Matrix, renderingMode, lightCoefLocation, lightColorLocation);
//    u3Model->draw(U3Matrix, renderingMode, lightCoefLocation, lightColorLocation);
//    t9Model->draw(T9Matrix, renderingMode, lightCoefLocation, lightColorLocation);
//    c4Model->draw(C4Matrix, renderingMode, lightCoefLocation, lightColorLocation);
//    carModel->draw(carMatrix, renderingMode, lightCoefLocation, lightColorLocation);
//    garbageModel->draw(garbageMatrix, renderingMode, lightCoefLocation, lightColorLocation);
//    hydrantModel->draw(hydrantMatrix, renderingMode, lightCoefLocation, lightColorLocation);
//    lampModel->draw(lampMatrix, renderingMode, lightCoefLocation, lightColorLocation);

    for (int i = 0; i < floorModels.size(); i++) {
        Model *fm = floorModels[i];

        if (glm::distance(
                glm::vec3(cameraPosition.x, 0, cameraPosition.z),
                glm::vec3(worldMap[i].second->offset.x, 0, worldMap[i].second->offset.z) * glm::vec3(400, 1, 400)
        ) <= viewDistance + std::sqrt(200 * 200 + 200 * 200)) {
            floorModels[i]->draw(mat4(1.0f), renderingMode, lightCoefLocation, lightColorLocation);
        }
    }
}

int main(int argc, char* argv[])
{
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
//	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

	// Load Textures
#if defined(PLATFORM_OSX) || __linux__
	woodTexture = loadTexture("Textures/wood.jpg");
	skyTexture = loadTexture("Textures/sky.jpg");
	windowTexture = loadTexture("Textures/window.png");
	brownTexture = loadTexture("Textures/brown.jpg");
	beigeTexture = loadTexture("Textures/beige.jpg");
	blackTexture = loadTexture("Textures/black.jpg");
	redTexture = loadTexture("Textures/red.png");
	blueTexture = loadTexture("Textures/blue.jpg");
	purpleTexture = loadTexture("Textures/purple.jpg");
	yellowTexture = loadTexture("Textures/yellow.jpg");
	whiteTexture = loadTexture("Textures/white.jpg");
	cementTexture = loadTexture("Textures/cement.jpg");
	marbleTexture = loadTexture("Textures/marble.jpg");
#else
	woodTexture = loadTexture("../Source/COMP371-Group14-Project/Textures/wood.jpg");
	skyTexture = loadTexture("../Source/COMP371-Group14-Project/Textures/sky.jpg");
	windowTexture = loadTexture("../Source/COMP371-Group14-Project/Textures/window.png");
	brownTexture = loadTexture("../Source/COMP371-Group14-Project/Textures/brown.jpg");
	beigeTexture = loadTexture("../Source/COMP371-Group14-Project/Textures/beige.jpg");
	blackTexture = loadTexture("../Source/COMP371-Group14-Project/Textures/black.jpg");
	redTexture = loadTexture("../Source/COMP371-Group14-Project/Textures/red.png");
	blueTexture = loadTexture("../Source/COMP371-Group14-Project/Textures/blue.jpg");
	purpleTexture = loadTexture("../Source/COMP371-Group14-Project/Textures/purple.jpg");
	yellowTexture = loadTexture("../Source/COMP371-Group14-Project/Textures/yellow.jpg");
	grayTexture = loadTexture("../Source/COMP371-Group14-Project/Textures/gray.jpg");
	whiteTexture = loadTexture("../Source/COMP371-Group14-Project/Textures/white.jpg");
	cementTexture = loadTexture("../Source/COMP371-Group14-Project/Textures/cement.jpg");
	marbleTexture = loadTexture("../Source/COMP371-Group14-Project/Textures/marble.jpg");
#endif

	float globalAmbientIntensity = 0.25f;

	wood = {};
	wood.texture = woodTexture;
	wood.lightCoefficients = vec4(globalAmbientIntensity, 0.8f, 0.9f, 52);
	wood.lightColor = vec3(252.0f / 255.0f, 244.0f / 255.0f, 202.0f / 255.0f);

	cement = {};
	cement.texture = cementTexture;
	cement.lightCoefficients = vec4(globalAmbientIntensity, 0.8f, 0.9f, 52);
	cement.lightColor = vec3(252.0f / 255.0f, 244.0f / 255.0f, 202.0f / 255.0f);

	marble = {};
	marble.texture = marbleTexture;
	marble.lightCoefficients = vec4(globalAmbientIntensity, 0.8f, 0.9f, 52);
	marble.lightColor = vec3(252.0f / 255.0f, 244.0f / 255.0f, 202.0f / 255.0f);

	sky = {};
	sky.texture = skyTexture;
	sky.lightCoefficients = vec4(1.0f, 0.0f, 0.0f, 0);
	sky.lightColor = vec3(1.0f);

	windowFrame = {};
	windowFrame.texture = windowTexture;
	windowFrame.lightCoefficients = vec4(globalAmbientIntensity, 0.8f, 0.5f, 256);
	windowFrame.lightColor = vec3(1.0f);

	brown = {};
	brown.texture = brownTexture;
	brown.lightCoefficients = vec4(globalAmbientIntensity, 0.8f, 0.5f, 256);
	brown.lightColor = vec3(1.0f);

	beige = {};
	beige.texture = beigeTexture;
	beige.lightCoefficients = vec4(globalAmbientIntensity, 0.8f, 0.5f, 256);
	beige.lightColor = vec3(1.0f);

	black = {};
	black.texture = blackTexture;
	black.lightCoefficients = vec4(globalAmbientIntensity, 0.8f, 0.5f, 256);
	black.lightColor = vec3(1.0f);

	red = {};
	red.texture = redTexture;
	red.lightCoefficients = vec4(globalAmbientIntensity, 0.8f, 0.5f, 256);
	red.lightColor = vec3(1.0f);

	blue = {};
	blue.texture = blueTexture;
	blue.lightCoefficients = vec4(globalAmbientIntensity, 0.8f, 0.5f, 256);
	blue.lightColor = vec3(1.0f);

	purple = {};
	purple.texture = purpleTexture;
	purple.lightCoefficients = vec4(globalAmbientIntensity, 0.8f, 0.5f, 256);
	purple.lightColor = vec3(1.0f);

	yellow = {};
	yellow.texture = yellowTexture;
	yellow.lightCoefficients = vec4(globalAmbientIntensity, 0.8f, 0.5f, 256);
	yellow.lightColor = vec3(1.0f);

	gray = {};
	gray.texture = grayTexture;
	gray.lightCoefficients = vec4(globalAmbientIntensity, 0.8f, 0.5f, 256);
	gray.lightColor = vec3(1.0f);

	white = {};
	white.texture = whiteTexture;
	white.lightCoefficients = vec4(1.0f, 0.0f, 0.0f, 0.0f);
	white.lightColor = vec3(1.0f);

	// Compile and link shaders here ...
#if defined(PLATFORM_OSX) || __linux__
	defaultShaderProgram = shader("Models/modelShader.vs", "Models/modelShader.fs");
	phongLightShaderProgram = shader("LightingSystem/lightShader.vs", "LightingSystem/lightShader.fs");
	shadowShaderProgram = shader("LightingSystem/shadowShader.vs", "LightingSystem/shadowShader.fs");
	uiShaderProgram = shader("UISystem/uiShader.vs", "UISystem/uiShader.fs");
#else
	defaultShaderProgram = shader("../Source/COMP371-Group14-Project/Models/modelShader.vs", "../Source/COMP371-Group14-Project/Models/modelShader.fs");
	phongLightShaderProgram = shader("../Source/COMP371-Group14-Project/LightingSystem/lightShader.vs", "../Source/COMP371-Group14-Project/LightingSystem/lightShader.fs");
	shadowShaderProgram = shader("../Source/COMP371-Group14-Project/LightingSystem/shadowShader.vs", "../Source/COMP371-Group14-Project/LightingSystem/shadowShader.fs");
	uiShaderProgram = shader("../Source/COMP371-Group14-Project/UISystem/uiShader.vs", "../Source/COMP371-Group14-Project/UISystem/uiShader.fs");
#endif


#pragma endregion windowSetUp

#pragma region shaderBuffersSetUp
	// world matrix (both shaders)
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

	// depth view projetion matrix (shadow shader)
	glGenBuffers(1, &uboDepthVPBlock);
	glBindBuffer(GL_UNIFORM_BUFFER, uboDepthVPBlock);
	glBufferData(GL_UNIFORM_BUFFER, 64, NULL, GL_STATIC_DRAW);
	glBindBuffer(GL_UNIFORM_BUFFER, 0);
	glBindBufferBase(GL_UNIFORM_BUFFER, 1, uboDepthVPBlock);  // bind world matrix info to block 1
	//bind shaders' locations for the world matrix block to the same location, block 1:
	unsigned int depthVP_matrix_index = glGetUniformBlockIndex(shadowShaderProgram, "DepthVPMatrix");
	glUniformBlockBinding(shadowShaderProgram, depthVP_matrix_index, 1);
	setUBODepthVPBlockInLightSourceManager(uboDepthVPBlock);

	// all light info matrix (light shader)
	glGenBuffers(1, &uboLightInfoBlock);
	glBindBuffer(GL_UNIFORM_BUFFER, uboLightInfoBlock);
	glBufferData(GL_UNIFORM_BUFFER, totalNumLights * 96, NULL, GL_STATIC_DRAW);
	glBindBuffer(GL_UNIFORM_BUFFER, 0);
	glBindBufferBase(GL_UNIFORM_BUFFER, 2, uboLightInfoBlock);  // bind world matrix info to block 1
	//bind shaders' locations for the world matrix block to the same location, block 1:
	unsigned int light_info_index = glGetUniformBlockIndex(phongLightShaderProgram, "LightInfo");
	glUniformBlockBinding(phongLightShaderProgram, light_info_index, 2);
#pragma endregion

	// Define and upload geometry to the GPU here ...
	texturedCubeVAO = createTextureCubeVertexArrayObject();
#if defined(PLATFORM_OSX) || __linux__
	sphereVAO = createSphereObjectVAO("Objects/sphere.obj");
	carVAO = createObjectVAO("Objects/car.obj", carVertices);
	lampVAO = createObjectVAO("Objects/lamp-post.obj", lampVertices);
	garbageVAO = createObjectVAO("Objects/garbage.obj", garbageVertices);
	hydrantVAO = createObjectVAO("Objects/fire-hydrant.obj", hydrantVertices);
#else
	sphereVAO = createSphereObjectVAO("../Source/COMP371-Group14-Project/Objects/sphere.obj");
	carVAO = createObjectVAO("../Source/COMP371-Group14-Project/Objects/car.obj", carVertices);
	lampVAO = createObjectVAO("../Source/COMP371-Group14-Project/Objects/lamp-post.obj", lampVertices);
	garbageVAO = createObjectVAO("../Source/COMP371-Group14-Project/Objects/garbage.obj", garbageVertices);
	hydrantVAO = createObjectVAO("../Source/COMP371-Group14-Project/Objects/fire-hydrant.obj", hydrantVertices);
#endif

	vector<Model*> collisionModels;
//	collisionModels.push_back(l9Model);
//	collisionModels.push_back(i9Model);
//	collisionModels.push_back(u3Model);
//	collisionModels.push_back(t9Model);
//	collisionModels.push_back(c4Model);
	
//	collisionModels.push_back(carModel);
//	collisionModels.push_back(lampModel);
//	collisionModels.push_back(hydrantModel);
//	collisionModels.push_back(garbageModel);

    worldMap = std::vector<std::pair<Terrain*, City*>>();

    floorModels = std::vector<Model*>();

	int citySize = 15;
    City* city = new City(citySize, citySize, glm::vec3(0));
    Terrain* terrain = new Terrain(glm::vec3(citySize, 1, citySize), glm::vec3(0), citySize + 1);

    for (int i = 0; i < 1; i++) {
        for (int j = 0; j < 1; j++) {
            City* city = new City(citySize, citySize, glm::vec3(i * citySize, 0, j * citySize));
            Terrain* terrain = new Terrain(glm::vec3(citySize, 1, citySize), glm::vec3(i * citySize, 0, j * citySize), citySize + 1);

            worldMap.push_back(std::make_pair(terrain, city));

            floorModel = makeFloorModel(*terrain, *city, glm::vec3(i * 200, 0, j * 200));

            floorModels.push_back(floorModel);
			collisionModels.push_back(floorModel);
        }
    }

	Skybox* skyBoxModel = makeSkyBoxModel(sphereVAO);

	createPointedCornerVertexArrayObject();
	Model* flashlightUIModel = makeFlashlightUIModel(white, texturedCubeVAO, cubeVertexPositions, uboWorldMatrixBlock);
	Model* controlsUIModel = makeMovementUIModel();
	Model* controlLabelUIModel = makeControlsUIModel();

	// For frame time
	float lastFrameTime = glfwGetTime();
	int lastMouseLeftState = GLFW_RELEASE;
	double lastMousePosX, lastMousePosY;
	glfwGetCursorPos(window, &lastMousePosX, &lastMousePosY);

	// Enable Backface culling and depth test
	glEnable(GL_CULL_FACE);
	glEnable(GL_TEXTURE_2D);

	glPointSize(3.0f);

	//Create light sources
	createLightSources();

	setRandomizedPositionScale(L9Matrix, *terrain);
	setRandomizedPositionScale(C4Matrix, *terrain);
	setRandomizedPositionScale(U3Matrix, *terrain);
	setRandomizedPositionScale(T9Matrix, *terrain);
	setRandomizedPositionScale(I9Matrix, *terrain);
	setRandomizedPositionScale(carMatrix, *terrain);
	setRandomizedPositionScale(garbageMatrix, *terrain);
	setRandomizedPositionScale(hydrantMatrix, *terrain);
	setRandomizedPositionScale(lampMatrix, *terrain);

	// Entering Main Loop
	while (!glfwWindowShouldClose(window))
	{
		// Frame time calculation
		dt = glfwGetTime() - lastFrameTime;
		lastFrameTime += dt;

		mat4 skyboxMatrix = translate(mat4(1.0f), vec3(cameraPosition.x, cameraPosition.y, cameraPosition.z));

#pragma region shadowPass1
		//use the shadow shader, draw all objects for all lights
		glUseProgram(shadowShaderProgram);
		while (makeShadowMapForNextLight()) {
			//Draw scene for the shadow maps
			drawScene();
		}
#pragma endRegion

#pragma region shadowPass2
		//bind and clear the default (screen) framebuffer
		glBindFramebuffer(GL_FRAMEBUFFER, 0);
		glViewport(0, 0, windowWidth, windowHeight);

		// Each frame, reset color of each pixel to glClearColor
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		useLightingShader();
		bindShadowMaps(phongLightShaderProgram);
		skyBoxModel->draw(skyboxMatrix, GL_TRIANGLES, glGetUniformLocation(phongLightShaderProgram, "lightCoefficients"), glGetUniformLocation(phongLightShaderProgram, "lightColor"));
		drawScene();
#pragma endregion

#pragma region renderUI
		useUIShader();
		vec3 UIPos = cameraPosition + 100.0f * cameraLookAt;
		glDisable(GL_DEPTH_TEST);
		mat4 drawUIAT = inverse(lookAt(UIPos, cameraPosition, cameraUp))
			* translate(mat4(1.0f), vec3(30.0f, -40.0f, 0.0f))
			* rotate(mat4(1.0f), radians(180.0f), vec3(0.0f, 1.0f, 0.0f));
		flashlightUIModel->draw(drawUIAT, renderingMode, glGetUniformLocation(phongLightShaderProgram, "lightCoefficients"), glGetUniformLocation(phongLightShaderProgram, "lightColor"));

		drawUIAT = inverse(lookAt(UIPos, cameraPosition, cameraUp))
			* translate(mat4(1.0f), vec3(30.0f, -35.0f, 0.0f))
			* rotate(mat4(1.0f), radians(180.0f), vec3(0.0f, 1.0f, 0.0f));
		controlsUIModel->draw(drawUIAT, renderingMode, glGetUniformLocation(phongLightShaderProgram, "lightCoefficients"), glGetUniformLocation(phongLightShaderProgram, "lightColor"));

		drawUIAT = inverse(lookAt(UIPos, cameraPosition, cameraUp))
			* translate(mat4(1.0f), vec3(30.0f, -28.0f, 0.0f))
			* rotate(mat4(1.0f), radians(180.0f), vec3(0.0f, 1.0f, 0.0f));
		controlLabelUIModel->draw(drawUIAT, renderingMode, glGetUniformLocation(phongLightShaderProgram, "lightCoefficients"), glGetUniformLocation(phongLightShaderProgram, "lightColor"));
		glEnable(GL_DEPTH_TEST);
#pragma endregion

        char title[10]; // F P S : _ X X . X \0
        snprintf(title, sizeof title, "FPS: %f", 1.0f/dt);
        // display FPS in title
        glfwSetWindowTitle(window, title);

		// End Frame, include swap interval to prevent blurriness
		glfwSwapBuffers(window);
		glfwSwapInterval(1);
		glfwPollEvents();

		// Handle inputs
		handleRenderingModeInput(window);
		handleExitInput(window);
		handleCameraPositionInputs(window);

#pragma region mouseCameraMovement
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

		int slowingFactor = 3;

		cameraHorizontalAngle += (cameraAngularSpeed * -1 * dt * dx) / slowingFactor;
		cameraVerticalAngle += (cameraAngularSpeed * -1 * dt * dy) / slowingFactor;


		if (doRaycastCollision(viewMatrix, collisionModels) && currentCamFacingMovement > 0.0f) //The player is trying to move forward into an object, allow only strafing or backwards movement
		{
			cameraPosition = vec3(cameraPosition.x, cameraPosition.y, cameraPosition.z)
				+ vec3(cameraSideVector.x * currentCamStrafingMovement, 0.0f, cameraSideVector.z * currentCamStrafingMovement);
			viewMatrix = lookAt(cameraPosition, cameraPosition + cameraLookAt, cameraUp);
		}
		else {
			cameraPosition = vec3(cameraPosition.x, cameraPosition.y, cameraPosition.z)
				+ vec3(cameraLookAt.x * currentCamFacingMovement, 0.0f, cameraLookAt.z * currentCamFacingMovement)
				+ vec3(cameraSideVector.x * currentCamStrafingMovement, 0.0f, cameraSideVector.z * currentCamStrafingMovement);
			viewMatrix = lookAt(cameraPosition, cameraPosition + cameraLookAt, cameraUp);
		}

		currentCamStrafingMovement = 0;
		currentCamFacingMovement = 0;
		vec3 flashlightPosition = cameraPosition + vec3(0.0f, -5.0f, 0.0f);
		updateFlashlight(flashlightPosition, flashlightPosition + cameraLookAt + vec3(0.0f, 0.2f, 0.0f));

#pragma endregion
	}

	// Shutdown GLFW
	glfwTerminate();

	return 0;
}

void setRandomizedPositionScale(mat4& modelMatrix, Terrain terrain) {

	float scaleFactor = getRandomNumber(0.5, 1.5);

	//TODO use Antoine's system to place the letters/numbers
	float xRandTranslate = getRandomNumber(-100.0f, 100.0f);
	float zRandTranslate = getRandomNumber(-100.0f, 100.0f);

	vec3 position = vec3(xRandTranslate, terrain.getHeightAt(xRandTranslate, zRandTranslate), zRandTranslate);
	position = vec3(0, terrain.getHeightAt(0, 0) * 100, 0);

	// Building L9 scalable/translatable/rotateable matrix for individual letter
	mat4 modelScalingMatrix = scale(mat4(1.0f), vec3(scaleFactor));
	mat4 modelTranslationMatrix = translate(mat4(1.0f), position);
	modelMatrix = modelTranslationMatrix * modelScalingMatrix;
}
