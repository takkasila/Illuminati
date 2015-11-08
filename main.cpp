#include <stdio.h>
#include <stdlib.h>
#include <vector>

#include "Libraries\GLEW\glew.h"
#include "Libraries\GLFW\glfw3.h"
#include "Libraries\glm\glm\glm.hpp"
#include "Libraries\FreeImage\FreeImage.h"

#include "LoadShader.h"
#include "genSipenski.h"
#include "controls.h"
#include "texture.h"
#include "vboindexer.h"

using namespace glm;
using namespace std;

GLFWwindow* window;
float window_width = 1200;
float window_height = 900;

extern "C" {
	_declspec(dllexport) DWORD NvOptimusEnablement = 0x00000001;
}

void takeScreenShot()
{
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
	delete[] pixels;
}

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

	GLuint programID = LoadShaders("vShader.glsl", "fShader.glsl");
	
	GLuint VertexArrayID;
	glGenVertexArrays(1, &VertexArrayID);
	glBindVertexArray(VertexArrayID);
	
	GLuint MVPID = glGetUniformLocation(programID, "MVP");
	GLuint ViewMatrixID = glGetUniformLocation(programID, "View");
	GLuint ModelMatrixID = glGetUniformLocation(programID, "Model");
	GLuint renderModeID = glGetUniformLocation(programID, "renderMode");

	int depth = 10;
	//int numVertex = 3 * (int) pow(3, depth);	// 2D
	int numVertex =  3 * 4 * (int) pow(4, depth);	// 3D

	int numVertexAllocate = 3 * numVertex;

	GLfloat* v_data = new GLfloat[numVertexAllocate];
	//sipenski(v_data, depth, 10, 10);
	sipenski3D(v_data, depth, 10);

	GLuint v_buffer;
	glGenBuffers(1, &v_buffer);
	glBindBuffer(GL_ARRAY_BUFFER, v_buffer);
	glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat) * numVertexAllocate, v_data, GL_STATIC_DRAW);
	
	// texture
	GLuint texture = loadBMP_custom("Resources/Illuminati.bmp");
	GLuint TextureID = glGetUniformLocation(programID, "textureSampler");
	GLfloat* uv_data = new GLfloat[numVertex * 2];
	
	for (int f1 = 0; f1 < numVertex * 2; f1+= 6)
	{
		uv_data[f1] = 991/1000.0f;
		uv_data[f1 + 1] = 6 / 1000.0f;

		uv_data[f1 + 2] = 573 / 1000.0f;
		uv_data[f1 + 3] = 995 / 1000.0f;

		uv_data[f1 + 4] = 24 / 1000.0f;
		uv_data[f1 + 5] = 6 / 1000.0f;
	}
	
	GLuint uvBuffer;
	glGenBuffers(1, &uvBuffer);
	glBindBuffer(GL_ARRAY_BUFFER, uvBuffer);
	glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat) * numVertex * 2, uv_data, GL_STATIC_DRAW);


	vector<unsigned int> indices;

	//indexVBO(v_buffer, );

	GLuint elementbuffer;
	glGenBuffers(1, &elementbuffer);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, elementbuffer);
	//glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned int), &indices[0], GL_STATIC_DRAW);

	bool save = true;
	do{
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		computeMatricesFromInputs();
		mat4 projection = getProjectionMatrix();
		mat4 view = getViewMatrix();
		mat4 model = mat4(1.0f);
		mat4 MVP = projection  * view * model;

		glUseProgram(programID);

		glUniformMatrix4fv(MVPID, 1, GL_FALSE, &MVP[0][0]);

		// Texture
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, texture);
		glUniform1i(TextureID, 0);

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

		glEnableVertexAttribArray(1);
		glBindBuffer(GL_ARRAY_BUFFER, uvBuffer);
		glVertexAttribPointer(
			1
			, 2
			, GL_FLOAT
			, GL_FALSE
			, 0
			, (void*) 0
		);


		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
		glUniform1i(renderModeID, 0);	// 0 = fill
		glDrawArrays(GL_TRIANGLES, 0, numVertex);
		
		/*glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, elementbuffer);

		glDrawElements(
			GL_TRIANGLES
			, indices.size()
			, GL_UNSIGNED_INT
			, (void*) 0
			);*/


		// Draw outline
		//glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
		//glUniform1i(renderModeID, 1);	// 1 = line
		//glDrawArrays(GL_TRIANGLES, 0, numVertex);

		glDisableVertexAttribArray(0);


		if (save)
		{
			save = false;
			takeScreenShot();
		}

		glfwSwapBuffers(window);
		glfwPollEvents();

	} while (glfwGetKey(window, GLFW_KEY_ESCAPE) != GLFW_PRESS
		&& glfwWindowShouldClose(window) == 0);


	glDeleteBuffers(1, &v_buffer);
	glDeleteProgram(programID);
	glDeleteVertexArrays(1, &VertexArrayID);
	glDeleteTextures(1, &texture);

	glfwTerminate();

	return 0;
}