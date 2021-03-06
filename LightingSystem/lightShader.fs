#version 330 core
  in vec2 texCoord;
	in vec3 vertexColor;
    in vec3 normalN;
    in vec3 eyeVectorV;
	in vec3 fragPosition;

	uniform sampler2D someTexture;	

	in LIGHT_1 {
		vec3 lightVectorL;
		vec3 lightPointingDir;
		vec4 shadowCoordinate;
		float distanceToLightSource;
	} light1;
	uniform float isLight1On = 1.0f;
	uniform sampler2D shadowMap;

	in LIGHT_2 {
		vec3 lightVectorL;
		vec3 lightPointingDir;
		vec4 shadowCoordinate;
		float distanceToLightSource;
	} light2;
	uniform float isLight2On = 1.0f;
	uniform sampler2D shadowMap2;
	
	in LIGHT_3 {
		vec3 lightVectorL;
		vec3 lightPointingDir;
		vec4 shadowCoordinate;
		float distanceToLightSource;
	} light3;
	uniform float isLight3On = 1.0f;
	uniform sampler2D shadowMap3;

	in LIGHT_4 {
		vec3 lightVectorL;
		vec3 lightPointingDir;
		vec4 shadowCoordinate;
		float distanceToLightSource;
	} light4;
	uniform float isLight4On = 1.0f;
	uniform sampler2D shadowMap4;

	in LIGHT_5 {
		vec3 lightVectorL;
		vec3 lightPointingDir;
		vec4 shadowCoordinate;
		float distanceToLightSource;
	} light5;
	uniform float isLight5On = 1.0f;
	uniform sampler2D shadowMap5;

	in LIGHT_6 {
		vec3 lightVectorL;
		vec3 lightPointingDir;
		vec4 shadowCoordinate;
		float distanceToLightSource;
	} light6;
	uniform float isLight6On = 1.0f;
	uniform sampler2D shadowMap6;

	in LIGHT_7 {
		vec3 lightVectorL;
		vec3 lightPointingDir;
		vec4 shadowCoordinate;
		float distanceToLightSource;
	} light7;
	uniform float isLight7On = 1.0f;
	uniform sampler2D shadowMap7;

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
			return vec3(clampIt(attenuationFactor * diff));
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
			return vec3(clampIt(attenuationFactor * spec));
		}

		vec3 getTotalIntensity(float distToLightSource, vec3 lightVector, vec3 lightPointingDir, vec4 shadowCoordinate, float bias, int percentageCloserFilteringRadius, sampler2D shadowMap, bool isLightOn, float angle) {
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

			vec3 diffuseIntensity = (coeffDiffuse * lightColor) * getDiffuseIntensity(attenuationFactor, norm, lightDirection);
			vec3 specularIntensity = (coeffSpecular * lightColor) * getSpecularIntensity(attenuationFactor, norm, lightDirection);

			//Only for use with spotlights
			float spotLitness = dot(lightDirection, lightPointingDir);
			spotLitness = (spotLitness > angle ? spotLitness : 0.0f);
			float lightOnness = 1.0f;
			if (!isLightOn) {
				lightOnness = 0.0f;
				visibility = 0.0f;	//do not cast shadows when the light is off
			} else {
				//Only for use with spotlights
				spotLitness = clampIt(spotLitness);	
				spotLitness = (spotLitness - angle) * 10.0f;
				lightOnness = clampIt(spotLitness);	
				if (spotLitness <= bias) {
					visibility = 0.0f;
				}
			}

			return ((visibility + lightOnness)/2.0f) * (diffuseIntensity + specularIntensity);
		}

	void main()
	{

		//Shadow math
		float bias = 0.000001f;
		int percentageCloserFilteringRadius = 2;

		float angle = 0.9f; //0.76f is roughly cos(40), our spotlight covers 40 degrees
		vec3 intensity1 = getTotalIntensity(light1.distanceToLightSource, light1.lightVectorL, light1.lightPointingDir, light1.shadowCoordinate, bias, percentageCloserFilteringRadius, shadowMap, isLight1On > 0.5f, angle);

		vec3 intensity2 = vec3(0.0f);
		if (light2.lightVectorL != vec3(0.0f)) {
			intensity2 = getTotalIntensity(light2.distanceToLightSource, light2.lightVectorL, light2.lightPointingDir, light2.shadowCoordinate, bias, percentageCloserFilteringRadius, shadowMap2, isLight2On > 0.5f, angle);
		}

		vec3 intensity3 = vec3(0.0f);
		if (light3.lightVectorL != vec3(0.0f)) {
			intensity3 = getTotalIntensity(light3.distanceToLightSource, light3.lightVectorL, light3.lightPointingDir, light3.shadowCoordinate, bias, percentageCloserFilteringRadius, shadowMap3, isLight3On > 0.5f, angle);
		}

		vec3 intensity4 = vec3(0.0f);
		if (light4.lightVectorL != vec3(0.0f)) {
			intensity4 = getTotalIntensity(light4.distanceToLightSource, light4.lightVectorL, light4.lightPointingDir, light4.shadowCoordinate, bias, percentageCloserFilteringRadius, shadowMap4, isLight4On > 0.5f, angle);
		}

		vec3 intensity5 = vec3(0.0f);
		if (light5.lightVectorL != vec3(0.0f)) {
			intensity5 = getTotalIntensity(light5.distanceToLightSource, light5.lightVectorL, light5.lightPointingDir, light5.shadowCoordinate, bias, percentageCloserFilteringRadius, shadowMap5, isLight5On > 0.5f, angle);
		}

		vec3 intensity6 = vec3(0.0f);
		if (light6.lightVectorL != vec3(0.0f)) {
			intensity6 = getTotalIntensity(light6.distanceToLightSource, light6.lightVectorL, light6.lightPointingDir, light6.shadowCoordinate, bias, percentageCloserFilteringRadius, shadowMap6, isLight6On > 0.5f, angle);
		}

		vec3 intensity7 = vec3(0.0f);
		if (light7.lightVectorL != vec3(0.0f)) {
			intensity7 = getTotalIntensity(light7.distanceToLightSource, light7.lightVectorL, light7.lightPointingDir, light7.shadowCoordinate, bias, percentageCloserFilteringRadius, shadowMap7, isLight7On > 0.5f, angle);
		}

		float coeffAmbient = lightCoefficients[0]; //0.3f;
		vec3 ambientIntensity = vec3(coeffAmbient);
		vec3 totalIntensity = ambientIntensity + intensity1 + intensity2 + intensity3 + intensity4 + intensity5 + intensity6 + intensity7;	
		vec4 texColor = texture(someTexture, texCoord);

		//Calculate each color channel  (R,G,B) separately
		//Clamp the final result to [0, 1]
		float totalIntensityR = clampIt(totalIntensity.r);	
		float totalIntensityG = clampIt(totalIntensity.g);	
		float totalIntensityB = clampIt(totalIntensity.b);	

    fragmentColor = vec4(texColor.r * totalIntensityR * vertexColor.r, 
      texColor.g * totalIntensityG * vertexColor.g, 
      texColor.b * totalIntensityB * vertexColor.b,
      1.0f);
	}
