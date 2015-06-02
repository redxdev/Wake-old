#include "Shader.h"

#include <gl/glew.h>
#include <glm/gtc/type_ptr.hpp>

#include <fstream>

Uniform::Uniform(GLuint Program, GLuint Location)
{
	this->Program = Program;
	this->Location = Location;
}

Uniform::Uniform(const Uniform& Other)
{
	Program = Other.Program;
	Location = Other.Location;
}

GLuint Uniform::GetProgram()
{
	return Program;
}

GLuint Uniform::GetLocation()
{
	return Location;
}

void Uniform::Set(float x)
{
	glProgramUniform1f(Program, Location, x);
}

void Uniform::Set(float x, float y)
{
	glProgramUniform2f(Program, Location, x, y);
}

void Uniform::Set(float x, float y, float z)
{
	glProgramUniform3f(Program, Location, x, y, z);
}

void Uniform::Set(float x, float y, float z, float w)
{
	glProgramUniform4f(Program, Location, x, y, z, w);
}

void Uniform::Set(const glm::vec2& xy)
{
	glProgramUniform2fv(Program, Location, 1, glm::value_ptr(xy));
}

void Uniform::Set(const glm::vec3& xyz)
{
	glProgramUniform3fv(Program, Location, 1, glm::value_ptr(xyz));
}

void Uniform::Set(const glm::vec4& xyzw)
{
	glProgramUniform4fv(Program, Location, 1, glm::value_ptr(xyzw));
}

void Uniform::Set(const glm::mat2x2& m22)
{
	glProgramUniformMatrix2fv(Program, Location, 1, false, glm::value_ptr(m22));
}

void Uniform::Set(const glm::mat2x3& m23)
{
	glProgramUniformMatrix2x3fv(Program, Location, 1, false, glm::value_ptr(m23));
}

void Uniform::Set(const glm::mat2x4& m24)
{
	glProgramUniformMatrix2x4fv(Program, Location, 1, false, glm::value_ptr(m24));
}

void Uniform::Set(const glm::mat3x2& m32)
{
	glProgramUniformMatrix3x2fv(Program, Location, 1, false, glm::value_ptr(m32));
}

void Uniform::Set(const glm::mat3x3& m33)
{
	glProgramUniformMatrix3fv(Program, Location, 1, false, glm::value_ptr(m33));
}

void Uniform::Set(const glm::mat3x4& m34)
{
	glProgramUniformMatrix3x4fv(Program, Location, 1, false, glm::value_ptr(m34));
}

void Uniform::Set(const glm::mat4x2& m42)
{
	glProgramUniformMatrix4x2fv(Program, Location, 1, false, glm::value_ptr(m42));
}

void Uniform::Set(const glm::mat4x3& m43)
{
	glProgramUniformMatrix4x3fv(Program, Location, 1, false, glm::value_ptr(m43));
}

void Uniform::Set(const glm::mat4x4& m44)
{
	glProgramUniformMatrix4fv(Program, Location, 1, false, glm::value_ptr(m44));
}


CLOG_LOGGER_DEF(ShaderProgram);

char* ReadFile(const char* Path)
{
	std::ifstream in(Path);
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

GLuint ShaderProgram::LoadFile(const char* Path, GLenum ShaderType)
{
	char* buf = ReadFile(Path);
	if (buf == nullptr)
	{
		CLOG_ERROR("Unable to read shader file " << Path);
		return 0;
	}

	GLuint result = LoadString(buf, ShaderType);

	delete[] buf;

	return result;
}

GLuint ShaderProgram::LoadString(const char* Str, GLenum ShaderType)
{
	if (Str == nullptr)
	{
		CLOG_ERROR("Cannot load null string as a shader");
		return 0;
	}

	const GLchar* sources[] = { Str };
	GLint lengths[] = { strlen(Str) };
	GLuint shader = glCreateShader(ShaderType);
	glShaderSource(shader, 1, sources, lengths);
	glCompileShader(shader);

	GLint result;
	glGetShaderiv(shader, GL_COMPILE_STATUS, &result);
	if (result != GL_FALSE)
	{
		return shader;
	}

	glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &result);
	char* log = new char[result + 1];
	log[result] = '\0';

	glGetShaderInfoLog(shader, result, 0, log);
	CLOG_ERROR("Unable to compile shader");
	CLOG_ERROR(log);
	delete[] log;

	glDeleteShader(shader);

	return 0;
}

GLuint ShaderProgram::LoadProgram(GLuint VertexShader, GLuint FragmentShader)
{
	GLuint program = glCreateProgram();
	glAttachShader(program, VertexShader);
	glAttachShader(program, FragmentShader);
	glLinkProgram(program);

	GLint result;
	glGetProgramiv(program, GL_LINK_STATUS, &result);

	if (result != GL_FALSE)
	{
		return program;
	}

	glGetProgramiv(program, GL_INFO_LOG_LENGTH, &result);
	char* log = new char[result + 1];
	log[result] = '\0';

	glGetProgramInfoLog(program, result, 0, log);
	CLOG_ERROR("Unable to link shader program");
	CLOG_ERROR(log);
	delete[] log;

	glDeleteProgram(program);

	return 0;
}

ShaderProgram::ShaderProgram(GLuint Program)
{
	this->Program = Program;
}

ShaderProgram::~ShaderProgram()
{
	glDeleteProgram(Program);
}

GLuint ShaderProgram::GetProgram()
{
	return Program;
}

Uniform ShaderProgram::GetUniform(const char* Name)
{
	return Uniform(Program, glGetUniformLocation(Program, Name));
}

void ShaderProgram::Use()
{
	glUseProgram(Program);
}