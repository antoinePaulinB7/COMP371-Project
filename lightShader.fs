#version 330 core
	in vec3 vertexColor;
    in vec3 normalN;
    in vec3 lightVectorL;
    in vec3 eyeVectorV;
    in float distanceToLightSource;

	uniform vec3 matCoefficientKa;	//ambient reflection: 0 <= Ka <= 1
	uniform vec3 matCoefficientKd;	//diffuse reflection: 0 <= Kd <= 1
	uniform vec3 matCoefficientKs;	//specular reflection: 0 <= Ks <= 1
	uniform vec3 matCoefficientShininessA;
	uniform vec3 lightCoefficientColor;
	uniform vec3 lightComponentAmbientLa;
	uniform vec3 lightComponentDiffuseLd;
	uniform vec3 lightComponentSpecularLs;
	uniform float lightCoefficientAttenuationConstantA;
	uniform float lightCoefficientAttenuationConstantB;
	uniform float lightCoefficientAttenuationConstantC;

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
		//Calculate and add ambient, diffuse and specular components

		//ambient
		vec3 ambientIntensity = matCoefficientKa * lightComponentAmbientLa;

		//diffuse
		//float distanceQuotient = lightCoefficientAttenuationConstantA + lightCoefficientAttenuationConstantB * distanceToLightSource + lightCoefficientAttenuationConstantC * distanceToLightSource * distanceToLightSource;
		float distanceQuotient = 1.0f;
		vec3 diffuseIntensity = (matCoefficientKd * lightComponentDiffuseLd * dot(lightVectorL, normalN)) / distanceQuotient;
		if (diffuseIntensity.r < 0.0f) {
			diffuseIntensity = vec3(0.0f);
		}

		//specular
		vec3 r = 2*(dot(lightVectorL, normalN)) * normalN - lightVectorL; //r = reflection of l at p (determined by l and n)
		vec3 specularIntensity = pow((matCoefficientKs * lightComponentSpecularLs * dot(eyeVectorV, r)), matCoefficientShininessA) / distanceQuotient;
		if (specularIntensity.r < 0.0f) {
			specularIntensity = vec3(0.0f);
		}
		
		vec3 totalIntensity = ambientIntensity + diffuseIntensity + specularIntensity;


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
