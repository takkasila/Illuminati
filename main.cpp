#include <stdio.h>
#include <stdlib.h>

#include "Libraries\GLEW\glew.h"
#include "Libraries\GLFW\glfw3.h"
#include "Libraries\glm\glm\glm.hpp"

#include "LoadShader.h";

using namespace glm;
using namespace std;

GLFWwindow* window;
float window_width = 600;
float window_height = 600;
int main()
{
#pragma region Init
	if (!glfwInit())
	{
		fprintf(stderr, "Failed to init glfw\n");
		return -1;
	}
	glfwWindowHint(GLFW_SAMPLES, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_VERSION_MINOR, 3);
	
	window = glfwCreateWindow(window_width, window_height, "Illuminati", NULL, NULL);
	
	if (window == NULL)
	{
		fprintf(stderr, "Failed to open glfw window");
		glfwTerminate();
		return -1;
	}
	glfwMakeContextCurrent(window);
	
	glewExperimental = true;
	if (glewInit() != GLEW_OK)
	{
		fprintf(stderr, "Failed to init GLEW");
		return -1;
	}
	glfwSetInputMode(window, GLFW_STICKY_KEYS, GL_TRUE);
	glfwSetCursorPos(window, window_width / 2.0, window_height / 2.0);
	glClearColor(0.95, 0.95, 0.95, 0);

	//Z-Buffer
	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LESS);
	glEnable(GL_CULL_FACE);
#pragma endregion

	GLuint programID = LoadShaders("vShader.glsl", "fShader.glsl");

	GLuint VertexArrayID;
	glGenVertexArrays(1, &VertexArrayID);
	glBindVertexArray(VertexArrayID);

	static const GLfloat v_data [] = {
		-1, -1, 0
		, 1, -1, 0
		, 0, 1, 0
	};

	GLuint v_buffer;
	glGenBuffers(1, &v_buffer);
	glBindBuffer(GL_ARRAY_BUFFER, v_buffer);
	glBufferData(GL_ARRAY_BUFFER, sizeof(v_data), v_data, GL_STATIC_DRAW);


	do{
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		glUseProgram(programID);

		glEnableVertexAttribArray(0);
		glBindBuffer(GL_ARRAY_BUFFER, v_buffer);
		glVertexAttribPointer(
			0
			, 3
			, GL_FLOAT
			, GL_FALSE
			, 0
			, (void*) 0
		);

		glDrawArrays(GL_TRIANGLES, 0, 3);
		glDisableVertexAttribArray(0);

		glfwSwapBuffers(window);
		glfwPollEvents();
	} while (glfwGetKey(window, GLFW_KEY_ESCAPE) != GLFW_PRESS
		&& glfwWindowShouldClose(window) == 0);

	return 0;
}