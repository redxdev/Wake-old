#pragma once

#include <gl/glew.h>
#include <glm/fwd.hpp>

#include "../Logging/LogMacros.h"

/**
 * Utility to encapsulate a OpenGL uniform.
 */
class Uniform
{
public:
	Uniform(GLuint Program, GLuint Location);

	Uniform(const Uniform& Other);

	GLuint GetProgram();
	GLuint GetLocation();

	void Set(float x);
	void Set(float x, float y);
	void Set(float x, float y, float z);
	void Set(float x, float y, float z, float w);

	void Set(const glm::vec2& xy);
	void Set(const glm::vec3& xyz);
	void Set(const glm::vec4& xyzw);

	void Set(const glm::mat2x2& m22);
	void Set(const glm::mat2x3& m23);
	void Set(const glm::mat2x4& m24);

	void Set(const glm::mat3x2& m32);
	void Set(const glm::mat3x3& m33);
	void Set(const glm::mat3x4& m34);

	void Set(const glm::mat4x2& m42);
	void Set(const glm::mat4x3& m43);
	void Set(const glm::mat4x4& m44);

private:
	GLuint Program;
	GLuint Location;
};

/**
 * Utility to encapsulate an OpenGL shader program.
 */
class ShaderProgram
{
	CLOG_LOGGER_DECL;
public:
	static GLuint LoadFile(const char* Path, GLenum ShaderType);

	static GLuint LoadString(const char* Str, GLenum ShaderType);

	static GLuint LoadProgram(GLuint VertexShader, GLuint FragmentShader);

public:
	ShaderProgram(GLuint Program);
	~ShaderProgram();

	GLuint GetProgram();

	Uniform GetUniform(const char* Name);

	void Use();

private:
	GLuint Program;
};