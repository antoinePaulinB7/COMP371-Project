#version 330 core
	in vec3 vertexColor;
    in vec3 normalN;
    in vec3 lightVectorL;
    in vec3 eyeVectorV;
    in float distanceToLightSource;
	in vec3 fragPosition;

	in vec4 shadowCoordinate;

	uniform float shouldRenderShadows = 1.0f;
	uniform sampler2D shadowMap;

	out vec4 fragmentColor;

	void main()
	{
		//Initialize variables for lighting attributes
		float coeffAmbient = 0.5f;
		float coeffDiffuse = 0.3f;
		float coeffSpecular = 0.3f;
		int coeffShininess = 32;
		vec3 lightColor = vec3(1.0f, 1.0f, 1.0f);
		
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
		vec3 ambientIntensity = lightColor * coeffAmbient;

		//diffuse
		vec3 norm = normalize(normalN);
		vec3 lightDirection = normalize(lightVectorL - fragPosition);
		float diff = max(dot(norm, lightDirection), 0.0f);
		vec3 diffuseIntensity = coeffDiffuse * diff * lightColor;
		
		//specular
		vec3 viewDirection = normalize(eyeVectorV - fragPosition);
		vec3 reflectDirection = reflect(-lightDirection, norm);
		float spec = pow(max(dot(viewDirection, reflectDirection), 0.0), coeffShininess);
		vec3 specularIntensity = coeffSpecular * spec * lightColor;

		//Phong Lighting Model combines the 3 lighting components
		vec3 totalIntensity;
		if (shouldRenderShadows > 0.5f) {
			totalIntensity = ambientIntensity + (visibility * diffuseIntensity) + (visibility * specularIntensity);
		} else {
			   totalIntensity = ambientIntensity + diffuseIntensity + specularIntensity;
		}
			   
		vec3 result = totalIntensity * vertexColor;
		fragmentColor = vec4(result, 1.0);
	}
