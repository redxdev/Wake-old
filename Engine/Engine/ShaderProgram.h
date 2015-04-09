#pragma once

#include <SFML/OpenGL.hpp>

#include "../Logging/LogMacros.h"

class ShaderProgram
{
	CLOG_LOGGER_DECL;
public:
	static GLuint LoadShader(const char* path, GLenum shaderType);

	static ShaderProgram* LoadProgram(const char* vertPath, const char* fragPath);

public:
	ShaderProgram(GLuint Program);
	~ShaderProgram();

private:
	GLuint Program;
};