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

	layout (std140) uniform DepthVPMatrix
	{
		mat4 depthVP;		// 16		// 0 (column 0)
							// 16		// 16 (column 1)
							// 16		// 32 (column 2)
							// 16		// 48 (column 3)
							//total = 64
	};

    void main()
    {
		mat4 mvp = depthVP * worldMatrix;
        gl_Position = mvp * vec4(aPos, 1.0f);
    }
