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
	uniform float coeffAmbient;
	uniform float coeffDiffuse;
	uniform float coeffSpecular;
	uniform int coeffShininess;
	uniform vec3 lightColor;

	out vec4 fragmentColor;

		float clampIt(float value) {
		if (value < 0.0f) {
		return 0.0f;
		} else if (value > 1.0f) {
			return 1.0f;
		} else {
			return value;
		}
	}

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

		//ambient
		vec3 ambientIntensity = lightColor * coeffAmbient;

		//attenuation maths following the attenuation formula Id = (kd*Ld) / (a + bq + cq^2) * (l dot n) attenuationFactor being 1/(a + bq + cq^2)
		float attenuationConstantA = 0.5f;
		float attenuationConstantB = 0.01f;
		float attenuationConstantC = 0.0001f;
		float attenuationFactor = 1 / (attenuationConstantA + (attenuationConstantB * distanceToLightSource) + (attenuationConstantC * distanceToLightSource * distanceToLightSource));

		//diffuse
		vec3 norm = normalize(normalN);
		vec3 lightDirection = normalize(lightVectorL - fragPosition);
		float diff = max(dot(norm, lightDirection), 0.0f);
		vec3 diffuseIntensity = attenuationFactor * (coeffDiffuse * lightColor) * diff;
		
		//specular
		vec3 viewDirection = normalize(eyeVectorV - fragPosition);
		vec3 reflectDirection = reflect(-lightDirection, norm);
		float spec = pow(max(dot(viewDirection, reflectDirection), 0.0), coeffShininess);
		vec3 specularIntensity = attenuationFactor * coeffSpecular * spec * lightColor;
		
		//Phong Lighting Model combines the 3 lighting components
		vec3 totalIntensity;
		if (shouldRenderShadows > 0.5f) {
			totalIntensity = ambientIntensity + (visibility * diffuseIntensity) + (visibility * specularIntensity);
		} else {
			  totalIntensity = ambientIntensity + diffuseIntensity + specularIntensity;
		}

		//Calculate each color channel  (R,G,B) separately
		//Clamp the final result to [0, 1]
		float totalIntensityR = clampIt(totalIntensity.r);	
		float totalIntensityG = clampIt(totalIntensity.g);	
		float totalIntensityB = clampIt(totalIntensity.b);	
			   

		fragmentColor = vec4(vertexColor.r * totalIntensityR, 
			vertexColor.g * totalIntensityG, 
			vertexColor.b * totalIntensityB,
			1.0f);
	}
