#version 330 core
    layout (location = 0) in vec3 aPos; 
	layout (location = 1) in vec3 aColor;
	layout (location = 2) in vec3 aNormal;
    layout (location = 3) in vec2 aTex;
	
    uniform vec3 cameraPosition;
    uniform mat4 worldMatrix;
    uniform mat4 viewMatrix = mat4(1.0);
    uniform mat4 projectionMatrix = mat4(1.0);

    uniform mat4 depthVP;
    uniform vec3 lightPosition;
    
	uniform mat4 depthVP2;
    uniform vec3 lightPosition2 = vec3(0.0f);

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
