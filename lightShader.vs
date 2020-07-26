#version 330 core
    layout (location = 0) in vec3 aPos; 
	layout (location = 1) in vec3 aColor;
	layout (location = 2) in vec3 aNormal;
	
    uniform vec3 cameraPosition;
    uniform mat4 worldMatrix;
	uniform mat4 viewMatrix = mat4(1.0);
	uniform mat4 projectionMatrix = mat4(1.0);
	uniform mat4 depthVP;
	uniform vec3 lightPosition;

    out vec3 vertexColor;
	out vec3 normalN;
	out vec3 fragPosition;
	out vec3 lightVectorL;
	out vec3 eyeVectorV;
    out vec4 shadowCoordinate;
	out float distanceToLightSource;

    void main()
    {
        vertexColor = aColor;

		//Transform vertex positions to Clip Space
		mat4 mvp = projectionMatrix * viewMatrix * worldMatrix;
        gl_Position = mvp * vec4(aPos, 1.0f);
		
		//used this reference https://learnopengl.com/Lighting/Basic-Lighting
		//Light math
		fragPosition = vec3(worldMatrix * vec4(aPos, 1.0));
		normalN = vec3(worldMatrix * vec4(aNormal, 0.0f));
		lightVectorL = lightPosition;
		eyeVectorV = normalize(vec3(viewMatrix * vec4((cameraPosition - fragPosition), 0.0f)));

		//Distance to Light math
		vec3 vectorToLightSource = lightPosition - fragPosition;
		distanceToLightSource = length(vectorToLightSource);

		//Shadow math
		//Transform points in shadow map
		shadowCoordinate = depthVP * worldMatrix * vec4(aPos, 1.0f);
    }