#version 330 core
    layout (location = 0) in vec3 aPos; 
	layout (location = 1) in vec3 aColor;
	layout (location = 2) in vec3 aNormal;
	
    uniform vec3 cameraPosition;
    uniform vec3 lightPosition;
    uniform mat4 worldMatrix;
	uniform mat4 viewMatrix = mat4(1.0);
	uniform mat4 projectionMatrix = mat4(1.0);

    out vec3 vertexColor;
    out vec3 normalN;		//surface normal vector
    out vec3 lightVectorL;	//unit vector to light source
    out vec3 eyeVectorV;	//unit vector to viewer
    out float distanceToLightSource;

    void main()
    {
        vertexColor = aColor;

		//Transform vertex positions to Clip Space
		mat4 mvp = projectionMatrix * viewMatrix * worldMatrix;
        gl_Position = mvp * vec4(aPos, 1.0f);

		//Calculate the distance to the light source
		vec3 posToLight = lightPosition - vec3(worldMatrix * vec4(aPos, 1.0f));
		distanceToLightSource = length(posToLight);

		//Transform lighting data to View Space
		normalN = normalize(vec3(viewMatrix * worldMatrix * vec4(aNormal, 0.0f)));
		lightVectorL = normalize(vec3(viewMatrix * vec4(posToLight, 0.0f))); 
		eyeVectorV = normalize(vec3(viewMatrix * vec4((cameraPosition - vec3(worldMatrix * vec4(aPos, 1.0f))), 0.0f)));
		
    }
