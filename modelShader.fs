#version 330 core
	in vec3 vertexColor;
	out vec4 color;
	uniform vec4 model_color;

	void main()
	{
		color = vec4(model_color.r,model_color.g,model_color.b,model_color.a);
	}