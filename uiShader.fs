#version 330 core
	in vec2 texCoord;
	in vec3 vertexColor;

	uniform sampler2D someTexture;	

	out vec4 fragmentColor;

	void main()
	{
		vec4 texColor = texture(someTexture, texCoord);


		fragmentColor = vec4(texColor.r * vertexColor.r, 
		texColor.g * vertexColor.g, 
		texColor.b * vertexColor.b,
		1.0f);
	}
