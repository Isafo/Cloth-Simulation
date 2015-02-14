#include "shader.h"


Shader::Shader () {
	this->programID = 0;
}

/* 
** Constructor, input argument for vertex shader and fragment shader.
** Loads file names, compiles and assembles the shader 
*/
Shader::Shader (const char *vertexFilePath, const char *fragmentFilePath) {
	this->createShader(vertexFilePath, fragmentFilePath);
}

//Destructor
Shader::~Shader () {
	if (programID != 0)
		glDeleteProgram(programID);
}

void Shader::createShader(const char *vertexFilePath, const char *fragmentFilePath) {
	
	GLint isCompiled = 0;
	GLint isLinked = 0;

	//Read the source code in shader files into the buffers
	string vertexSource = readFile(vertexFilePath);
	string fragmentSource = readFile(fragmentFilePath);

	// Create empty vertex shader handle
	GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER);

	// Send the source code in teh shader to GL
	const GLchar *sourceVertex = (const GLchar *)vertexSource.c_str();
	glShaderSource(vertexShader, 1, &sourceVertex, 0);

	glCompileShader(vertexShader);

	glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &isCompiled);
	
	if (isCompiled == GL_FALSE) {
		GLint maxLength = 0;
		glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &maxLength);

		// maxLenght includes the NULL char
		vector<GLchar> infoLog(maxLength);
		glGetShaderInfoLog(vertexShader, GL_INFO_LOG_LENGTH, &maxLength, &infoLog[0]);

		glDeleteShader(vertexShader);
		fprintf(stderr, "%s: %s\n", "Vertex shader compile error", infoLog);

		return;
	}

	// Create empty fragment shader handle
	GLuint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);

	// Send the source code in the shader to GL
	const GLchar *sourceFragment = (const GLchar *)fragmentSource.c_str();
	glShaderSource(fragmentShader, 1, &sourceFragment, 0);

	glCompileShader(fragmentShader);

	glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &isCompiled);

	if (isCompiled == GL_FALSE) {
		GLint maxLength = 0;
		glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &maxLength);

		// maxLenght includes the NULL char
		vector<GLchar> infoLog(maxLength);
		glGetShaderInfoLog(fragmentShader, GL_INFO_LOG_LENGTH, &maxLength, &infoLog[0]);

		glDeleteShader(fragmentShader);
		fprintf(stderr, "%s: %s\n", "Fragment shader compile error", infoLog);

		return;
	}

	// create program object
	GLuint program = glCreateProgram();

	glAttachShader(program, vertexShader);
	glAttachShader(program, fragmentShader);

	glLinkProgram(program);
	glGetProgramiv(program, GL_LINK_STATUS, (int *)&isLinked);

	if (isLinked == GL_FALSE) {
		GLint maxLength = 0;
		glGetProgramiv(program, GL_COMPILE_STATUS, &maxLength);

		// maxLenght includes the NULL char
		vector<GLchar> infoLog(maxLength);
		glGetProgramInfoLog(program, GL_INFO_LOG_LENGTH, &maxLength, &infoLog[0]);

		glDeleteProgram(program);
		glDeleteShader(fragmentShader);
		glDeleteShader(vertexShader);

		fprintf(stderr, "%s: %s\n", "Program object linking error", infoLog);

		return;
	}

	glDetachShader(program, vertexShader);
	glDetachShader(program, fragmentShader);

	programID = program;
}

/*
** Reads the Source code in the shader file into a string
*/
string Shader::readFile(const char *filePath) {

	string sourceCode;
	string line = "";
	ifstream fileStream (filePath);

	if (fileStream.is_open()) {
		
		while (!fileStream.eof()) {

			std::getline(fileStream, line);
			sourceCode.append(line + "\n");
		}
	}

	else {
		cerr << "Could not open file: " << filePath << endl;
		return "";
	}
	
	return sourceCode;
}