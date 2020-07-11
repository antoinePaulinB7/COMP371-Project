//
// COMP 371 Labs Framework
//
// Created by Nicolas Bergeron on 20/06/2019.
//
// Inspired by the following tutorials:
// - https://learnopengl.com/Getting-started/Hello-Window
// - https://learnopengl.com/Getting-started/Hello-Triangle

#include <iostream>


#define GLEW_STATIC 1   // This allows linking with Static Library on Windows, without DLL
#include <GL/glew.h>    // Include GLEW - OpenGL Extension Wrangler

#include <GLFW/glfw3.h> // GLFW provides a cross-platform interface for creating a graphical context,
                        // initializing OpenGL and binding inputs

#include <glm/glm.hpp>  // GLM is an optimized math library with syntax to similar to OpenGL Shading Language
#include <glm/gtc/matrix_transform.hpp> // include this to create transformation matrices

#include <algorithm>
using namespace std;
#include <list>

const char* getVertexShaderSource()
{
    // For now, you use a string for your shader code, in the assignment, shaders will be stored in .glsl files
    return
                "#version 330 core\n"
                "layout (location = 0) in vec3 aPos;"
                "layout (location = 1) in vec3 aColor;"
                ""
                "uniform mat4 worldMatrix;"
                "uniform mat4 viewMatrix = mat4(1.0);"  // default value for view matrix (identity)
                "uniform mat4 projectionMatrix = mat4(1.0);"
                ""
                "out vec3 vertexColor;"
                "void main()"
                "{"
                "   vertexColor = aColor;"
                "   mat4 modelViewProjection = projectionMatrix * viewMatrix * worldMatrix;"
                "   gl_Position = modelViewProjection * vec4(aPos.x, aPos.y, aPos.z, 1.0);"
                "}";
}


const char* getFragmentShaderSource()
{
    return
                "#version 330 core\n"
                "in vec3 vertexColor;"
                "out vec4 FragColor;"
                "void main()"
                "{"
                "   FragColor = vec4(vertexColor.r, vertexColor.g, vertexColor.b, 1.0f);"
                "}";
}


int compileAndLinkShaders()
{
    // compile and link shader program
    // return shader program id
    // ------------------------------------

    // vertex shader
    int vertexShader = glCreateShader(GL_VERTEX_SHADER);
    const char* vertexShaderSource = getVertexShaderSource();
    glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);
    glCompileShader(vertexShader);
    
    // check for shader compile errors
    int success;
    char infoLog[512];
    glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
    if (!success)
    {
        glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
        std::cerr << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << infoLog << std::endl;
    }
    
    // fragment shader
    int fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    const char* fragmentShaderSource = getFragmentShaderSource();
    glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);
    glCompileShader(fragmentShader);
    
    // check for shader compile errors
    glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);
    if (!success)
    {
        glGetShaderInfoLog(fragmentShader, 512, NULL, infoLog);
        std::cerr << "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n" << infoLog << std::endl;
    }
    
    // link shaders
    int shaderProgram = glCreateProgram();
    glAttachShader(shaderProgram, vertexShader);
    glAttachShader(shaderProgram, fragmentShader);
    glLinkProgram(shaderProgram);
    
    // check for linking errors
    glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
    if (!success) {
        glGetProgramInfoLog(shaderProgram, 512, NULL, infoLog);
        std::cerr << "ERROR::SHADER::PROGRAM::LINKING_FAILED\n" << infoLog << std::endl;
    }
    
    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);
    
    return shaderProgram;
}

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
      
      
		// Draw alphanumeric models
		glBindBuffer(GL_ARRAY_BUFFER, unitCubeVBO);

		GLuint worldMatrixLocation = glGetUniformLocation(shaderProgram, "worldMatrix");

#pragma region L9
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
#pragma endregion

        // End Frame
        glfwSwapBuffers(window);
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