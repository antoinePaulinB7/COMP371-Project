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


//define namespaces for glm and c++ std
using namespace glm;
using namespace std;



int main(int argc, char*argv[])
{
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

	// Create Window and rendering context using GLFW, resolution is 1024x768
	GLFWwindow* window = glfwCreateWindow(1024, 768, "Comp371 - A1", NULL, NULL);
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

	//define identity matrices to translate, rotate and scale I9 model
	//used for hierarchical modeling 
	mat4 translateI9Model(1.f);
	mat4 scaleI9Model(1.f);
	mat4 rotateI9Model = rotate(mat4(1.0f), radians(90.0f), vec3(00.0f, 90.0f, 0.0f));

	// Black background
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
	
	//storing the redering mode in a variable 
	int renderingMode = GL_TRIANGLES;

	/* IGNORE
	//loads shader files and creates shader objects
	GLuint grid_shader = Shader("G:/COMP 371/Lab02FullFramework/Lab_Framework/Source/gridShader.vs", 
		"G:/COMP 371/Lab02FullFramework/Lab_Framework/Source/gridShader.fs");

	GLuint axis_Shader = Shader("G:/COMP 371/Lab02FullFramework/Lab_Framework/Source/axisShader.vs", 
		"G:/COMP 371/Lab02FullFramework/Lab_Framework/Source/axisShader.fs"); 
	*/

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


	//Enable Backface culling
	glEnable(GL_CULL_FACE);
	glEnable(GL_DEPTH_TEST);


	//Camera parameters for view transform
	vec3 cameraPosition(0.0f, 10.0f, 30.0f);
	vec3 cameraLookAt(0.0f, 0.0f, 0.0f);
	vec3 cameraUp(0.0f, 1.0f, 0.0f);

	//Other camera parameters
	float cameraHorizontalAngle = 90.0f;
	float cameraVerticalAngle = 0.0f;


	// Set projection matrix for shader, this won't change
	mat4 projectionMatrix = mat4(1.0f);
	projectionMatrix = perspective(radians(45.0f), 1024.0f / 768.0f, 0.1f, 100.0f);
	// Set initial view matrix
	mat4 viewMatrix = lookAt(cameraPosition, cameraPosition + cameraLookAt, cameraUp);
	mat4 modelMatrix = mat4(1.0f);
	mat4 modelViewProjection = projectionMatrix * viewMatrix * modelMatrix;


	//Entering Main Loop
	while (!glfwWindowShouldClose(window))
	{
		//setting up the MVP of the world to place the objects 
		modelViewProjection = projectionMatrix * viewMatrix * modelMatrix;


		// set the background color to the black  
		glClearColor(0.0f, 0.0f, 0.0f, 1.0f);

		// clear the color and depth buffer at the beginning of each loop
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		/* IGNORE
		//geometry for grid
		glEnableVertexAttribArray(0);
		glUseProgram(grid_shader);
		glBindVertexArray(VAOs[0]);

		//get the MVP of the grid from the shader
		GLuint modelViewProjection_Grid = glGetUniformLocation(grid_shader, "mvp");
		// Draw grid
		for (int j = -50; j < 50; ++j)
		{
			for (int i = -50; i < 50; ++i)
			{
				mat4 grid = modelViewProjection * translate(mat4(1.0f), vec3(i, 0.f, j));
				glUniformMatrix4fv(modelViewProjection_Grid, 1, GL_FALSE, &grid[0][0]);
				glDrawArrays(GL_LINE_LOOP, 0, 4);
			}
		} 


		//geometry for the axis lines
		glEnableVertexAttribArray(0);
		glUseProgram(axis_Shader);
		glBindVertexArray(VAOs[1]);

		//get the MVP of the axis lines from the shader
		//get the uniform color variable to set each line's color
		GLuint modelViewProjection_axis = glGetUniformLocation(axis_Shader, "mvp");
		GLuint axis_color = glGetUniformLocation(axis_Shader, "axis_color");
		//create the MVP of the camera to be placed within the world
		mat4 camera_ModelViewProject_XYZ = projectionMatrix * viewMatrix * modelMatrix;

		glUniformMatrix4fv(modelViewProjection_axis, 1, GL_FALSE, &camera_ModelViewProject_XYZ[0][0]);

		//for loop to draw the 3 lines
		int axis_vertexIndex = 0;
		for (int i = 0; i < 3; i++) {
			glUniform3f(axis_color, axis_Colors[i].x, axis_Colors[i].y, axis_Colors[i].z);
			glDrawArrays(GL_LINES, axis_vertexIndex, 2);
			axis_vertexIndex = axis_vertexIndex + 2;
		} 
		*/

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

		// End Frame, include swap interval to prevent blurriness
		glfwSwapBuffers(window);
		glfwSwapInterval(1);
		glfwPollEvents();


		// Clamp vertical angle to -85,85
		cameraVerticalAngle = max(-85.0f, min(85.0f, cameraVerticalAngle));
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

		cameraLookAt = vec3(cosf(phi)*cosf(theta), sinf(phi), -cosf(phi)*sinf(theta));
		vec3 cameraSideVector = cross(cameraLookAt, vec3(0.0f, 1.0f, 0.0f));
		normalize(cameraSideVector);

		/* IGNORE
		if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS) //move model to the left
		{
			translateI9Model = translateI9Model * translate(mat4(1.0f), vec3(-0.1, 0.0f, 0.0f));
		}

		if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS) //move model to the right
		{
			translateI9Model = translateI9Model * translate(mat4(1.0f), vec3(0.1, 0.0f, 0.0f));
		}

		if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) //move model up
		{
			translateI9Model = translateI9Model * translate(mat4(1.0f), vec3(0.0f, 0.0f, 0.1));
		}

		if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) //move model down
		{
			translateI9Model = translateI9Model * translate(mat4(1.0f), vec3(0.0f, 0.0f, -0.1));
		} 
		*/

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

		viewMatrix = lookAt(cameraPosition, cameraPosition + cameraLookAt, cameraUp);

	}


	// Shutdown GLFW
	glfwTerminate();

	return 0;
}
