#pragma once

#include <gl/glew.h>

#include "../Logging/LogMacros.h"

class Uniform
{
public:
	Uniform(GLuint Program, GLuint Location);

	GLuint GetProgram();
	GLuint GetLocation();

	void Set(float x);
	void Set(float x, float y);
	void Set(float x, float y, float z);
	void Set(float x, float y, float z, float w);

private:
	GLuint Program;
	GLuint Location;
};

class ShaderProgram
{
	CLOG_LOGGER_DECL;
public:
	static GLuint LoadShader(const char* Path, GLenum ShaderType);

	static ShaderProgram* LoadProgram(const char* VertPath, const char* FragPath);

public:
	ShaderProgram(GLuint Program);
	~ShaderProgram();

	GLuint GetProgram();

	Uniform GetUniform(const char* Name);

private:
	GLuint Program;
};