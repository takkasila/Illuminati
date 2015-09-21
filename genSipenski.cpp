#include <stdio.h>
#include <stdlib.h>

#include "Libraries\GLEW\glew.h"
#include "Libraries\glm\glm\glm.hpp"

#include "genSipenski.h"

using namespace glm;

void triangle(vec3 a, vec3 b, vec3 c)
{

}

void sipenski(GLfloat* v_data, const int depth)
{
	v_data[0] = -1;
	v_data[1] = -1;
	v_data[2] = 0;

	v_data[3] = 1;
	v_data[4] = -1;
	v_data[5] = 0;

	v_data[6] = 0;
	v_data[7] = 1;
	v_data[8] = 0;
}