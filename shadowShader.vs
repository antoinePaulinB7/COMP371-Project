#version 330 core
    layout (location = 0) in vec3 aPos; 
    
	layout (std140) uniform WorldMatrix
	{
		mat4 worldMatrix;	// 16		// 0 (column 0)
							// 16		// 16 (column 1)
							// 16		// 32 (column 2)
							// 16		// 48 (column 3)
							//total = 64
	};

	uniform mat4 viewMatrix = mat4(1.0);
    uniform mat4 projectionMatrix = mat4(1.0);
	/*
	layout (std140) uniform DepthVPMatrices
	{
		mat4 depthProjectionMatrix = mat4(1.0);	// 16		// 0 (column 0)
												// 16		// 16 (column 1)
												// 16		// 32 (column 2)
												// 16		// 48 (column 3)
		mat4 depthViewMatrix = mat4(1.0);		// 16		// 64 (column 0)
												// 16		// 80 (column 1)
												// 16		// 96 (column 2)
												// 16		// 112 (column 3)
												//total = 128
	};
	*/

    void main()
    {
		mat4 mvp = projectionMatrix * viewMatrix * worldMatrix;
        gl_Position = mvp * vec4(aPos, 1.0f);
    }
