#version  420 core
in vec2 UV;
out vec3 color;

uniform int renderMode;
uniform sampler2D textureSampler;
void main()
{
	if (renderMode == 0)	// Fill
		color = texture2D(textureSampler, UV).rgb;
	else
		color = texture2D(textureSampler, UV).rgb;
		//color = vec3(0, 0, 0);
}