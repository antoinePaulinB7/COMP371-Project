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
		mat4 depthVP;					// 16		// 0 (column 0)
										// 16		// 16 (column 1)
										// 16		// 32 (column 2)
										// 16		// 48 (column 3)
		vec3 lightPosition;				// 16		// 60
		vec3 lightPointingDirection;	// 16		// 72
										//total = 96 per light

		mat4 depthVP2;	
		vec3 lightPosition2;	
		vec3 lightPointingDirection2;

		mat4 depthVP3;	
		vec3 lightPosition3;	
		vec3 lightPointingDirection3;

		mat4 depthVP4;	
		vec3 lightPosition4;	
		vec3 lightPointingDirection4;

		mat4 depthVP5;	
		vec3 lightPosition5;	
		vec3 lightPointingDirection5;

		mat4 depthVP6;	
		vec3 lightPosition6;	
		vec3 lightPointingDirection6;

		mat4 depthVP7;	
		vec3 lightPosition7;	
		vec3 lightPointingDirection7;
	};


    out vec2 texCoord;
    out vec3 vertexColor;
    out vec3 normalN;
    out vec3 fragPosition;
    out vec3 eyeVectorV;

	out LIGHT_1 {
		vec3 lightVectorL;
		vec3 lightPointingDir;
		vec4 shadowCoordinate;
		float distanceToLightSource;
	} light1;

	out LIGHT_2 {
		vec3 lightVectorL;
		vec3 lightPointingDir;
		vec4 shadowCoordinate;
		float distanceToLightSource;
	} light2;

	out LIGHT_3 {
		vec3 lightVectorL;
		vec3 lightPointingDir;
		vec4 shadowCoordinate;
		float distanceToLightSource;
	} light3;

	out LIGHT_4 {
		vec3 lightVectorL;
		vec3 lightPointingDir;
		vec4 shadowCoordinate;
		float distanceToLightSource;
	} light4;

	out LIGHT_5 {
		vec3 lightVectorL;
		vec3 lightPointingDir;
		vec4 shadowCoordinate;
		float distanceToLightSource;
	} light5;

	out LIGHT_6 {
		vec3 lightVectorL;
		vec3 lightPointingDir;
		vec4 shadowCoordinate;
		float distanceToLightSource;
	} light6;

	out LIGHT_7 {
		vec3 lightVectorL;
		vec3 lightPointingDir;
		vec4 shadowCoordinate;
		float distanceToLightSource;
	} light7;

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
		light1.lightPointingDir = lightPointingDirection;

		light2.lightVectorL = lightPosition2;
		vec3 vectorToLightSource2 = lightPosition2 - fragPosition;
		light2.distanceToLightSource = length(vectorToLightSource2);
		light2.shadowCoordinate = depthVP2 * worldMatrix * vec4(aPos, 1.0f);
		light2.lightPointingDir = lightPointingDirection2;

		light3.lightVectorL = lightPosition3;
		vec3 vectorToLightSource3 = lightPosition3 - fragPosition;
		light3.distanceToLightSource = length(vectorToLightSource3);
		light3.shadowCoordinate = depthVP3 * worldMatrix * vec4(aPos, 1.0f);
		light3.lightPointingDir = lightPointingDirection3;

		light4.lightVectorL = lightPosition4;
		vec3 vectorToLightSource4 = lightPosition4 - fragPosition;
		light4.distanceToLightSource = length(vectorToLightSource4);
		light4.shadowCoordinate = depthVP4 * worldMatrix * vec4(aPos, 1.0f);
		light4.lightPointingDir = lightPointingDirection4;

		light5.lightVectorL = lightPosition5;
		vec3 vectorToLightSource5 = lightPosition5 - fragPosition;
		light5.distanceToLightSource = length(vectorToLightSource5);
		light5.shadowCoordinate = depthVP5 * worldMatrix * vec4(aPos, 1.0f);
		light5.lightPointingDir = lightPointingDirection5;

		light6.lightVectorL = lightPosition6;
		vec3 vectorToLightSource6 = lightPosition6 - fragPosition;
		light6.distanceToLightSource = length(vectorToLightSource6);
		light6.shadowCoordinate = depthVP6 * worldMatrix * vec4(aPos, 1.0f);
		light6.lightPointingDir = lightPointingDirection6;

		light7.lightVectorL = lightPosition7;
		vec3 vectorToLightSource7 = lightPosition7 - fragPosition;
		light7.distanceToLightSource = length(vectorToLightSource7);
		light7.shadowCoordinate = depthVP7 * worldMatrix * vec4(aPos, 1.0f);
		light7.lightPointingDir = lightPointingDirection7;
    }
