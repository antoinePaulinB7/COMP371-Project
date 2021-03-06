#version 330 core
    layout (location = 0) in vec3 aPos; 
	layout (location = 1) in vec3 aColor;
    uniform mat4 worldMatrix;
	uniform mat4 viewMatrix = mat4(1.0);
	uniform mat4 projectionMatrix = mat4(1.0);
    out vec3 vertexColor;

    void main()
    {
		mat4 mvp = projectionMatrix * viewMatrix * worldMatrix;
        gl_Position = mvp * vec4(aPos, 1.0f);
        vertexColor = aColor;
    }
