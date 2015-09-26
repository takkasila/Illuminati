#include <stdio.h>
#include <stdlib.h>

#include "Libraries\GLEW\glew.h"
#include "Libraries\glm\glm\glm.hpp"
#include "Libraries\glm\glm\gtc\matrix_transform.hpp"

#include "genSipenski.h"

using namespace glm;
int i = 0;
void point(GLfloat* v_data, vec3 a)
{
	v_data[i++] = a.x;
	v_data[i++] = a.y;
	v_data[i++] = a.z;

}

void triangle(GLfloat* v_data, vec3 a, vec3 b, vec3 c)
{
	point(v_data, a);
	point(v_data, b);
	point(v_data, c);

}

void divide_triangle(GLfloat* v_data, int depth, vec3 a, vec3 b, vec3 c)
{

	if (depth > 0)
	{
		vec3 v0, v1, v2;
		v0 = (a + b) / 2.0f;
		v1 = (b + c) / 2.0f;
		v2 = (a + c) / 2.0f;

		triangle(v_data, v0, v1, v2);

		divide_triangle(v_data, depth - 1, a, v0, v2);
		divide_triangle(v_data, depth - 1, v0, b, v1);
		divide_triangle(v_data, depth - 1, v2, v1, c);
	}
	
}

void sipenski(GLfloat* v_data, int depth, float width, float height)
{
	i = 0;
	width /= 2;
	height /= 2;
	vec3 a = { -1 * width, -1 * height, 0 };
	vec3 b = { 1 * width, -1 * height, 0 };
	vec3 c = { 0 * width, 1 * height, 0 };

	divide_triangle(v_data, depth, a, b, c);
	
}

void divide_tetrahedron(GLfloat* v_data, int depth, vec3 a, vec3 b, vec3 c, vec3 d)
{
	if (depth > 0)
	{
		vec3 Vac = (a + c) / 2.0f;
		vec3 Vab = (a + b) / 2.0f;
		vec3 Vbc = (b + c) / 2.0f;
		
		vec3 Vbd = (b + d) / 2.0f;
		vec3 Vdc = (d + c) / 2.0f;

		vec3 Vad = (a + d) / 2.0f;

		divide_tetrahedron(v_data, depth - 1, a, Vab, Vac, Vad);
		divide_tetrahedron(v_data, depth - 1, Vab, b, Vbc, Vbd);
		divide_tetrahedron(v_data, depth - 1, Vac, Vbc, c, Vdc);
		divide_tetrahedron(v_data, depth - 1, Vad, Vbd, Vdc, d);

	}
	else
	{
		triangle(v_data, a, b, c);
		triangle(v_data, b, d, c);
		triangle(v_data, d, a, c);
		triangle(v_data, a, d, b);
	}
}

void sipenski3D(GLfloat* v_data, int depth, float radius)
{
	i = 0;
	float PI = pi<float>();
	float turnRate = (2 / 3.0f)*PI;
	float theta = PI / 2 + turnRate;
	
	vec3 a = { cosf(theta), 0, sinf(theta) };
	theta += turnRate;
	vec3 b = { cosf(theta), 0, sinf(theta) };
	theta += turnRate;
	vec3 c = { cosf(theta), 0, sinf(theta) };
	vec3 d = { 0, 1, 0 };
	
	a *= radius;
	b *= radius;
	c *= radius;
	d *= radius;

	divide_tetrahedron(v_data, depth, a, b, c, d);
}