#pragma once

#include <string>
#include <GL\glew.h>

GLuint loadShader(const std::string& filename, GLenum type);

GLuint createProgram(GLuint* shaders, int count);