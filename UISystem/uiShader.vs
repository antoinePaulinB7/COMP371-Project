#version 330 core
    layout (location = 0) in vec3 aPos; 
	layout (location = 1) in vec3 aColor;
	layout (location = 2) in vec3 aNormal;
    layout (location = 3) in vec2 aTex;
	
    uniform vec3 cameraPosition;
    uniform mat4 viewMatrix = mat4(1.0);
    uniform mat4 projectionMatrix = mat4(1.0);

	layout (std140) uniform WorldMatrix
	{
		mat4 worldMatrix;	// 16		// 0 (column 0)
							// 16		// 16 (column 1)
							// 16		// 32 (column 2)
							// 16		// 48 (column 3)
							//total = 64
	};

    out vec2 texCoord;
    out vec3 vertexColor;

    void main()
    {
		vertexColor = aColor;
		texCoord = aTex;

		//Transform vertex positions to Clip Space
		mat4 mvp = projectionMatrix * viewMatrix * worldMatrix;
        gl_Position = mvp * vec4(aPos, 1.0f);
    }
