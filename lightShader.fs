#version 330 core
  in vec2 texCoord;
	in vec3 vertexColor;
    in vec3 normalN;
    in vec3 lightVectorL;
    in vec3 eyeVectorV;
    in float distanceToLightSource;
	in vec3 fragPosition;

	in vec4 shadowCoordinate;

	uniform float shouldRenderShadows = 1.0f;
	uniform sampler2D shadowMap;
  uniform sampler2D someTexture;

  uniform vec4 lightCoefficients = vec4(0.3f, 0.8f, 0.5f, 256);
  uniform vec3 lightColor = vec3(1.0f, 1.0f, 1.0f);

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
		//Initialize variables for lighting attributes
		float coeffAmbient = lightCoefficients[0]; //0.3f;
		float coeffDiffuse = lightCoefficients[1]; //0.8f;
		float coeffSpecular = lightCoefficients[2]; //0.5f;
		float coeffShininess = lightCoefficients[3]; //256;
		
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

    vec4 texColor = texture(someTexture, texCoord);

		//Calculate each color channel  (R,G,B) separately
		//Clamp the final result to [0, 1]
		float totalIntensityR = totalIntensity.r;//clampIt(totalIntensity.r);	
		float totalIntensityG = totalIntensity.g;//clampIt(totalIntensity.g);	
		float totalIntensityB = totalIntensity.b;//clampIt(totalIntensity.b);	
			   
		fragmentColor = vec4(texColor.r * totalIntensityR, 
			texColor.g * totalIntensityG, 
			texColor.b * totalIntensityB,
			1.0f);
	}
