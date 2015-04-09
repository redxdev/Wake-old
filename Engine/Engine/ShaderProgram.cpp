#include "ShaderProgram.h"

#include <gl/glew.h>
#include <fstream>

CLOG_LOGGER_DEF(ShaderProgram);

char* ReadFile(const char* path)
{
	std::ifstream in(path);
	if (!in.is_open())
	{
		return nullptr;
	}

	std::string str;
	while (!in.eof())
	{
		std::string tmp;
		std::getline(in, tmp);
		str += tmp + '\n';
	}
	in.close();

	char* buf = new char[str.length() + 1];
	strcpy(buf, str.c_str());
	buf[str.length()] = '\0';

	return buf;
}

GLuint ShaderProgram::LoadShader(const char* path, GLenum shaderType)
{
	char* buf = ReadFile(path);
	if (buf == nullptr)
	{
		CLOG_ERROR("Unable to read shader file " << path);
		return 0;
	}

	const GLchar* sources[] = { buf };
	GLint lengths[] = { strlen(buf) };

	GLuint shader = glCreateShader(shaderType);
	glShaderSource(shader, 1, sources, lengths);
	glCompileShader(shader);

	delete[] buf;

	GLint result;
	glGetShaderiv(shader, GL_COMPILE_STATUS, &result);
	if (result == GL_TRUE)
	{
		return shader;
	}

	glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &result);
	char* log = new char[result + 1];
	log[result] = '\0';

	glGetShaderInfoLog(shader, result, 0, log);
	CLOG_ERROR("Unable to compile shader " << path);
	CLOG_ERROR(log);
	delete[] log;

	glDeleteShader(shader);

	return 0;
}

ShaderProgram* ShaderProgram::LoadProgram(const char* vertPath, const char* fragPath)
{
	GLuint vertShader = LoadShader(vertPath, GL_VERTEX_SHADER);
	if (vertShader == 0)
	{
		CLOG_ERROR("Unable to load vertex shader");
		return nullptr;
	}

	GLuint fragShader = LoadShader(fragPath, GL_FRAGMENT_SHADER);
	if (fragShader == 0)
	{
		CLOG_ERROR("Unable to load fragment shader");
		glDeleteShader(vertShader);
		return nullptr;
	}

	GLuint program = glCreateProgram();
	glAttachShader(program, vertShader);
	glAttachShader(program, fragShader);
	glLinkProgram(program);

	GLint result;
	glGetProgramiv(program, GL_LINK_STATUS, &result);

	if (result == GL_TRUE)
	{
		return new ShaderProgram(program);
	}

	glGetProgramiv(program, GL_INFO_LOG_LENGTH, &result);
	char* log = new char[result + 1];
	log[result] = '\0';

	glGetProgramInfoLog(program, result, 0, log);
	CLOG_ERROR("Unable to link shader program");
	CLOG_ERROR(log);
	delete[] log;

	glDeleteShader(vertShader);
	glDeleteShader(fragShader);
	glDeleteProgram(program);

	return nullptr;
}

ShaderProgram::ShaderProgram(GLuint Program)
{
	this->Program = Program;
}

ShaderProgram::~ShaderProgram()
{
	glDeleteProgram(Program);
}