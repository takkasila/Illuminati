#include <stdio.h>
#include <stdlib.h>

#include "Libraries\GLEW\glew.h"
#include "Libraries\GLFW\glfw3.h"
#include "Libraries\glm\glm\glm.hpp"
#include "Libraries\FreeImage\FreeImage.h"

#include "LoadShader.h"
#include "genSipenski.h"

using namespace glm;
using namespace std;

GLFWwindow* window;
float window_width = 1200;
float window_height = 900;
int main()
{
#pragma region Init
	if (!glfwInit())
	{
		fprintf(stderr, "Failed to init glfw\n");
		return -1;
	}
	glfwWindowHint(GLFW_SAMPLES, 16);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 2);
	
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
	glClearColor(230 / 255.0
		, 74 / 255.0
		, 25 / 255.0
		, 0);

	//Z-Buffer
	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LESS);
	glEnable(GL_CULL_FACE);
#pragma endregion

	GLuint frameBuffer;
	glGenFramebuffers(1, &frameBuffer);
	glBindBuffer(GL_FRAMEBUFFER, frameBuffer);

	GLuint programID = LoadShaders("vShader.glsl", "fShader.glsl");
	
	GLuint VertexArrayID;
	glGenVertexArrays(1, &VertexArrayID);
	glBindVertexArray(VertexArrayID);
	
	int depth = 10;
	int numVertex = 3 * (int) pow(3, depth);	// Total verticies
	int numVertexAllocate = 3 * numVertex;		// x, y, z per verticies

	GLfloat* v_data = new GLfloat[numVertexAllocate];
	sipenski(v_data, depth);

	GLuint v_buffer;
	glGenBuffers(1, &v_buffer);
	glBindBuffer(GL_ARRAY_BUFFER, v_buffer);
	glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat) * numVertexAllocate, v_data, GL_STATIC_DRAW);

	bool save = true;
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

		glDrawArrays(GL_TRIANGLES, 0, numVertex);
		glDisableVertexAttribArray(0);

		if (save)
		{
			save = false;
			BYTE* pixels = new GLubyte[3 * window_width*window_height];
			
			glReadPixels(0, 0, window_width, window_height
				, GL_RGB
				, GL_UNSIGNED_BYTE
				, pixels);
			FIBITMAP* image = FreeImage_ConvertFromRawBits(pixels, window_width, window_height
				, 3 * window_width, 24
				, 0xFF0000, 0xFF0000, 0xFF0000
				, false);
			FreeImage_Save(FIF_BMP, image, "../Sipenski.bmp", 0);

			FreeImage_Unload(image);
			delete [] pixels;
		}

		glfwSwapBuffers(window);
		glfwPollEvents();
	} while (glfwGetKey(window, GLFW_KEY_ESCAPE) != GLFW_PRESS
		&& glfwWindowShouldClose(window) == 0);

	return 0;
}