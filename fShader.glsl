#version  420 core
out vec3 color;

uniform int renderMode;
void main()
{
	if (renderMode == 0)	// Fill
		color = vec3(0, 188, 212);
	else
		color = vec3(0, 0, 0);
}