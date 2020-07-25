#version 330 core
	in vec3 vertexColor;
    in vec3 normalN;
    in vec3 lightVectorL;
    in vec3 eyeVectorV;
    in float distanceToLightSource;

	in vec4 shadowCoordinate;
	
	uniform sampler2D shadowMap;

	out vec4 fragmentColor;

	void main()
	{
		//Shadow math
		vec3 shadowCoord = (shadowCoordinate.xyz / shadowCoordinate.w) * 0.5 + 0.5; 
		float visibility = 1.0f;
		float depthVal = texture(shadowMap, shadowCoord.xy).z;
		float dist = shadowCoord.z;
		float bias = 0.000001f;
		if (dist > depthVal + bias) {
			visibility = 0.0f;
		}


		//Calculate and add ambient, diffuse and specular components

		//ambient
		vec3 ambientIntensity = vec3(0.3f);

		//diffuse
		vec3 diffuseIntensity = vec3(0.3f);

		//specular
		vec3 specularIntensity = vec3(0.3f);
		

		vec3 totalIntensity = ambientIntensity + (visibility * diffuseIntensity) + (visibility * specularIntensity);
			   
		fragmentColor = vec4(vertexColor.r * totalIntensity.r, 
			vertexColor.g * totalIntensity.g, 
			vertexColor.b * totalIntensity.b,
			1.0f);

	}
