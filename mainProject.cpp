#include <iostream>
#include <list>
#define GLEW_STATIC 1

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/common.hpp>
#include "vertices.h"
#include "shaders.h"
#include <time.h>
#include <algorithm>
#include <list>

//define namespaces for glm and c++ std
using namespace glm;
using namespace std;

//storing the redering mode in a variable 
	int renderingMode = GL_TRIANGLES;

int createUnitCubeVertexBufferObject()
{
	// Cube model
	glm::vec3 vertexArray[] = {  // position,                            color
		glm::vec3(-0.5f,-0.5f,-0.5f), glm::vec3(0.7f, 0.7f, 0.7f), //left - gray
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


	return vertexBufferObject;
}

// Camera parameters
float cameraSpeed = 1.0f;
float cameraFastSpeed = 2 * cameraSpeed;
float cameraHorizontalAngle = 90.0f;
float cameraVerticalAngle = 0.0f;
float FOV = 70.0f;
const float cameraAngularSpeed = 60.0f;
bool cameraFirstPerson = true, xMoveMode = false, angleMoveMode = false, zoomMoveMode = false, fastCam = false;

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
		xMoveMode = true;
	}

	// On key down , unset movement mode flag
	if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_RIGHT) == GLFW_RELEASE)
	{
		xMoveMode = false;
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

int createVertexBufferObjectU3()
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


    return vertexBufferObject;
}

int main(int argc, char*argv[])
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
    GLFWwindow* window = glfwCreateWindow(800, 600, "Comp371 - Lab 03", NULL, NULL);
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
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);

    // Compile and link shaders here ...
    int shaderProgram = compileAndLinkShaders();
  
	// We can set the shader once, since we have only one
	glUseProgram(shaderProgram);
#pragma endregion windowSetUp

	// Camera parameters for view transform
	glm::vec3 cameraPosition(0.6f, 1.0f, 10.0f);
	glm::vec3 cameraLookAt(0.0f, 0.0f, -1.0f);
	glm::vec3 cameraUp(0.0f, 1.0f, 0.0f);

	// Set projection matrix for shader, this won't change
	glm::mat4 projectionMatrix = glm::perspective(70.0f,            // field of view in degrees
		800.0f / 600.0f,  // aspect ratio
		0.01f, 100.0f);   // near and far (near > 0)

	GLuint projectionMatrixLocation = glGetUniformLocation(shaderProgram, "projectionMatrix");
	glUniformMatrix4fv(projectionMatrixLocation, 1, GL_FALSE, &projectionMatrix[0][0]);


	// Set initial view matrix
	glm::mat4 viewMatrix = lookAt(cameraPosition,  // eye
		cameraPosition + cameraLookAt,  // center
		cameraUp); // up

	GLuint viewMatrixLocation = glGetUniformLocation(shaderProgram, "viewMatrix");
	glUniformMatrix4fv(viewMatrixLocation, 1, GL_FALSE, &viewMatrix[0][0]);

  //define identity matrices to translate, rotate and scale I9 model
	//used for hierarchical modeling 
	mat4 translateI9Model(1.f);
	mat4 scaleI9Model(1.f);
	mat4 rotateI9Model = rotate(mat4(1.0f), radians(90.0f), vec3(00.0f, 90.0f, 0.0f));
  
  	GLuint model_Shader = Shader("G:/COMP 371/Lab02FullFramework/Lab_Framework/Source/modelShader.vs", 
		"G:/COMP 371/Lab02FullFramework/Lab_Framework/Source/modelShader.fs");
  
  	//Define and upload geometry to the GPU here by creating a VAO and VBO that has multiple objects
	//This way we can store the geometry of all the objects at different indices.
	//adapted from the online tutorial: https://learnopengl.com/Getting-started/Hello-Triangle
	GLuint VAOs[3], VBOs[3];
	glGenVertexArrays(3, VAOs);
	glGenBuffers(3, VBOs);

	/* IGNORE
	//grid vertices
	glBindVertexArray(VAOs[0]);
	glBindBuffer(GL_ARRAY_BUFFER, VBOs[0]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(grid_vertices), grid_vertices, GL_STATIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, (void*)0); 

	//axis vertices
	glBindVertexArray(VAOs[1]);
	glBindBuffer(GL_ARRAY_BUFFER, VBOs[1]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(axis_vertices), axis_vertices, GL_STATIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);  
	*/

	//model vertices
	glBindVertexArray(VAOs[2]);
	glBindBuffer(GL_ARRAY_BUFFER, VBOs[2]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(model_vertices), model_vertices, GL_STATIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);
  
	// Define and upload geometry to the GPU here ...
	int unitCubeVBO = createUnitCubeVertexBufferObject();    
  int cubeVBO = createVertexBufferObjectU3();
  
	// For frame time
	float lastFrameTime = glfwGetTime();
	int lastMouseLeftState = GLFW_RELEASE;
	double lastMousePosX, lastMousePosY;
	glfwGetCursorPos(window, &lastMousePosX, &lastMousePosY);

	// Enable Backface culling and depth test
	glEnable(GL_CULL_FACE);
	glEnable(GL_DEPTH_TEST);

    // Entering Main Loop
    while (!glfwWindowShouldClose(window))
    {
		// Frame time calculation
		float dt = glfwGetTime() - lastFrameTime;
		lastFrameTime += dt;

        // Each frame, reset color of each pixel to glClearColor
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
       
      
		// Draw alphanumeric models
		glBindBuffer(GL_ARRAY_BUFFER, unitCubeVBO);

		GLuint worldMatrixLocation = glGetUniformLocation(shaderProgram, "worldMatrix");

		//Set your flag true to draw your alphanumeric model
		bool drawL9 = false, drawU3 = false;

#pragma region L9
		if (drawL9) {
			// Draw L9
			glm::mat4 letterL1 = translate(glm::mat4(1.0f), glm::vec3(-3.0f, 0.0f, 0.0f)) * scale(glm::mat4(1.0f), glm::vec3(1.0f, 5.0f, 1.0f));
			glUniformMatrix4fv(worldMatrixLocation, 1, GL_FALSE, &letterL1[0][0]);
			glDrawArrays(GL_TRIANGLES, 0, 36);
			glm::mat4 letterL2 = translate(glm::mat4(1.0f), glm::vec3(-1.5f, -2.0f, 0.0f)) * scale(glm::mat4(1.0f), glm::vec3(2.0f, 1.0f, 1.0f));
			glUniformMatrix4fv(worldMatrixLocation, 1, GL_FALSE, &letterL2[0][0]);
			glDrawArrays(GL_TRIANGLES, 0, 36);
			glm::mat4 nine1 = translate(glm::mat4(1.0f), glm::vec3(3.0f, 0.0f, 0.0f)) * scale(glm::mat4(1.0f), glm::vec3(1.0f, 5.0f, 1.0f));
			glUniformMatrix4fv(worldMatrixLocation, 1, GL_FALSE, &nine1[0][0]);
			glDrawArrays(GL_TRIANGLES, 0, 36);
			glm::mat4 nine2 = translate(glm::mat4(1.0f), glm::vec3(1.5f, 2.0f, 0.0f)) * scale(glm::mat4(1.0f), glm::vec3(2.0f, 1.0f, 1.0f));
			glUniformMatrix4fv(worldMatrixLocation, 1, GL_FALSE, &nine2[0][0]);
			glDrawArrays(GL_TRIANGLES, 0, 36);
			glm::mat4 nine3 = translate(glm::mat4(1.0f), glm::vec3(1.0f, 0.5f, 0.0f)) * scale(glm::mat4(1.0f), glm::vec3(1.0f, 2.0f, 1.0f));
			glUniformMatrix4fv(worldMatrixLocation, 1, GL_FALSE, &nine3[0][0]);
			glDrawArrays(GL_TRIANGLES, 0, 36);
			glm::mat4 nine4 = translate(glm::mat4(1.0f), glm::vec3(2.0f, 0.0f, 0.0f)) * scale(glm::mat4(1.0f), glm::vec3(1.0f, 1.0f, 1.0f));
			glUniformMatrix4fv(worldMatrixLocation, 1, GL_FALSE, &nine4[0][0]);
			glDrawArrays(GL_TRIANGLES, 0, 36);
		}
#pragma endregion

#pragma region U3
		if (drawU3) {
			glBindBuffer(GL_ARRAY_BUFFER, cubeVBO);

			/* Transforming the Unit Triangle - La(u)ra Wheatley 400(3)4960 */

			// Creating U from unit cube
			glm::mat4 uLeft = translate(glm::mat4(1.0f), glm::vec3(-3.0f, 0.0f, 0.0f)) * scale(glm::mat4(1.0f), glm::vec3(1.0f, 5.0f, 1.0f));
			glUniformMatrix4fv(worldMatrixLocation, 1, GL_FALSE, &uLeft[0][0]);
			glDrawArrays(GL_TRIANGLES, 0, 36);
			glm::mat4 uBottom = translate(glm::mat4(1.0f), glm::vec3(-1.5f, -2.0f, 0.0f)) * scale(glm::mat4(1.0f), glm::vec3(2.0f, 1.0f, 1.0f));
			glUniformMatrix4fv(worldMatrixLocation, 1, GL_FALSE, &uBottom[0][0]);
			glDrawArrays(GL_TRIANGLES, 0, 36);
			glm::mat4 uRight = translate(glm::mat4(1.0f), glm::vec3(0.0f, 0.0f, 0.0f)) * scale(glm::mat4(1.0f), glm::vec3(1.0f, 5.0f, 1.0f));
			glUniformMatrix4fv(worldMatrixLocation, 1, GL_FALSE, &uRight[0][0]);
			glDrawArrays(GL_TRIANGLES, 0, 36);


			// Creating 3 from unit cube
			glm::mat4 threeBase = translate(glm::mat4(1.0f), glm::vec3(4.0f, 0.0f, 0.0f)) * scale(glm::mat4(1.0f), glm::vec3(1.0f, 5.0f, 1.0f));
			glUniformMatrix4fv(worldMatrixLocation, 1, GL_FALSE, &threeBase[0][0]);
			glDrawArrays(GL_TRIANGLES, 0, 36);
			glm::mat4 threeArm1 = translate(glm::mat4(1.0f), glm::vec3(2.5f, 2.0f, 0.0f)) * scale(glm::mat4(1.0f), glm::vec3(2.0f, 1.0f, 1.0f));
			glUniformMatrix4fv(worldMatrixLocation, 1, GL_FALSE, &threeArm1[0][0]);
			glDrawArrays(GL_TRIANGLES, 0, 36);
			glm::mat4 threeArm2 = translate(glm::mat4(1.0f), glm::vec3(2.5f, -2.0f, 0.0f)) * scale(glm::mat4(1.0f), glm::vec3(2.0f, 1.0f, 1.0f));
			glUniformMatrix4fv(worldMatrixLocation, 1, GL_FALSE, &threeArm2[0][0]);
			glDrawArrays(GL_TRIANGLES, 0, 36);
			glm::mat4 threeArm3 = translate(glm::mat4(1.0f), glm::vec3(3.0f, 0.0f, 0.0f)) * scale(glm::mat4(1.0f), glm::vec3(1.0f, 1.0f, 1.0f));
			glUniformMatrix4fv(worldMatrixLocation, 1, GL_FALSE, &threeArm3[0][0]);
			glDrawArrays(GL_TRIANGLES, 0, 36);
		}
#pragma endregion

#pragma region I9
      		//setting up the MVP of the world to place the objects 
		modelViewProjection = projectionMatrix * viewMatrix * modelMatrix;

		//----------------------------------------------------------------------------------
		//geometry for the I9 model
		glEnableVertexAttribArray(0);
		glUseProgram(model_Shader);
		glBindVertexArray(VAOs[2]);


		//get the worldview of the model within the scene
		mat4 WorldView_Model = projectionMatrix * viewMatrix * translateI9Model * scaleI9Model * rotateI9Model;

		//get the mvp of the model and uniform color variable so we can use it to make the I and 9.
		GLuint modelViewProjection_Model = glGetUniformLocation(model_Shader, "mvp");
		GLuint model_Color = glGetUniformLocation(model_Shader, "model_color");

		//topI
		mat4 topI = WorldView_Model * translate(mat4(1.0f), vec3(0.f, 4.5f, -3.f))* scale(mat4(1.0f), vec3(1.0f, 1.0f, 4.0f));
		glUniformMatrix4fv(modelViewProjection_Model, 1, GL_FALSE, &topI[0][0]);
		glUniform4f(model_Color, 0.5f, 0.5f, 0.5f, 1.0f);
		glDrawArrays(renderingMode, 0, 36);


		//middleI
		mat4 middleI = WorldView_Model * translate(mat4(1.0f), vec3(0.f, 2.5f, -3.f)) * scale(mat4(1.0f), vec3(1.0f, 3.0f, 1.0f));
		glUniformMatrix4fv(modelViewProjection_Model, 1, GL_FALSE, &middleI[0][0]);
		glUniform4f(model_Color, 0.5f, 0.5f, 0.5f, 1.0f);
		glDrawArrays(renderingMode, 0, 36);

		//bottomI
		mat4 bottomI = WorldView_Model * translate(mat4(1.0f), vec3(0.f, 0.5f, -3.f)) * scale(mat4(1.0f), vec3(1.0f, 1.0f, 4.0f));
		glUniformMatrix4fv(modelViewProjection_Model, 1, GL_FALSE, &bottomI[0][0]);
		glUniform4f(model_Color, 0.5f, 0.5f, 0.5f, 1.0f);
		glDrawArrays(renderingMode, 0, 36);

		//top9
		mat4 top9 = WorldView_Model * translate(mat4(1.0f), vec3(0.f, 4.5f, 2.75f)) * scale(mat4(1.0f), vec3(1.0f, 1.0f, 1.5f));
		glUniformMatrix4fv(modelViewProjection_Model, 1, GL_FALSE, &top9[0][0]);
		glUniform4f(model_Color, 0.5f, 0.5f, 0.5f, 1.0f);
		glDrawArrays(renderingMode, 0, 36);


		//left9
		mat4 left9 = WorldView_Model * translate(mat4(1.0f), vec3(0.f, 3.25f, 1.5f)) * scale(mat4(1.0f), vec3(1.0f, 3.5f, 1.0f));
		glUniformMatrix4fv(modelViewProjection_Model, 1, GL_FALSE, &left9[0][0]);
		glUniform4f(model_Color, 0.5f, 0.5f, 0.5f, 1.0f);
		glDrawArrays(renderingMode, 0, 36);


		//bottom9
		mat4 bottom9 = WorldView_Model * translate(mat4(1.0f), vec3(0.f, 2.0f, 2.75f)) * scale(mat4(1.0f), vec3(1.0f, 1.0f, 1.5f));
		glUniformMatrix4fv(modelViewProjection_Model, 1, GL_FALSE, &bottom9[0][0]);
		glUniform4f(model_Color, 0.5f, 0.5f, 0.5f, 1.0f);
		glDrawArrays(renderingMode, 0, 36);


		//right9
		mat4 right9 = WorldView_Model * translate(mat4(1.0f), vec3(0.f, 2.51f, 4.0f)) * scale(mat4(1.0f), vec3(1.0f, 5.f, 1.0f));
		glUniformMatrix4fv(modelViewProjection_Model, 1, GL_FALSE, &right9[0][0]);
		glUniform4f(model_Color, 0.5f, 0.5f, 0.5f, 1.0f);
		glDrawArrays(renderingMode, 0, 36);
		//----------------------------------------------------------------------------------
#pragma endregion

      		// End Frame, include swap interval to prevent blurriness
		glfwSwapBuffers(window);
		glfwSwapInterval(1);
		glfwPollEvents();

        // Handle inputs
		handleCameraFlagInputs(window);
        if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
            glfwSetWindowShouldClose(window, true);

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

        /* Begin Part 2 - SIMULTANEOUS MOUSE AND KEY movement */      
        if (xMoveMode)
        {
            // If mouse position goes toward positive axis, increase cameraposition
            if (dx > 0) {
                cameraPosition.x += currentCameraSpeed * dx * dt;
            }
            // If position goes toward negative axis, increase cameraposition
            else if (dx < 0) {
                cameraPosition.x -= currentCameraSpeed * (-1) * dx * dt;
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

        /*
         *  NOTE: in zoomMoveMode, I chose to manipulate the FOV to zoom. IF we slowly increase/decrease the FOV linearly,
         *  the view goes upside down then right side up again depending on the values. So here, I have chosen three
         *  "safe" values we can jump around to: 90.0f, 70.0f, and 0.5f.
         */

        if (zoomMoveMode)
        {
            if (dy > 0) {
                if (FOV == 70.0f) {
                    FOV = 90.0f;
                }
                else if (FOV == 0.5f) {
                    FOV = 70.f;
                }
            }

            else if (dy < 0) {
                if (FOV == 90.0f) {
                    FOV = 70.0f;
                }
                else if (FOV == 70.0f) {
                    FOV = 0.5f;
                }
            }

			glm::mat4 projectionMatrix = glm::perspective(FOV,            // field of view in degrees
                800.0f / 600.0f,  // aspect ratio
                0.01f, 100.0f);   // near and far (near > 0)

            GLuint projectionMatrixLocation = glGetUniformLocation(shaderProgram, "projectionMatrix");
            glUniformMatrix4fv(projectionMatrixLocation, 1, GL_FALSE, &projectionMatrix[0][0]);

        }

        /* END Part 2 - SIMULTANEOUS MOUSE AND KEY movement */
      
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

		//Changing World Orientation 
		if (glfwGetKey(window, GLFW_KEY_RIGHT) == GLFW_PRESS) //rotate X axis in anti-clockwise direction
		{
			modelMatrix = modelMatrix * rotate(mat4(1.0f), radians(5.0f), vec3(-1.0f, 0.f, 0.f));
		}

		if (glfwGetKey(window, GLFW_KEY_LEFT) == GLFW_PRESS) //rotate X axis in clockwise direction
		{
			modelMatrix = modelMatrix * rotate(mat4(1.0f), radians(5.0f), vec3(1.0f, 0.f, 0.f));
		}

		if (glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS) //rotate Y axis in anti-clockwise direction
		{
			modelMatrix = modelMatrix * rotate(mat4(1.0f), radians(5.0f), vec3(0.0f, -1.f, 0.f));
		}


		if (glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS) //rotate Y axis in clockwise direction
		{
			modelMatrix = modelMatrix * rotate(mat4(1.0f), radians(5.0f), vec3(0.0f, 1.0f, 0.f));
		}

		//reset world orientation to original settings
		//used Tab to test as I do not have a Home button
		if (glfwGetKey(window, GLFW_KEY_TAB) == GLFW_PRESS || glfwGetKey(window, GLFW_KEY_HOME) == GLFW_PRESS) 
		{
			modelMatrix = mat4(1.0f);
		}
		//----------------------------------------------------------------------------------

		if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
			glfwSetWindowShouldClose(window, true);


        // Update viewMatrix
        glm::mat4 viewMatrix = lookAt(cameraPosition, cameraPosition + cameraLookAt, cameraUp);

        // Update viewMatrixLocation
        GLuint viewMatrixLocation = glGetUniformLocation(shaderProgram, "viewMatrix");
        glUniformMatrix4fv(viewMatrixLocation, 1, GL_FALSE, &viewMatrix[0][0]);
    }

    // Shutdown GLFW
    glfwTerminate();

    return 0;
}

