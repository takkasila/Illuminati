#include <stdio.h>
#include <stdlib.h>

#include "Libraries\GLEW\glew.h"
#include "Libraries\glm\glm\glm.hpp"

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

void sipenski(GLfloat* v_data, int depth)
{
	i = 0;
	
	vec3 a = { -1, -1, 0 };
	vec3 b = { 1, -1, 0 };
	vec3 c = { 0, 1, 0 };

	divide_triangle(v_data, depth, a, b, c);
	
	int k = 5;
	return;
}