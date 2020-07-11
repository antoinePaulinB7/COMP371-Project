//
// COMP 371 Labs Framework
//
// Created by Nicolas Bergeron on 20/06/2019.
//
// Inspired by the following tutorials:
// - https://learnopengl.com/Getting-started/Hello-Window
// - https://learnopengl.com/Getting-started/Hello-Triangle

#include <iostream>
#include <string>
#include <map>

#define GLEW_STATIC 1   // This allows linking with Static Library on Windows, without DLL
#include <GL/glew.h>    // Include GLEW - OpenGL Extension Wrangler

#include <GLFW/glfw3.h> // GLFW provides a cross-platform interface for creating a graphical context,
                        // initializing OpenGL and binding inputs

#include <glm/glm.hpp>  // GLM is an optimized math library with syntax to similar to OpenGL Shading Language
#include <glm/gtc/matrix_transform.hpp> // include this to create transformation matrices

using namespace glm;

// font taken from https://fontstruct.com/fontstructions/show/716744/3_by_5_pixel_font
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

const char* getVertexShaderSource()
{
    // For now, you use a string for your shader code, in the assignment, shaders will be stored in .glsl files
    return
                "#version 330 core\n"
                "layout (location = 0) in vec3 aPos;"
                "layout (location = 1) in vec3 aColor;"
                ""
                "uniform mat4 worldMatrix;"
                "uniform mat4 viewMatrix = mat4(1.0);"
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

int createVertexArrayObject()
{
    // Cube model
    vec3 vertexArray[] = {  // position, color
        vec3(0.0f,0.0f,0.0f), vec3(1.0f, 0.0f, 0.0f), //left - red
        vec3(0.0f,0.0f, 1.0f), vec3(1.0f, 0.0f, 0.0f),
        vec3(0.0f, 1.0f, 1.0f), vec3(1.0f, 0.0f, 0.0f),
        
        vec3(0.0f,0.0f,0.0f), vec3(1.0f, 0.0f, 0.0f),
        vec3(0.0f, 1.0f, 1.0f), vec3(1.0f, 0.0f, 0.0f),
        vec3(0.0f, 1.0f,0.0f), vec3(1.0f, 0.0f, 0.0f),
        
        vec3( 1.0f, 1.0f,0.0f), vec3(0.0f, 0.0f, 1.0f), // far - blue
        vec3(0.0f,0.0f,0.0f), vec3(0.0f, 0.0f, 1.0f),
        vec3(0.0f, 1.0f,0.0f), vec3(0.0f, 0.0f, 1.0f),
        
        vec3( 1.0f, 1.0f,0.0f), vec3(0.0f, 0.0f, 1.0f),
        vec3( 1.0f,0.0f,0.0f), vec3(0.0f, 0.0f, 1.0f),
        vec3(0.0f,0.0f,0.0f), vec3(0.0f, 0.0f, 1.0f),
        
        vec3( 1.0f,0.0f, 1.0f), vec3(0.0f, 1.0f, 1.0f), // bottom - turquoise
        vec3(0.0f,0.0f,0.0f), vec3(0.0f, 1.0f, 1.0f),
        vec3( 1.0f,0.0f,0.0f), vec3(0.0f, 1.0f, 1.0f),
        
        vec3( 1.0f,0.0f, 1.0f), vec3(0.0f, 1.0f, 1.0f),
        vec3(0.0f,0.0f, 1.0f), vec3(0.0f, 1.0f, 1.0f),
        vec3(0.0f,0.0f,0.0f), vec3(0.0f, 1.0f, 1.0f),
        
        vec3(0.0f, 1.0f, 1.0f), vec3(0.0f, 1.0f, 0.0f), // near - green
        vec3(0.0f,0.0f, 1.0f), vec3(0.0f, 1.0f, 0.0f),
        vec3( 1.0f,0.0f, 1.0f), vec3(0.0f, 1.0f, 0.0f),
        
        vec3( 1.0f, 1.0f, 1.0f), vec3(0.0f, 1.0f, 0.0f),
        vec3(0.0f, 1.0f, 1.0f), vec3(0.0f, 1.0f, 0.0f),
        vec3( 1.0f,0.0f, 1.0f), vec3(0.0f, 1.0f, 0.0f),
        
        vec3( 1.0f, 1.0f, 1.0f), vec3(1.0f, 0.0f, 1.0f), // right - purple
        vec3( 1.0f,0.0f,0.0f), vec3(1.0f, 0.0f, 1.0f),
        vec3( 1.0f, 1.0f,0.0f), vec3(1.0f, 0.0f, 1.0f),
        
        vec3( 1.0f,0.0f,0.0f), vec3(1.0f, 0.0f, 1.0f),
        vec3( 1.0f, 1.0f, 1.0f), vec3(1.0f, 0.0f, 1.0f),
        vec3( 1.0f,0.0f, 1.0f), vec3(1.0f, 0.0f, 1.0f),
        
        vec3( 1.0f, 1.0f, 1.0f), vec3(1.0f, 1.0f, 0.0f), // top - yellow
        vec3( 1.0f, 1.0f,0.0f), vec3(1.0f, 1.0f, 0.0f),
        vec3(0.0f, 1.0f,0.0f), vec3(1.0f, 1.0f, 0.0f),
        
        vec3( 1.0f, 1.0f, 1.0f), vec3(1.0f, 1.0f, 0.0f),
        vec3(0.0f, 1.0f,0.0f), vec3(1.0f, 1.0f, 0.0f),
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
                          2*sizeof(vec3), // stride - each vertex contain 2 vec3 (position, color)
                          (void*)0             // array buffer offset
                          );
    glEnableVertexAttribArray(0);


    glVertexAttribPointer(1,                            // attribute 1 matches aColor in Vertex Shader
                          3,
                          GL_FLOAT,
                          GL_FALSE,
                          2*sizeof(vec3),
                          (void*)sizeof(vec3)      // color is offseted a vec3 (comes after position)
                          );
    glEnableVertexAttribArray(1);

    return vertexArrayObject;
}

void drawGrid() {
    glPolygonMode(GL_BACK, GL_LINE);
    glColor3f(1.0, 0.0, 0.0);
    int xsize = 0, ysize = 0;
    for(int j = 0; j < 10; j++)
    {
        xsize = 0;
        for(int i = 0; i < 10; i++)
        {
            glBegin(GL_POLYGON);
                glVertex3f(-50.0+xsize, -50.0+ysize, -5.0);
                glVertex3f(-40.0+xsize, -50.0+ysize, -5.0);
                glVertex3f(-40.0+xsize, -40.0+ysize, -5.0);
                glVertex3f(-50.0+xsize, -40.0+ysize, -5.0);
            glEnd();
            xsize += 10.0;
        }
        ysize += 10.0;
    }
    glFlush();
    // glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
}

void drawLetter(char c, int index, mat4 modelTranslationMatrix, mat4 modelRotationMatrix, mat4 modelScalingMatrix, GLuint worldMatrixLocation, mat4 worldMatrix)
{
    char* letter = alphabet[c];

    mat4 primitiveScalingMatrix = scale(mat4(1.0f), vec3(1.0f));
    mat4 primitiveRotationMatrix = rotate(mat4(1.0f), 0.0f, vec3(1.0f));
    mat4 primitiveTranslationMatrix;

    for(int i = 0; i < 15; i++)
    {
        if (letter[i] == '*')
        {
            primitiveTranslationMatrix = translate(mat4(1.0f), vec3(-3.5f + (index * 4.0f) + (i%3)*1.0f, 1.5 + (ceil(i/3)*-1.0f), -0.5f));

            worldMatrix = modelTranslationMatrix * modelScalingMatrix * modelRotationMatrix * primitiveTranslationMatrix * primitiveRotationMatrix * primitiveScalingMatrix;

            glUniformMatrix4fv(worldMatrixLocation, 1, GL_FALSE, &worldMatrix[0][0]);
            glDrawArrays(GL_TRIANGLES, 0, 36); // 3 vertices, starting at index 0
        }
    }
}

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

    // Create Window and rendering context using GLFW, resolution is 800x600
    GLFWwindow* window = glfwCreateWindow(800, 600, "Comp371 - Lab 02", NULL, NULL);
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
    
    // Define and upload geometry to the GPU here ...
    int vao = createVertexArrayObject();
    
    // Variables to be used later in tutorial
    float angle = 0;
    float rotationSpeed = 90.0f;  // 180 degrees per second
    float lastFrameTime = glfwGetTime();

    vec3 cameraPosition = vec3(0.0f, 0.0f, 5.0f);
    bool projection = false; //false = perspective / true = ortho
    // glEnable(GL_CULL_FACE);

    float walkSpeed = 0.5f;
    float runSpeed = 1.0f;
    float scaleFactor = 0.25f;
    float scaleSpeed = 0.25f;

    float modelXRotationAngle = 0.0f;
    float modelYRotationAngle = 0.0f;
    float modelScaleFactor = 1.0f;
    vec3 modelPosition = vec3(0.0f, 0.0f, 0.0f);

    mat4 modelScalingMatrix;
    mat4 modelRotationMatrix;
    mat4 modelTranslationMatrix;

    bool isRunning = false;
    bool wireframe = false;
    bool spinning = false;

    int lastKeyStateP = GLFW_RELEASE;
    int lastKeyStateF = GLFW_RELEASE;
    int lastKeyStateT = GLFW_RELEASE;

    int textCursor = 0;

    glEnable(GL_DEPTH_TEST);

    // Entering Main Loop
    while(!glfwWindowShouldClose(window))
    {
        // Each frame, reset color of each pixel to glClearColor
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        
        // Draw geometry
        glUseProgram(shaderProgram);
        glBindVertexArray(vao);

        float dt = glfwGetTime() - lastFrameTime;
        lastFrameTime += dt;
        
        //spinning ? (modelRotationAngle + rotationSpeed * dt) : 0; //angles in degrees, but glm expects radians (conversion below)

        modelScalingMatrix = scale(mat4(1.0f), vec3(1.0f, 1.0f, 1.0f) * modelScaleFactor);
        modelRotationMatrix = rotate(mat4(1.0f), radians(modelYRotationAngle), vec3(0.0f, 1.0f, 0.0f)) * rotate(mat4(1.0f), radians(modelXRotationAngle), vec3(1.0f, 0.0f, 0.0f));
        modelTranslationMatrix = translate(mat4(1.0f), modelPosition);

        mat4 worldMatrix = mat4(1.0f);
        GLuint worldMatrixLocation = glGetUniformLocation(shaderProgram, "worldMatrix");

        glUniformMatrix4fv(worldMatrixLocation, 1, GL_FALSE, &worldMatrix[0][0]);

        drawLetter('A', 0, modelTranslationMatrix, modelRotationMatrix, modelScalingMatrix, worldMatrixLocation, worldMatrix);
        drawLetter('A', 1, modelTranslationMatrix, modelRotationMatrix, modelScalingMatrix, worldMatrixLocation, worldMatrix);
        // drawLetter('9', 2, modelTranslationMatrix, modelRotationMatrix, modelScalingMatrix, worldMatrixLocation, worldMatrix);
        // drawLetter('2', 3, modelTranslationMatrix, modelRotationMatrix, modelScalingMatrix, worldMatrixLocation, worldMatrix);

        // drawLetter('F', 4, modelTranslationMatrix, modelRotationMatrix, modelScalingMatrix, worldMatrixLocation, worldMatrix);
        // drawLetter('H', 5, modelTranslationMatrix, modelRotationMatrix, modelScalingMatrix, worldMatrixLocation, worldMatrix);
        // drawLetter('3', 6, modelTranslationMatrix, modelRotationMatrix, modelScalingMatrix, worldMatrixLocation, worldMatrix);
        // drawLetter('1', 7, modelTranslationMatrix, modelRotationMatrix, modelScalingMatrix, worldMatrixLocation, worldMatrix);

        // drawLetter('4', 8, modelTranslationMatrix, modelRotationMatrix, modelScalingMatrix, worldMatrixLocation, worldMatrix);
        // drawLetter('P', 9, modelTranslationMatrix, modelRotationMatrix, modelScalingMatrix, worldMatrixLocation, worldMatrix);
        // drawLetter('5', 10, modelTranslationMatrix, modelRotationMatrix, modelScalingMatrix, worldMatrixLocation, worldMatrix);
        // drawLetter('Z', 11, modelTranslationMatrix, modelRotationMatrix, modelScalingMatrix, worldMatrixLocation, worldMatrix);

        glBindVertexArray(0);
        // End Frame
        glfwSwapBuffers(window);
        glfwPollEvents();
        
        // Handle inputs
        if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
            glfwSetWindowShouldClose(window, true);
        
        //by default, camera is centered at the origin and looks towards negative z-axis
        if (glfwGetKey(window, GLFW_KEY_R) == GLFW_PRESS)
        {
            cameraPosition = vec3(0.0f, 0.0f, 5.0f);
            modelScaleFactor = 1.0f;
            modelPosition = vec3(0.0f);
            modelXRotationAngle = 0.0f;
            modelYRotationAngle = 0.0f;
        }

        if (glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS) {
            isRunning = true;
        } else {
            isRunning = false;
        }

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

        if (glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_RELEASE && glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) // move forwards
        {
            modelPosition += vec3(0.0f, 0.0f, -1.0f) * (isRunning ? runSpeed : walkSpeed) * dt;
        }

        if (glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_RELEASE && glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) // move backwards
        {
            modelPosition += vec3(0.0f, 0.0f, 1.0f) * (isRunning ? runSpeed : walkSpeed) * dt;
        }

        if (glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_RELEASE && glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS) // move left
        {
            modelPosition += vec3(-1.0f, 0.0f, 0.0f) * (isRunning ? runSpeed : walkSpeed) * dt;
        }

        if (glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_RELEASE && glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS) // move right 
        {
            modelPosition += vec3(1.0f, 0.0f, 0.0f) * (isRunning ? runSpeed : walkSpeed) * dt;
        }

        mat4 viewMatrix = lookAt(
            cameraPosition,    // eye
            vec3(0.0f),             // center
            vec3(0.0f, 1.0f, 0.0f));    // up

        GLuint viewMatrixLocation = glGetUniformLocation(shaderProgram, "viewMatrix");
        glUniformMatrix4fv(viewMatrixLocation, 1, GL_FALSE, &viewMatrix[0][0]);
        
        if (lastKeyStateP == GLFW_RELEASE && glfwGetKey(window, GLFW_KEY_P) == GLFW_PRESS) // move right 
        {
            projection = !projection;
        }
        lastKeyStateP = glfwGetKey(window, GLFW_KEY_P);

        if (lastKeyStateF == GLFW_RELEASE && glfwGetKey(window, GLFW_KEY_F) == GLFW_PRESS)
        {
            wireframe = !wireframe;

            if (wireframe) {
                glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
            } else {
                glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
            }
        }
        lastKeyStateF = glfwGetKey(window, GLFW_KEY_F);

        if (glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS && glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
        {
            modelXRotationAngle -= rotationSpeed * dt;
        }

        if (glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS && glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
        {
            modelXRotationAngle += rotationSpeed * dt;
        }

        if (glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS && glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
        {
            modelYRotationAngle += rotationSpeed * dt;
        }

        if (glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS && glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
        {
            modelYRotationAngle -= rotationSpeed * dt;
        }
        // drawGrid();

        // perspective
        if (!projection)
        {
            mat4 projectionMatrix = perspective(
                70.0f,              // FOV in degrees
                800.0f / 600.0f,    // aspect ratio
                0.01f, 100.0f       // near and far (near > 0)
            );
            
            GLuint projectionMatrixLocation = glGetUniformLocation(shaderProgram, "projectionMatrix");
            glUniformMatrix4fv(projectionMatrixLocation, 1, GL_FALSE, &projectionMatrix[0][0]);
        } 
        else // ortholinear
        {
            mat4 projectionMatrix = ortho(
                -4.0f, 4.0f,    // left/right
                -3.0f, 3.0f,    // bottom/top
                -100.0f, 100.0f //near/far (near == 0 is ok for ortho)
            );
            
            GLuint projectionMatrixLocation = glGetUniformLocation(shaderProgram, "projectionMatrix");
            glUniformMatrix4fv(projectionMatrixLocation, 1, GL_FALSE, &projectionMatrix[0][0]);
        }
    }
    
    // Shutdown GLFW
    glfwTerminate();
    
    return 0;
}
