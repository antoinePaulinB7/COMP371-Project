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

	layout (std140) uniform LightInfo
	{
		mat4 depthVP;						// 16		// 0 (column 0)
											// 16		// 16 (column 1)
											// 16		// 32 (column 2)
											// 16		// 48 (column 3)
		vec3 lightPosition;					// 16		// 64

		mat4 depthVP2;						// 16		// 80 (column 0)
											// 16		// 96 (column 1)
											// 16		// 112 (column 2)
											// 16		// 128 (column 3)
		vec3 lightPosition2;				// 16		// 144
											//total = 160
	};


    out vec2 texCoord;
    out vec3 vertexColor;
    out vec3 normalN;
    out vec3 fragPosition;
    out vec3 eyeVectorV;

	out light1 {
		vec3 lightVectorL;
		vec4 shadowCoordinate;
		float distanceToLightSource;
	} light1;

	out light2 {
		vec3 lightVectorL;
		vec4 shadowCoordinate;
		float distanceToLightSource;
	} light2;

    void main()
    {
      vertexColor = aColor;
      texCoord = aTex;

      //Transform vertex positions to Clip Space
      mat4 mvp = projectionMatrix * viewMatrix * worldMatrix;
        gl_Position = mvp * vec4(aPos, 1.0f);
		
		//used this reference https://learnopengl.com/Lighting/Basic-Lighting
		//Light math
		fragPosition = vec3(worldMatrix * vec4(aPos, 1.0));
		normalN = vec3(worldMatrix * vec4(aNormal, 0.0f));
		eyeVectorV = normalize(vec3(vec4((cameraPosition - fragPosition), 0.0f)));
		

		light1.lightVectorL = lightPosition;
		vec3 vectorToLightSource = lightPosition - fragPosition;
		light1.distanceToLightSource = length(vectorToLightSource);
		light1.shadowCoordinate = depthVP * worldMatrix * vec4(aPos, 1.0f);

		light2.lightVectorL = lightPosition2;
		vec3 vectorToLightSource2 = lightPosition2 - fragPosition;
		light2.distanceToLightSource = length(vectorToLightSource2);
		light2.shadowCoordinate = depthVP2 * worldMatrix * vec4(aPos, 1.0f);
    }
