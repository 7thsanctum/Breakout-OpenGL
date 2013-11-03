#include "shader.h"
#include <iostream>
#include <fstream>
#include <sstream>

std::string readFile(const std::string& filename)  //C++ Standard method of loading in a file
{
	std::ifstream file(filename);  //Open file as input file stream (ifstream)
	std::stringstream buffer;  //Create string stream to act as buffer to read file into
	buffer << file.rdbuf();  //Read file into the buffer
	std::string content = buffer.str();  //Get the internal string of buffer and use it as file content
	return content;
}

GLuint loadShader(const std::string& filename, GLenum type)  //Load shader operation
{
	std::string fileContent = readFile(filename);  //Readd in file of given name
	GLuint shader = glCreateShader(type);  //Create shader object of relevant type given
	const char* source = fileContent.c_str();  //Get underlying character array from read in file
	glShaderSource(shader, 1, &source, 0);  //Set the shader source for the created shader object to the character array
	glCompileShader(shader);  //Compile the shader

	GLint compiled;                                            //
	glGetShaderiv(shader, GL_COMPILE_STATUS, &compiled);       //Check to see if shader compiled
	if (!compiled)  //If not cimpiled                                          
	{
		GLsizei length;
		char log[1024];
		glGetShaderInfoLog(shader, 1024, &length, log);  //If not compiled, get shader compile log, allocating space to read this into
		std::cout << "Could not compile shader: " << shader << ": " << filename << std::endl;
		std::cout << log << std::endl;
		glDeleteShader(shader);
		return 0;
	}
	return shader;  //Return shader object
}

GLuint createProgram(GLuint* shaders, int count)  //Create the program
{
	GLuint program = glCreateProgram();  //Create a program
	for (int i = 0; i < count; ++i)  //Attach all shader objects to program
		glAttachShader(program, shaders[i]);
	glLinkProgram(program);  //Link the program

	GLint linked;                                         //
	glGetProgramiv(program, GL_LINK_STATUS, &linked);     //Check to see if program linked ok
	if(!linked)  //If program not linked
	{
		GLsizei length;
		char log[1024];
		glGetShaderInfoLog(program, 1024, &length, log);  //If not linked, get program link log, allocating space to read this into
		std::cout << "Error linnking program.: " << std::endl;
		std::cout << log << std::endl;
		for (int i = 0; i < count; ++i)
			glDetachShader(program, shaders[i]);
		glDeleteShader(program);
		return 0;
	}
	return program;  //Return the program
}

//1.Create the vertex shader object
//2.Create the fragment shader object
//3.Create the program object from the vertex and fragment shader