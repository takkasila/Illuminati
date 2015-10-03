#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "Libraries\GLEW\glew.h"
#include "Libraries\GLFW\glfw3.h"

#include "texture.h"
GLuint loadBMP_custom(const char* imagepath)
{

	printf("Reading image %s\n", imagepath);

	// Read from header 
	unsigned char header[54]; // Each BMP file begins by a 54-bytes header
	unsigned int dataPos; // Position in the file where the actual data begins
	unsigned int width, height;
	unsigned int imageSize; // = width * height*3
	// Actual RGB data
	unsigned char* data;

	// Open the file
	FILE *file = fopen(imagepath, "rb");

	if (!file)
	{
		printf("Image could not be opened\n");
		getchar(); return 0;
	}
	if (fread(header, 1, 54, file) != 54)
	{
		// If not 54 btes read : problem
		printf("Not a correct BMP file1\n");
		return 0;
	}
	if (header[0] != 'B' || header[1] != 'M')
	{
		printf("Not a correct BMP file2\n");
	}
	// Make sure this is a 24bpp file
	if (*(int*) &(header[0x1E]) != 0)         { printf("Not a correct BMP file\n");    return 0; }
	if (*(int*) &(header[0x1C]) != 24)         { printf("Not a correct BMP file\n");    return 0; }


	//Read ints from the byte array
	dataPos = *(int*) &(header[0x0A]);
	imageSize = *(int*) &(header[0x22]);
	width = *(int*) &(header[0x12]);
	height = *(int*) &(header[0x16]);

	//Some BMP files are misformatted, guess missing information
	if (imageSize == 0)
		imageSize = width*height * 3; //3: one byte for each Red, Green and Blue component
	if (dataPos == 00)
		dataPos = 54; //The BMP header is doen that way

	// Create a buffer
	data = new unsigned char[imageSize];

	// Read the actual data from the file into the buffer
	fread(data, 1, imageSize, file);
	fclose(file);

	GLuint textureID;
	glGenTextures(1, &textureID);
	glBindTexture(GL_TEXTURE_2D, textureID);

	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_BGR, GL_UNSIGNED_BYTE, data);

	//Not a bug
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

	//When MAGnifying the image (no bigger mipmap available), use Linear filter
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	//When MINifying the image, use a Linear blend of two mipmaps, each filtered linearly too
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);

	// Gen Mip maps
	glGenerateMipmap(GL_TEXTURE_2D);
	return textureID;
}
