#version 330 core
  in vec2 texCoord;
	in vec3 vertexColor;
    in vec3 normalN;
    in vec3 eyeVectorV;
	in vec3 fragPosition;


	in light1 {
		vec3 lightVectorL;
		vec4 shadowCoordinate;
		float distanceToLightSource;
	} light1;
	uniform float isLight1On = 1.0f;
	uniform sampler2D shadowMap;

	in light2 {
		vec3 lightVectorL;
		vec4 shadowCoordinate;
		float distanceToLightSource;
	} light2;
	uniform float isLight2On = 1.0f;
	uniform sampler2D shadowMap2;

	
	uniform float shouldRenderShadows = 1.0f;
	uniform float shouldRenderTextures = 1.0f;
	uniform sampler2D someTexture;	

    uniform vec4 lightCoefficients = vec4(0.3f, 0.8f, 0.5f, 256);
    uniform vec3 lightColor = vec3(1.0f, 1.0f, 1.0f);

	float coeffAmbient;
	float coeffDiffuse;
	float coeffSpecular;
	float coeffShininess;

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

		float getVisibility(vec4 shadowCoordinates, float bias, int percentageCloserFilteringRadius, sampler2D shadowMapToUse) {
			vec3 shadowCoord = (shadowCoordinates.xyz / shadowCoordinates.w) * 0.5 + 0.5; 
			float visibility = 0.0f;
			float dist = shadowCoord.z;

			vec2 shadowMapDimensions = textureSize(shadowMapToUse, 0);	//https://www.khronos.org/opengl/wiki/Sampler_(GLSL)#Texture_size_retrieval
			float shadowMapTexelWidth = 1.0f / shadowMapDimensions.x;
			float shadowMapTexelHeight = 1.0f / shadowMapDimensions.y;
			for (int i = -percentageCloserFilteringRadius; i <= percentageCloserFilteringRadius; ++i) {
				for (int j = -percentageCloserFilteringRadius; j <= percentageCloserFilteringRadius; ++j) {
					float depthVal = texture(shadowMapToUse, vec2(shadowCoord.x + (i*shadowMapTexelWidth), shadowCoord.y + (j*shadowMapTexelHeight))).z;
					if (dist <= depthVal + bias) {
						visibility += 1.0f;
					}
				}
			}
			float sampleDiameter = (percentageCloserFilteringRadius * 2.0f + 1.0f);
			return visibility / (sampleDiameter * sampleDiameter);
		}

		vec3 getDiffuseIntensity(float attenuationFactor, vec3 norm, vec3 lightDirection) {
			float diff = max(dot(norm, lightDirection), 0.0f);
			return attenuationFactor * diff;
		}

		vec3 getSpecularIntensity(float attenuationFactor, vec3 norm, vec3 lightDirection) {
			float coeffShininess = lightCoefficients[3]; //256;
			vec3 viewDirection = normalize(eyeVectorV);
			vec3 reflectDirection = reflect(-lightDirection, norm);
			float spec = pow(max(dot(viewDirection, reflectDirection), 0.0), coeffShininess);
			//eliminate the backface specular light https://stackoverflow.com/questions/20008089/specular-lighting-appears-on-both-eye-facing-and-rear-sides-of-object
			if (dot(viewDirection, norm) < 0.0f) {
				spec = 0.0f; 
			}
			return attenuationFactor * spec;
		}

		vec3 getTotalIntensity(float distToLightSource, vec3 lightVector, vec4 shadowCoordinate, float bias, int percentageCloserFilteringRadius, sampler2D shadowMap, bool isLightOn, float angle) {
			//Initialize variables for lighting attributes
			float coeffAmbient = lightCoefficients[0]; //0.3f;
			float coeffDiffuse = lightCoefficients[1]; //0.8f;
			float coeffSpecular = lightCoefficients[2]; //0.5f;
			float coeffShininess = lightCoefficients[3]; //256;
			
			float visibility = getVisibility(shadowCoordinate, bias, percentageCloserFilteringRadius, shadowMap);

			//attenuation maths following the attenuation formula Id = (kd*Ld) / (a + bq + cq^2) * (l dot n) attenuationFactor being 1/(a + bq + cq^2)
			float attenuationConstantA = 0.5f;
			float attenuationConstantB = 0.01f;
			float attenuationConstantC = 0.0001f;
			float attenuationFactor = 1 / (attenuationConstantA + (attenuationConstantB * distToLightSource) + (attenuationConstantC * distToLightSource * distToLightSource));
			
			vec3 norm = normalize(normalN);
			vec3 lightDirection = normalize(lightVector - fragPosition);

			vec3 ambientIntensity = lightColor * coeffAmbient;
			vec3 diffuseIntensity = (coeffDiffuse * lightColor) * getDiffuseIntensity(attenuationFactor, norm, lightDirection);
			vec3 specularIntensity = (coeffSpecular * lightColor) * getSpecularIntensity(attenuationFactor, norm, lightDirection);
		
			//Phong Lighting Model combines the 3 lighting components
			if (shouldRenderShadows < 0.5f) {
				visibility = 1.0f;
			} 

			//Only for use with spotlights
			//float spotLitness = dot(lightDirection, normalize(lightVector));
			//spotLitness = (spotLitness > angle ? spotLitness : 0.0f);
			float lightOnness = 1.0f;
			if (!isLightOn) {
				lightOnness = 0.0f;
				visibility = 0.0f;	//do not cast shadows when the light is off
			} else {
				//Only for use with spotlights
				//spotLitness = clampIt(spotLitness);	
				//spotLitness = (spotLitness - angle) * 10.0f;
				//lightOnness = clampIt(spotLitness);	
				//if (spotLitness <= bias) {
				//	visibility = 0.0f;
				//}
			}

			return ambientIntensity + ((visibility + lightOnness)/2.0f) * (diffuseIntensity + specularIntensity);
		}

	void main()
	{

		//Shadow math
		float bias = 0.000001f;
		int percentageCloserFilteringRadius = 2;

		float angle = 0.76f; //0.76f is roughly cos(40), our spotlight covers 40 degrees
		vec3 intensity1 = getTotalIntensity(light1.distanceToLightSource, light1.lightVectorL, light1.shadowCoordinate, bias, percentageCloserFilteringRadius, shadowMap, isLight1On > 0.5f, angle);
		vec3 intensity2 = vec3(0.0f);
		if (light2.lightVectorL != vec3(0.0f)) {
			intensity2 = getTotalIntensity(light2.distanceToLightSource, light2.lightVectorL, light2.shadowCoordinate, bias, percentageCloserFilteringRadius, shadowMap2, isLight2On > 0.5f, angle);
		}
		vec3 totalIntensity = intensity1 + intensity2;	
		vec4 texColor = vec4(1.0f);

		if(shouldRenderTextures > 0.5f) {
		  texColor = texture(someTexture, texCoord);
		}

		//Calculate each color channel  (R,G,B) separately
		//Clamp the final result to [0, 1]
		float totalIntensityR = totalIntensity.r;//clampIt(totalIntensity.r);	
		float totalIntensityG = totalIntensity.g;//clampIt(totalIntensity.g);	
		float totalIntensityB = totalIntensity.b;//clampIt(totalIntensity.b);	

    fragmentColor = vec4(texColor.r * totalIntensityR * vertexColor.r, 
      texColor.g * totalIntensityG * vertexColor.g, 
      texColor.b * totalIntensityB * vertexColor.b,
      1.0f);
	}
