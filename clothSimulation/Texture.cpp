#include "Texture.h"

GLuint loadBMP(const char * imagePath){

	unsigned char header[54]; // BMB files has a 54-bytes header 
	unsigned int dataPos, // Position where the actual data begins
	unsigned int width, height, imageSize; // imageSize = width*height*3 , 3 colorchannels
	unsigned char *data;
	
	//open the file
	FILE * file = fopen(imagePath, "rb"); // rb for r = read and b = binary file

	if (!file) {
		printf("Could not open image\n");
		return 0;
	}

	//verify that the file is a BMP file
	if (fread(header, 1, 54, file) != 54 || header[0] != 'B' || header[1] != 'M'){
		printf("File is not a BMP\n");
		return 0;
	}

	// Read from the byte array
	dataPos = *(int*)&(header[0x0A]);
	imageSize = *(int*)&(header[0x22]);
	width = *(int*)&(header[0x12]);
	height = *(int*)&(header[0x16]);

	// Some BMP files are missing these
	if (imageSize == 0)
		imageSize = width*height * 3;

	if (dataPos == 0)
		dataPos = 54;

	data = new unsigned char[imageSize];
	fread(data, 1, imageSize, file);
	fclose(file); // close file after it has been read into data

	// Create a OpenGl texture
	GLuint textureID;
	glGenTextures(1, &textureID);

	glBindTexture(GL_TEXTURE_2D, textureID);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_BGR, GL_UNSIGNED_BYTE, data); // BMP stores as BGR and not RGB

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);

}
